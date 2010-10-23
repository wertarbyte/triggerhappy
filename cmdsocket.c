#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <sys/stat.h>

#include "devices.h"
#include "command.h"
#include "cmdsocket.h"

int bind_cmdsocket( char *name ) {
	int cmd_fd;
	struct sockaddr_un addr;
	/* remove any stale files */
	struct stat sb;
	int serr = stat(name, &sb);
	if ( !serr && S_ISSOCK(sb.st_mode)) {
		unlink(name);
	}
	cmd_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	strcpy(addr.sun_path, name);
	addr.sun_family = AF_UNIX;
	bind (cmd_fd, (struct sockaddr *) &addr, 
		strlen(addr.sun_path) + sizeof (addr.sun_family));
	return cmd_fd;
}

int connect_cmdsocket( char *name ) {
	int fd;
	struct sockaddr_un server;
	fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	strcpy(server.sun_path, name);
	server.sun_family = AF_UNIX;
	connect(fd, (struct sockaddr *) &server,
		strlen(server.sun_path) + sizeof (server.sun_family));
	return fd;
}

struct command *read_command( int cmd_fd ) {
	struct command *cmd = malloc(sizeof(struct command));
	int done = read( cmd_fd, cmd, sizeof(struct command) );
	if (done != sizeof(struct command)) {
		fprintf(stderr, "Error reading command\n");
		return NULL;
	}
	return cmd;
}

int send_command( int cmd_fd, enum command_type type, char *param ) {
	struct command cmd;
	cmd.type = type;
	if (param != NULL) {
		strcpy(cmd.param, param);
	} else {
		cmd.param[0] = '\0';
	}
	int done = write( cmd_fd, &cmd, sizeof(struct command) );
	return (done != sizeof(struct command));
}
