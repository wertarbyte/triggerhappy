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
#include <pwd.h>
#include <grp.h>

#include "thd.h"
#include "eventnames.h"
#include "devices.h"
#include "keystate.h"
#include "trigger.h"
#include "command.h"
#include "cmdsocket.h"
#include "obey.h"
#include "ignore.h"
#include "uinput.h"

/* version information */
#include "version.h"

/* command channel & FD */
static char *cmd_file = NULL;
static int cmd_fd = -1;

static char *uinput_dev = NULL;

static int dump_events = 0;
static int run_as_daemon = 0;

static char *triggerfile = NULL;
static char *pidfile = NULL;

static keystate_holder *keystate = NULL;

static ignore *ignored_keys = NULL;

static char *user = NULL;

static int exiting = 0;
static int reload_conf = 0;

/* forward declarations */
static int reload_triggerfile();

/* FDs to watch */
static fd_set rfds;
static int max_fd = -1;

/*
 * Look up event and key names and print them to STDOUT
 */
static void print_event(char* dev, struct input_event ev) {
	const char *typename = lookup_type_name( ev );
	const char *evname = lookup_event_name( ev );
	if ( evname != NULL ) {
		printf( "%s\t%s\t%d\t%s\n", typename, evname, ev.value, dev );
	} else {
		fprintf( stderr, "Unknown %s event id on %s : %d (value %d)\n", typename, dev, ev.code, ev.value );
	}
	fflush(stdout);
}

static void print_triggerline(struct input_event ev, keystate_holder ksh) {
	const char *evname = lookup_event_name( ev );
	char *state = ( ev.type == EV_KEY ? get_keystate_ignore_key( ksh, ev.code ) : "");
	const char *d = strlen(state)>0 ? "+" : "";
	if ( evname != NULL ) {
		if (ev.type == EV_KEY) {
			printf( "# %s%s%s\t%d\tcommand\n", state, d, evname, ev.value );
		}
		fflush(stdout);
	}
	free(state);
}

/*
 * Read event from fd, decode it and pass it to handlers
 */
static int read_event( device *dev ) {
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
		if (ev.type == EV_KEY && is_ignored( ev.code, ignored_keys)) {
			return 0;
		}
		if (dump_events) {
			print_event( devname, ev );
			print_triggerline( ev, *keystate );
		}
		run_triggers( ev.type, ev.code, ev.value, *keystate );
		change_keystate( *keystate, ev );
	}
	return 0;
}

static void check_device( device *d ) {
	int fd = d->fd;
	if (FD_ISSET( fd, &rfds )) {
		if (read_event( d )) {
			/* read error? Remove the device! */
			remove_device( d->devname );
		}
	}
}

static void process_devices(void) {
	for_each_device( &check_device );
}

static void add_device_to_fdset( device *d ) {
	int fd = d->fd;
	if (fd > max_fd) {
		max_fd = fd;
	}
	FD_SET( fd, &rfds );
}


