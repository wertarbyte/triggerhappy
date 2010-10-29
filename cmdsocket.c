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
	struct msghdr msg = {0};
	struct command *cmd = malloc(sizeof(struct command));
	struct iovec v;
	v.iov_base = cmd;
	v.iov_len = sizeof(*cmd);
	msg.msg_iov = &v;
	msg.msg_iovlen = 1;

	int done = recvmsg( cmd_fd, &msg, 0 );
	if (done == -1) {
		fprintf(stderr, "Error reading command.");
		free(cmd);
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

	struct msghdr m = {0};
	struct iovec v;
	memset(&m, 0, sizeof(m));
	v.iov_base = &cmd;
	v.iov_len = sizeof(cmd);
	m.msg_iov = &v;
	m.msg_iovlen = 1;

	int done = sendmsg( cmd_fd, &m, 0 );
	return (done != -1);
}
