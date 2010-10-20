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
#include <sys/select.h>
#include <sys/time.h>

#include "eventnames.h"
#include "device.h"
#include "keystate.h"
#include "triggerdir.h"
#include "trigger.h"

/* list of all devices with their FDs */
static device *devs = NULL;

/* command FIFO */
static char* command_pipe = NULL;
static int cmdfd = -1;

/* command buffer */
#define MAXCMD 1024
static char cmdbuffer[MAXCMD] = {};

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
 * Read event from fd, decode it and pass it to handlers
 */
int read_event( device *dev ) {
	int fd = dev->fd;
	char *devname = dev->devname;
	struct input_event ev;
	int n = read( fd, &ev, sizeof(ev) );
	if ( n != sizeof(ev) ) {
		fprintf(stderr, "Error reading device '%s'\n", dev->devname);
		return 1;
	}
	/* ignore all events except KEY and SW */
	if (ev.type == EV_KEY || ev.type == EV_SW) {
		change_keystate( *keystate, ev );
		if (dump_events) {
			print_event( devname, ev );
			print_keystate( *keystate );
		}
		if (script_basedir != NULL) {
			run_triggerdir( script_basedir, ev );
		}
		run_triggers( ev.type, ev.code, ev.value );
	}
	return 0;
}

void add_device(char *dev, device **list) {
	device **p = list;
	// find end of list
	while (*p != NULL) {
		p = &( (*p)->next );
	}
	int fd = open( dev, O_RDONLY );
	if (fd >= 0) {
		*p = malloc(sizeof(**list));
		(*p)->devname = strdup(dev);
		(*p)->fd = fd;
		(*p)->next = NULL;
	} else {
		fprintf( stderr, "Error opening '%s': %s\n", dev, strerror(errno) );
	}
}

int remove_device(char *dev, device **list) {
	device **p = list;
	while (*p != NULL) {
		if ( strcmp( (*p)->devname, dev ) == 0 ) {
			device *copy = *p;
			/* bypass the list item */
			*p = copy->next;
			close(copy->fd);
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

int count_devices(device **list) {
	int n = 0;
	device **p = list;
	while (*p != NULL) {
		n++;
		p = &( (*p)->next );
	}
	return n;
}

static int open_cmd(void) {
	cmdfd = open( command_pipe, O_RDONLY | O_NONBLOCK );
	if (cmdfd < 0) {
		fprintf(stderr, "Unable to open command fifo '%s': %s\n", command_pipe, strerror(errno));
		free(command_pipe);
		command_pipe = NULL;
		return 1;
	}
	return 0;
}

static void process_commandline( char *line ) {
	const char delimiters[] = " \t";
	char *op, *dev;
	op = strtok( line, delimiters );
	if (op == NULL) {
		return;
	}

	dev = strtok( NULL, delimiters );

	if (strcmp("ADD", op) == 0 && dev != NULL) {
		fprintf(stderr, "Adding device '%s'\n", dev);
		/* make sure we remove double devices */
		remove_device( dev, &devs );
		add_device( dev, &devs );
	} else if (strcmp("REMOVE", op) == 0 && dev != NULL) {
		fprintf(stderr, "Removing device '%s'\n", dev);
		remove_device( dev, &devs );
	}
}

static void read_command_pipe(void) {
	/* length of command currently buffered */
	int len = strlen(cmdbuffer);
	/* remaining data to fill it */
	int rem = MAXCMD - len;
	char buf[rem];
	int done = read( cmdfd, &buf, rem-1 );
	if (done == 0) {
		/* the client has closed the connection,
		 * so we have to reopen the pipe and clear the buffer
		 */
		cmdbuffer[0] = '\0';
		int r = close(cmdfd);
		open_cmd();
		return;
	}
	/* append the data read to our buffer - 
	 * we made sure before that we do not read more
	 * than we can handle
	 */
	strcat(cmdbuffer, buf);
	/* do we have a newline yet? */
	char *nl = NULL;
	while ( nl = strchr(cmdbuffer, '\n') ) {
		/* split and process */
		*nl = '\0';
		process_commandline( cmdbuffer );
		cmdbuffer[0] = '\0';
		/* now we copy the entire remaining string to the beginning */
		if (nl < cmdbuffer+MAXCMD) {
			/* TODO nice source of off-by-one-errors, I have to rethink this */
			strcpy( cmdbuffer, nl+1 );
		}
	}
}


static int add_to_fdset( fd_set *fds, device **list ) {
	int max = 0;
	device **p = list;
	while (*p != NULL) {
		int fd = (*p)->fd;
		if (fd > max) {
			max = fd;
		}

		FD_SET( fd, fds );
		p = &( (*p)->next );
	}
	return max;
}

void process_devices( fd_set *fds, device **list ) {
	device **p = list;
	while (*p != NULL) {
		int fd = (*p)->fd;
		char *dev = (*p)->devname;
		if (FD_ISSET( fd, fds )) {
			if (read_event( *p )) {
				/* read error? Remove the device! */
				remove_device( (*p)->devname, &devs );
				return;
			}
		}
		p = &( (*p)->next );
	}
}

static void process_events( device **list ) {
	fd_set rfds;
	struct timeval tv;
	int retval;
	/* loop as long as we have at least one device or
	 * the command channel
	 */
	while ( count_devices(&devs) > 0 || cmdfd != -1 ) {
		FD_ZERO( &rfds );
		int maxfd = 0;
		maxfd = add_to_fdset( &rfds, &devs );
		/* add command channel */
		if (cmdfd != -1) {
			FD_SET( cmdfd, &rfds );
			maxfd = cmdfd > maxfd ? cmdfd : maxfd;
		}
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
		if (retval == -1) {
			perror("select()");
		} else if (retval) {
			process_devices( &rfds, &devs );
			if ( cmdfd != -1 && FD_ISSET( cmdfd, &rfds ) ) {
				read_command_pipe();
			}
		}
	}
}

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
				read_triggerfile(optarg);
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
	/* init keystate holder */
	init_keystate_holder(&keystate);
	return start_readers(argc, argv, optind);
}

int start_readers(int argc, char *argv[], int start) {
	if (argc-start < 1 && command_pipe == NULL) {
		fprintf(stderr, "No input device files or command pipe specified.\n");
		return 1;
	}
	/* open command pipe */
	if (command_pipe) {
		open_cmd();
		if (cmdfd < 0) {
			return 1;
		}
	}

	// create one thread for every device file supplied
	int i;
	for (i=start; i<argc; i++) {
		char *dev = argv[i];
		add_device( dev, &devs );
	}
	process_events( &devs );
	return 0;
}