static void process_events(void) {
	struct timeval tv;
	int retval;
	/* loop as long as we have at least one device or
	 * the command channel
	 */
	while ( count_devices() > 0 || cmd_fd != -1 ) {
		FD_ZERO( &rfds );
		max_fd = -1;
		/* add device fds */
		for_each_device( &add_device_to_fdset );
		/* add command channel */
		if (cmd_fd != -1) {
			FD_SET( cmd_fd, &rfds );
			max_fd = cmd_fd > max_fd ? cmd_fd : max_fd;
		}
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		retval = select(max_fd+1, &rfds, NULL, NULL, &tv);
		if (retval == -1 && errno != EINTR) {
			perror("select()");
			continue;
		} else if (exiting) {
			break;
		} else if (reload_conf) {
			reload_conf = 0;
			reload_triggerfile();
			continue;
		} else if (retval) {
			process_devices();
			if ( cmd_fd != -1 && FD_ISSET( cmd_fd, &rfds ) ) {
				struct command *cmd = read_command( cmd_fd );
				obey_command( cmd );
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
	fprintf(pid, "%u\n", (unsigned int) getpid());
	return fclose(pid);
}

static struct option long_options[] = {
	{"dump",	no_argument, &dump_events, 1},
	{"daemon",	no_argument, &run_as_daemon, 1},
	{"user",	required_argument, 0, 'u'},
	{"pidfile",	required_argument, 0, 'p'},
	{"triggers",	required_argument, 0, 't'},
	{"socket",	required_argument, 0, 's'},
	{"ignore",	required_argument, 0, 'i'},
	{"help",	no_argument, 0, 'h'},
	{"uinput",	required_argument, 0, '<'},
	{"listevents",	no_argument, 0, 'l'},
	{0,0,0,0} /* end of list */
};

void show_help(void) {
	printf( "Triggerhappy event daemon " TH_VERSION "\n\n" );
	printf( "Usage:\n" );
	printf( "  thd [switches] [devices]\n\n" );
	printf( "Command line switches:\n" );
	printf( "  --help             Display this help message\n" );
	printf( "  --daemon           Run as daemon process\n" );
	printf( "  --dump             Dump events to console\n" );
	printf( "  --listevents       Print a list of all known events\n" );
	printf( "  --triggers <file>  Load trigger definitions from <file>\n" );
	printf( "  --socket <socket>  Read commands from socket\n" );
	printf( "  --ignore <event>   Ignore key events with name <event>\n" );
	printf( "  --user <name>      Drop privileges to <name> after opening devices\n" );
}

static void list_events(void) {
	int n = 0;
	for (n = 0; n < KEY_MAX; n++) {
		const char *name = lookup_event_name_i( EV_KEY, n );
		if (name) {
			printf( "%s\n", name );
		}
	}
	for (n = 0; n < SW_MAX; n++) {
		const char *name = lookup_event_name_i( EV_SW, n );
		if (name) {
			printf( "%s\n", name );
		}
	}
}

void cleanup(void) {
	close_uinput();
	if (cmd_file) {
		if (cmd_fd != -1) {
			close( cmd_fd );
		}
		unlink(cmd_file);
		cmd_fd = -1;
	}
	clear_devices();
	if (pidfile) {
		unlink(pidfile);
	}
}

static int reload_triggerfile(void) {
	clear_triggers();
	if (triggerfile) {
		int err = read_triggers(triggerfile);
		if (err) {
			fprintf(stderr, "Error loading triggerfile '%s'\n", triggerfile);
			return 1;
		}
	}
	return 0;
}

static void handle_signal(int sig) {
	switch (sig) {
		case SIGINT:
		case SIGTERM:
			exiting = 1;
			break;
		case SIGHUP:
			reload_conf = 1;
			break;
	}
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

	/* add every device file supplied on command line */
	int i;
	for (i=start; i<argc; i++) {
		char *dev = argv[i];
		/* TODO
		 * we should provide a method to
		 * optionally grab the device or
		 * assign a tag
		 */
		int grab_dev = 0;
		char *tag_dev = NULL;
		add_device( dev, -1, grab_dev, tag_dev );
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
	/* drop privileges */
	if (user) {
		struct passwd *pw = getpwnam(user);
		if (pw) {
			int uid = pw->pw_uid;
			int gid = pw->pw_gid;
			if ( initgroups(user, gid) ) {
				perror("initgroups");
				return 1;
			}
			if ( setregid( gid, gid ) ) {
				perror("setregid");
				return 1;
			}
			if ( setreuid( uid, uid ) ) {
				perror("setreuid");
				return 1;
			}
		} else {
			fprintf(stderr, "Unknown username '%s'.\n", user);
			return 1;
		}
	}

	process_events();

	return 0;
}

int main(int argc, char *argv[]) {
	signal(SIGCHLD, SIG_IGN);
	int option_index = 0;
	int c;
	while (1) {
		c = getopt_long (argc, argv, "t:s:dhpi:u:", long_options, &option_index);
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
				triggerfile = optarg;
				break;
			case 's':
				cmd_file = optarg;
				break;
			case 'i':
				if ( lookup_event_type(optarg) == EV_KEY ) {
					ignore_key( lookup_event_code(optarg), &ignored_keys );
				}
				break;
			case 'h':
				show_help();
				return 0;
			case 'l':
				list_events();
				return 0;
			case 'u':
				user = optarg;
				break;
			case '<':
				uinput_dev = optarg;
				break;
			case '?':
			default:
				return 1;
		}
	}
	/* load config file */
	if (reload_triggerfile() != 0) {
		return 1;
	}
	/* init keystate holder */
	init_keystate_holder(&keystate);
	/* set initial trigger mode */
	change_trigger_mode("");
	/* open uinput if requested */
	if (uinput_dev != NULL && open_uinput(uinput_dev) == -1) {
		fprintf(stderr, "Error setting up uinput support\n");
		return 1;
	}
	/* install signal handler */
	struct sigaction handler;
	handler.sa_handler = handle_signal;
	sigfillset(&handler.sa_mask);
	handler.sa_flags=0;
	sigaction(SIGINT,&handler,0);
	sigaction(SIGTERM,&handler,0);
	sigaction(SIGHUP,&handler,0);

	int status = start_readers(argc, argv, optind);
	cleanup();
	return status;
}

