#include <stdio.h>
#include <stdlib.h>
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
	fprintf( stderr, "  th-cmd --socket <socket> [--passfd] --udev\n");
	fprintf( stderr, "  th-cmd --socket <socket> [--passfd] --add <device>\n");
	fprintf( stderr, "  th-cmd --socket <socket> --remove <device>\n");
	fprintf( stderr, "  th-cmd --socket <socket> --enable\n");
	fprintf( stderr, "  th-cmd --socket <socket> --disable\n");
	fprintf( stderr, "  th-cmd --socket <socket> --quit\n");
	fprintf( stderr, "  th-cmd --socket <socket> --help\n");
}

int main(int argc, char *argv[]) {
	char *socket = NULL;
	char *dev = NULL;
	static int passfd = 0;
	enum command_type ctype = -1;

	static struct option long_options[] = {
		{ "socket", 1, 0, 's' },
		{ "add", 1, 0, 'a' },
		{ "remove", 1, 0, 'r' },
		{ "udev", 0, 0, 'u' },
		{ "passfd", 0, &passfd, 1 },
		{ "enable", 0, 0, 'e' },
		{ "disable", 0, 0, 'd' },
		{ "quit", 0, 0, 'q' },
		{ "help", 0, 0, 'h' },
		{ 0, 0, 0, 0 }
	};

	int c;
	while (1) {
		int option_index = 0;
		c = getopt_long(argc, argv, "s:a:r:u", long_options, &option_index);
		if (c == -1) {
			break;
		}
		switch(c) {
			case 's':
				socket = optarg;
				break;
			case 'a':
				ctype = CMD_ADD;
				dev = optarg;
				break;
			case 'r':
				ctype = CMD_REMOVE;
				dev = optarg;
				break;
			case 'u':
				if (strcasecmp("add", getenv("ACTION")) == 0) {
					ctype = CMD_ADD;
				} else if (strcasecmp("remove", getenv("ACTION")) == 0) {
					ctype = CMD_REMOVE;
				}
				dev = getenv("DEVNAME");
				break;
			case 'e':
				ctype = CMD_ENABLE;
				break;
			case 'd':
				ctype = CMD_DISABLE;
				break;
			case 'q':
				ctype = CMD_QUIT;
				break;
			case 'h':
			case '?':
			case -1:
				show_help();
				return 1;
		}
	}
	if (ctype == -1) {
		show_help();
		return 1;
	}

	int s = connect_cmdsocket( socket );
	if (s < 0) {
		perror("connect()");
		return 1;
	}
	int err;
	err = send_command( s, ctype, dev, passfd );
	if (err) {
		fprintf( stderr, "Error sending command\n");
	}
	close(s);

}
