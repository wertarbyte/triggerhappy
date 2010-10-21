#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>

#include "devices.h"
#include "command.h"
#include "cmdsocket.h"

int main(int argc, char *argv[]) {
	if (argc < 4) {
		fprintf( stderr, "Use:\n  th-cmd <socket> add|remove <device>\n");
		return 1;
	}
	int s = connect_cmdsocket( argv[1] );
	if (s < 0) {
		perror("connect()");
		return 1;
	} else {
		if ( strcasecmp( "add", argv[2] ) == 0 ) {
			send_command( s, CMD_ADD, argv[3] );
		} else if ( strcasecmp( "remove", argv[2] ) == 0 ) {
			send_command( s, CMD_REMOVE, argv[3] );
		} else {
			fprintf( stderr, "Unknown command: %s\n", argv[2]);
		}
		close(s);
	}
}
