/*
 * thd.c - the triggerhappy daemon
 * by Stefan Tomanek <stefan.tomanek@wertarbyte.de>
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <stdlib.h>
#include <sys/wait.h>

#include "eventnames.h"
#include "devreader.h"
#include "keystate.h"
#include "triggerdir.h"
#include "trigger.h"

#ifndef NOTHREADS

#include <pthread.h>

// PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP looks like a GNU thing
#ifndef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
{ { 0, 0, 0, PTHREAD_MUTEX_RECURSIVE_NP, 0, { 0 } } }
#endif

/* locked while processing an event */
pthread_mutex_t processing_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_mutex_t readerlist_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
/* keeps track of the number of reader threads */
pthread_cond_t readerlist_count_cv = PTHREAD_COND_INITIALIZER;

#define LOCK(mutex) pthread_mutex_lock(&mutex)
#define UNLOCK(mutex) pthread_mutex_unlock(&mutex)

/* list of all devices with their handling threads */
static devreader *readers = NULL;

static char* command_pipe = NULL;
#else

#define LOCK(x) 
#define UNLOCK(x)

#endif // NOTHREADS

static trigger *triggers = NULL;
static char* script_basedir = NULL;
static int dump_events = 0;

static keystate_holder *keystate = NULL;

/*
 * Look up event and key names and print them to STDOUT
 */
void print_event(char* dev, struct input_event ev) {
	char *typename = lookup_type_name( ev );
	char *evname = lookup_event_name( ev );
	if ( evname != NULL ) {
		printf( "%s\t%s\t%d\t%s\n", typename, evname, ev.value, dev );
	} else {
		fprintf( stderr, "Unknown %s event id on %s : %d (value %d)\n", typename, dev, ev.code, ev.value );
	}
	fflush(stdout);
}

/*
 * Read events from device file, decode them and print them to STDOUT
 */
int read_events(char *devname) {
	int dev;
	dev = open(devname, O_RDONLY);
	if (dev < 0) {
		fprintf(stderr, "Unable to open device file '%s': %s\n", devname, strerror(errno));
		return 1;
	} else {
		struct input_event ev;
		while(1) {
			int n = read( dev, &ev, sizeof(ev) );
			if ( n != sizeof(ev) ) {
				fprintf(stderr, "Read error\n");
				return 1;
			}
			/* ignore all events except KEY and SW */
			if (ev.type == EV_KEY || ev.type == EV_SW) {
				LOCK(processing_mutex);
				change_keystate( *keystate, ev );
				if (dump_events) {
					print_event( devname, ev );
					print_keystate( *keystate );
				}
				if (script_basedir != NULL)
					run_triggerdir( script_basedir, ev );
				run_triggers( ev.type, ev.code, ev.value, triggers );
				UNLOCK(processing_mutex);
			}
		}
		close(dev);
	}
	return 0;
}

#ifndef NOTHREADS
void* reader_thread(void* ptr) {
	// detach myself
	pthread_detach(pthread_self());
	char *devname;
	devname = strdup((char *) ptr);
	read_events( devname );
	// thread has done its work, remove device from list
	LOCK(readerlist_mutex);
	remove_device( devname, &readers );
	UNLOCK(readerlist_mutex);
	// luckily, we don't pthread_cancel ourselves
	fprintf(stderr, "Device %s removed\n", devname);
	free(devname);
	pthread_cond_signal(&readerlist_count_cv);
}

void add_device(char *dev, devreader **list) {
	devreader **p = list;
	// find end of list
	while (*p != NULL) {
		p = &( (*p)->next );
	}
	*p = malloc(sizeof(**list));
	(*p)->devname = strdup(dev);
	(*p)->next = NULL;
	pthread_create( &((*p)->thread), NULL, &reader_thread, (void *)(*p)->devname );
}

int remove_device(char *dev, devreader **list) {
	devreader **p = list;
	while (*p != NULL) {
		if ( strcmp( (*p)->devname, dev ) == 0 ) {
			devreader *copy = *p;
			/* we don't cancel ourselves */
			if (pthread_equal((*p)->thread, pthread_self()) == 0) {
				pthread_cancel( (*p)->thread );
			}
			/* bypass the list item */
			*p = copy->next;
			free(copy->devname);
			free(copy);
			return 1;
		}
		/* advance to the next node */
		p = &( (*p)->next );
	}
	/* reached the end of the list */
	return 0;
}

int count_readers(devreader **list) {
	int n = 0;
	devreader **p = list;
	while (*p != NULL) {
		n++;
		p = &( (*p)->next );
	}
	return n;
}

int read_commands(void) {
	if (command_pipe == NULL) {
		/* don't continue */
		return 0;
	}
	FILE *pipe;
	int len = 0;
	char *line = NULL;
	ssize_t read;
	pipe = fopen(command_pipe, "r");
	if (pipe == NULL) {
		return 0;
	}
	while ((read = getline(&line, &len, pipe)) != -1) {
		if (read < 1)
			continue;

		const char delimiters[] = " \t\n";
		char *sptr;
		char *op, *dev;
		op = strtok_r( line, delimiters, &sptr );
		dev = strtok_r( NULL, delimiters, &sptr );

		LOCK(readerlist_mutex);
		if (strcmp("ADD", op) == 0 && dev != NULL) {
			fprintf(stderr, "Adding device '%s'\n", dev);
			/* make sure we remove double devices */
			remove_device( dev, &readers );
			add_device( dev, &readers );
		} else if (strcmp("REMOVE", op) == 0 && dev != NULL) {
			fprintf(stderr, "Removing device '%s'\n", dev);
			remove_device( dev, &readers );
		}
		UNLOCK(readerlist_mutex);
	}
	free(line);
	return 1;
}

#endif

int main(int argc, char *argv[]) {
	signal(SIGCHLD, SIG_IGN);
	int c;
	while ((c = getopt(argc, argv, "ds:c:e:")) != -1) {
		switch (c) {
			case 'd':
				dump_events = 1;
				break;
			case 's':
				script_basedir = optarg;
				break;
			case 'e':
				read_triggerfile(optarg, &triggers);
				break;
#ifndef NOTHREADS
			case 'c':
				command_pipe = optarg;
				break;
#endif
			case '?':
				if (optopt == 's' || optopt == 'c') {
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				} else if (isprint(optopt)) {
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				} else {
					fprintf(stderr, "Unknown option character\n");
				}
				return 1;
		}
	}
	/* init keystate holder */
	init_keystate_holder(&keystate);
	printf("Initialized keystate_holder\n");
	return start_readers(argc, argv, optind);
}

int start_readers(int argc, char *argv[], int start) {
#ifndef NOTHREADS
	if (argc-start < 1 && command_pipe == NULL) {
		fprintf(stderr, "No input device files or command pipe specified.\n");
		return 1;
	}
	// create one thread for every device file supplied
	int i;
	for (i=start; i<argc; i++) {
		char *dev = argv[i];
		LOCK(readerlist_mutex);
		add_device( dev, &readers );
		UNLOCK(readerlist_mutex);
	}

	while (read_commands()) {}

	// Now we have to wait until all threads terminate
	LOCK(readerlist_mutex);
	while(count_readers( &readers ) > 0) {
		pthread_cond_wait(&readerlist_count_cv, &readerlist_mutex);
	}
	UNLOCK(readerlist_mutex);
	fprintf(stderr, "Terminating...\n");

#else
	if (argc-start < 1) {
		fprintf(stderr, "No input device files specified.\n");
		return 1;
	}
	// without threading, we only handle the first device file named
	read_events( argv[start] );
#endif
	return 0;
}
