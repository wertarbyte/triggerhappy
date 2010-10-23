/*
 * thd.c - the triggerhappy daemon
 * by Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
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
#include <getopt.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>

#include "thd.h"
#include "eventnames.h"
#include "devices.h"
#include "keystate.h"
#include "trigger.h"
#include "command.h"
#include "cmdsocket.h"
#include "obey.h"

/* list of all devices with their FDs */
static device *devs = NULL;

static char *cmd_file = NULL;
static int cmd_fd = -1;

static int dump_events = 0;
static int run_as_daemon = 0;

static char *pidfile = NULL;

static keystate_holder *keystate = NULL;

static int exiting = 0;

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
		run_triggers( ev.type, ev.code, ev.value, *keystate );
	}
	return 0;
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
	while ( count_devices(&devs) > 0 || cmd_fd != -1 ) {
		FD_ZERO( &rfds );
		int maxfd = 0;
		maxfd = add_to_fdset( &rfds, &devs );
		/* add command channel */
		if (cmd_fd != -1) {
			FD_SET( cmd_fd, &rfds );
			maxfd = cmd_fd > maxfd ? cmd_fd : maxfd;
		}
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
		if (retval == -1) {
			perror("select()");
		} else if (exiting) {
			break;
		} else if (retval) {
			process_devices( &rfds, &devs );
			if ( cmd_fd != -1 && FD_ISSET( cmd_fd, &rfds ) ) {
				struct command *cmd = read_command( cmd_fd );
				obey_command( cmd, &devs );
				free(cmd);
			}
		}
	}
}

static int write_pidfile( char *pidfile ) {
	FILE *pid = fopen( pidfile, "w+" );
	if (pid == NULL) {
		return 1;
	}
	fprintf(pid, "%u\n", getpid());
	fclose(pid);
	return 0;
}

static struct option long_options[] = {
	{"dump",	no_argument, &dump_events, 1},
	{"daemon",	no_argument, &run_as_daemon, 1},
	{"pidfile",	required_argument, 0, 'p'},
	{"triggers",	required_argument, 0, 't'},
	{"socket",	required_argument, 0, 's'},
	{"help",	no_argument, 0, 'h'},
	{0,0,0,0} /* end of list */
};

void show_help(void) {
	printf( "Triggerhappy event daemon\n\n" );
	printf( "Usage:\n" );
	printf( "  thd [switches] [devices]\n\n" );
	printf( "Command line switches:\n" );
	printf( "  --help             Display this help message\n" );
	printf( "  --daemon           Run as daemon process\n");
	printf( "  --dump             Dump events to console\n");
	printf( "  --triggers <file>  Load trigger definitions from <file>\n");
	printf( "  --socket <socket>  Read commands from socket\n");
}

void cleanup(void) {
	if (cmd_file) {
		if (cmd_fd != -1) {
			close( cmd_fd );
		}
		unlink(cmd_file);
	}
	clear_devices( &devs );
	if (pidfile) {
		unlink(pidfile);
	}
}

static void handle_signal(int sig) {
	exiting = 1;
}

int main(int argc, char *argv[]) {
	signal(SIGCHLD, SIG_IGN);
	int option_index = 0;
	int c;
	while (1) {
		c = getopt_long (argc, argv, "t:s:dhp", long_options, &option_index);
		if (c == -1) {
			break;
		}
		switch (c) {
			case 0:
				if (long_options[option_index].flag != 0) {
					break;
				}
				printf ("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
				printf ("\n");
				break;
			case 'd': /* short for --dump */
				dump_events = 1;
				break;
			case 'p': /* short for --pidfile */
				pidfile = optarg;
				break;
			case 't':
				read_triggerfile(optarg);
				break;
			case 's':
				cmd_file = optarg;
				break;
			case 'h':
				show_help();
				return 0;
			case '?':
			default:
				return 1;
		}
	}
	/* init keystate holder */
	init_keystate_holder(&keystate);
	/* install signal handler */
	struct sigaction handler;
	handler.sa_handler = handle_signal;
	sigfillset(&handler.sa_mask);
	handler.sa_flags=0;
	sigaction(SIGINT,&handler,0);
	sigaction(SIGTERM,&handler,0);

	return start_readers(argc, argv, optind);
}

int start_readers(int argc, char *argv[], int start) {
	if (argc-start < 1 && cmd_file == NULL) {
		fprintf(stderr, "No input device files or command pipe specified.\n");
		return 1;
	}
	/* open command pipe */
	if (cmd_file) {
		cmd_fd = bind_cmdsocket(cmd_file);
		if (cmd_fd < 0) {
			return 1;
		}
	}

	// create one thread for every device file supplied
	int i;
	for (i=start; i<argc; i++) {
		char *dev = argv[i];
		add_device( dev, &devs );
	}
	if (run_as_daemon) {
		int err = daemon(0,0);
		if (err) {
			perror("daemon()");
		}
	}
	if (pidfile) {
		write_pidfile( pidfile );
	}
	process_events( &devs );

	cleanup();
	return 0;
}
