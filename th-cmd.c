#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>

#include "devices.h"
#include "command.h"
#include "cmdsocket.h"

int main(int argc, char *argv[]) {
	char *op = NULL;
	char *dev = NULL;
	if (argc == 3 && (strcasecmp( "udev", argv[2] ) == 0) ) {
		/* gather params from udev environment */
		op = getenv("ACTION");
		dev = getenv("DEVNAME");
		if (op == NULL || dev == NULL) {
			return 1;
		}
	} else if (argc == 4) {
		op = argv[2];
		dev = argv[4];
	} else {
		fprintf( stderr, "Use:\n");
		fprintf( stderr, "  th-cmd <socket> udev\n");
		fprintf( stderr, "  th-cmd <socket> add <device>\n");
		fprintf( stderr, "  th-cmd <socket> remove <device>\n");
		return 1;
	}

	int s = connect_cmdsocket( argv[1] );
	if (s < 0) {
		perror("connect()");
		return 1;
	}
	int err;
	if ( strcasecmp( "add", op ) == 0 ) {
		err = send_command( s, CMD_ADD, dev );
	} else if ( strcasecmp( "remove", op ) == 0 ) {
		err = send_command( s, CMD_REMOVE, dev );
	} else {
		fprintf( stderr, "Unknown command: %s\n", op);
	}
	if (err) {
		//fprintf( stderr, "Error sending command\n");
		perror("send_command()");
	}
	close(s);

}
