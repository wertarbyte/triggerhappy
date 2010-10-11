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
#include "readerlist.h"
#include "keystate.h"
#include "executer.h"

#ifndef NOTHREADS

#include <pthread.h>

// PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP looks like a GNU thing
#ifndef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
{ { 0, 0, 0, PTHREAD_MUTEX_RECURSIVE_NP, 0, { 0 } } }
#endif

pthread_mutex_t keystate_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_mutex_t reader_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
/* keeps track of the number of reader threads */
pthread_cond_t reader_count_cv = PTHREAD_COND_INITIALIZER;

#define LOCK(mutex) pthread_mutex_lock(&mutex)
#define UNLOCK(mutex) pthread_mutex_unlock(&mutex)

/* list of all devices with their handling threads */
static struct readerlist *readers = NULL;

#else

#define LOCK(x) 
#define UNLOCK(x)

#endif // NOTHREADS

static char* script_basedir = NULL;
static char* command_pipe = NULL;
static int dump_events = 0;

char* lookup_event_name(struct input_event ev) {
	if (ev.type == EV_KEY) {
		return (KEY_MAX >= ev.code ? KEY_NAME[ ev.code ] : NULL);
	}
	if (ev.type == EV_SW) {
		return (SW_MAX >= ev.code ? SW_NAME[ ev.code ] : NULL);
	}
	return NULL;
}

/*
 * Look up event and key names and print them to STDOUT
 */
void print_event(struct input_event ev) {
	char *typename = EV_NAME[ ev.type ];
	char *evname = lookup_event_name( ev );
	if ( evname != NULL ) {
		printf( "%s\t%s\t%d\n", typename, evname, ev.value );
	} else {
		fprintf( stderr, "Unknown %s event id: %d (value %d)\n", typename, ev.code, ev.value );
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
				LOCK(keystate_mutex);
				change_keystate( ev );
				if (dump_events) {
					print_event( ev );
					print_keystate();
				}
				if (script_basedir != NULL)
					launch_script( script_basedir, ev );
				UNLOCK(keystate_mutex);
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
	devname = (char *) ptr;
	read_events( devname );
	// thread has done its work, remove device from list
	LOCK(reader_mutex);
	remove_device( devname, &readers );
	UNLOCK(reader_mutex);
	// luckily, we don't pthread_cancel ourselves
	fprintf(stderr, "Device %s removed\n", devname);
	pthread_cond_signal(&reader_count_cv);
}

void add_device(char *dev, struct readerlist **list) {
	// make sure any previous instances are removed
	remove_device( dev, list );
	// append struct to list
	if (*list == NULL) {
		*list = malloc(sizeof(**list));
		(*list)->reader.devname = dev;
		(*list)->next = NULL;
		pthread_create( &((*list)->reader.thread), NULL, &reader_thread, (void *)dev );
	} else {
		add_device( dev, &((*list)->next) );
	}
}

int remove_device(char *dev, struct readerlist **list) {
	if (*list != NULL) {
		struct inputreader *r = &( (*list)->reader );
		if ( strcmp( r->devname, dev ) == 0 ) {
			struct readerlist *copy = *list;
			/* we don't cancel ourselves */
			if (pthread_equal(r->thread, pthread_self()) == 0) {
				pthread_cancel( r->thread );
			}
			*list = (*list)->next;
			free(copy);
			return 1;
		} else {
			return remove_device( dev, &( (*list)->next) );
		}
	} else {
		/* reached the end */
		return 0;
	}
}

int count_readers(struct readerlist **list) {
	if (*list == NULL) {
		return 0;
	} else {
		return 1 + count_readers( &( (*list)->next ) );
	}
}

#endif

int main(int argc, char *argv[]) {
	signal(SIGCHLD, SIG_IGN);
	int c;
	while ((c = getopt(argc, argv, "ds:c:")) != -1) {
		switch (c) {
			case 'd':
				dump_events = 1;
				break;
			case 's':
				script_basedir = optarg;
				break;
			case 'c':
				command_pipe = optarg;
				break;
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
	return start_readers(argc, argv, optind);
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
		/* remove newline character */
		char *nl = NULL;
		if ( (nl = strstr(line, "\n")) != NULL ) {
			*nl = '\0';
		}

		if (read < 1)
			continue;

		/* copy argument */
		int l = (read)*sizeof(char);
		char *dev = malloc(l);
		strncpy( dev, &line[1], l);

		LOCK(reader_mutex);
		if (line[0] == '+') {
			fprintf(stderr, "Adding device '%s'\n", dev);
			add_device( dev, &readers );
		} else if (line[0] == '-') {
			fprintf(stderr, "Removing device '%s'\n", dev);
			remove_device( dev, &readers );
		}
		UNLOCK(reader_mutex);
	}
	free(line);
	return 1;
}

int start_readers(int argc, char *argv[], int start) {
	if (argc-start < 1 && command_pipe == NULL) {
		fprintf(stderr, "No input device files or command pipe specified.\n");
		return 1;
	} else {
#ifndef NOTHREADS
		// create one thread for every device file supplied
		int i;
		for (i=start; i<argc; i++) {
			char *dev = argv[i];
			LOCK(reader_mutex);
			add_device( dev, &readers );
			UNLOCK(reader_mutex);
		}

		while (read_commands()) {}

		// Now we have to wait until all threads terminate
		LOCK(reader_mutex);
		while(count_readers( &readers ) > 0) {
			pthread_cond_wait(&reader_count_cv, &reader_mutex);
		}
		UNLOCK(reader_mutex);
		fprintf(stderr, "Terminating...\n");

#else
		// without threading, we only handle the first device file named
		read_events( argv[start] );
#endif
	}
	return 0;
}
