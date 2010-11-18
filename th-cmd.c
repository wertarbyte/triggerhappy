
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>
#include <getopt.h>

#include "devices.h"
#include "command.h"
#include "cmdsocket.h"

void show_help(void) {
	fprintf( stderr, "Use:\n");
	fprintf( stderr, "  th-cmd --socket <socket> [--passfd] --add <devices...>\n");
	fprintf( stderr, "  th-cmd --socket <socket> --remove <devices...>\n");
	fprintf( stderr, "  th-cmd --socket <socket> --clear\n");
	fprintf( stderr, "  th-cmd --socket <socket> --enable\n");
	fprintf( stderr, "  th-cmd --socket <socket> --disable\n");
	fprintf( stderr, "  th-cmd --socket <socket> --mode <mode>\n");
	fprintf( stderr, "  th-cmd --socket <socket> --quit\n");
	fprintf( stderr, "  th-cmd --socket <socket> --help\n");
}

int main(int argc, char *argv[]) {
	char *socket = NULL;
	static int passfd = 0;
	static int op_add = 0;
	static int op_rem = 0;
	static int op_clear = 0;
	static int op_udev = 0;
	static int op_en = 0;
	static int op_dis = 0;
	static int op_mode = 0;
	static int op_quit = 0;
	enum command_type ctype = -1;

	static struct option long_options[] = {
		{ "socket", 1, 0, 's' },
		{ "add", 0, &op_add, 1 },
		{ "remove", 0, &op_rem, 1 },
		{ "clear", 0, &op_clear, 1 },
		{ "udev", 0, &op_udev, 1 },
		{ "passfd", 0, &passfd, 1 },
		{ "enable", 0, &op_en, 1 },
		{ "disable", 0, &op_dis, 1 },
		{ "mode", 0, &op_mode, 1 },
		{ "quit", 0, &op_quit, 1 },
		{ "help", 0, 0, 'h' },
		{ 0, 0, 0, 0 }
	};

	int c;
	while (1) {
		int option_index = 0;
		c = getopt_long(argc, argv, "s:", long_options, &option_index);
		if (c == -1) {
			break;
		}
		switch(c) {
			case 's':
				socket = optarg;
				break;
			case 'h':
			case '?':
			case -1:
				show_help();
				return 1;
		}
	}

	if ( op_add + op_rem + op_clear + op_udev + op_en + op_dis + op_mode + op_quit != 1 ) {
		fprintf(stderr, "A single command must be specified!\n");
		show_help();
		return 1;
	}

	if (! socket ) {
		fprintf(stderr, "No socket specified!\n");
		show_help();
		return 1;
	}
	int s = connect_cmdsocket( socket );
	if (s < 0) {
		perror("connect()");
		return 1;
	}

	int err = 0;
	if (op_udev) {
		if (strcasecmp("add", getenv("ACTION")) == 0) {
			ctype = CMD_ADD;
		} else if (strcasecmp("remove", getenv("ACTION")) == 0) {
			ctype = CMD_REMOVE;
		}
		char *dev = getenv("DEVNAME");
		if ( ctype && dev ) {
			err = send_command( s, ctype, dev, passfd );
		}
	} else {
		/* get devices from command line */
		if (op_add) ctype = CMD_ADD;
		else if (op_rem) ctype = CMD_REMOVE;
		else if (op_clear) ctype = CMD_CLEARDEVS;
		else if (op_en) ctype = CMD_ENABLE;
		else if (op_dis) ctype = CMD_DISABLE;
		else if (op_mode) ctype = CMD_CHANGEMODE;
		else if (op_quit) ctype = CMD_QUIT;

		if ( ctype == -1) {
			show_help();
		}
		switch (ctype) {
			case CMD_CLEARDEVS:
			case CMD_ENABLE:
			case CMD_DISABLE:
			case CMD_QUIT:
				err = send_command( s, ctype, "", 0 );
				break;
			case CMD_CHANGEMODE:
				err = send_command( s, ctype,
						(optind < argc) ? argv[optind] : "",
						0 );
				break;
			case CMD_ADD:
			case CMD_REMOVE:
				while (optind < argc && err == 0) {
					err = send_command( s, ctype, argv[optind++], passfd );
				}
				break;
			default:
				err = 1;
		}
		if (err != 0) {
			fprintf( stderr, "Error sending command\n");
		}
	}
	close(s);
	return err;
}
