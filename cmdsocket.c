#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

	int fd[1] = {-1};
	char buffer[CMSG_SPACE(sizeof fd)];

	struct iovec v = {
		.iov_base = cmd,
		.iov_len  = sizeof(*cmd)
	};
	struct msghdr msg = {
		.msg_iov        = &v,
		.msg_iovlen     = 1,
		.msg_control    = buffer,
		.msg_controllen = sizeof(buffer)
	};

	int done = recvmsg( cmd_fd, &msg, 0 );

	if (done == -1) {
		fprintf(stderr, "Error reading command.");
		free(cmd);
		return NULL;
	}
	struct cmsghdr *cmessage = CMSG_FIRSTHDR(&msg);
	if (cmessage) {
		memcpy(fd, CMSG_DATA(cmessage), sizeof fd);
		/* place FD back in the command message */
		cmd->fd = (int) fd[0];
	}
	return cmd;
}

int send_command( int cmd_fd, enum command_type type, char *param, int passfd ) {
	if (type == CMD_ADD && passfd == 1) {
		type = CMD_PASSFD;
	}
	struct command cmd = {
		.fd    = -1,
		.type  = type,
		.param = {0}
	};
	if (param != NULL) {
		strcpy(cmd.param, param);
	}

	struct iovec v = {
		.iov_base = &cmd,
		.iov_len = sizeof(cmd)
	};
	struct msghdr m = {
		.msg_iov    = &v,
		.msg_iovlen = 1
	};

	/* add FD */
	if (passfd) {
		int fd = open( param, O_RDONLY );
		if (fd < 0) {
			perror("open");
			return 1;
		}
		int dev_fd[1] = { fd };
		char buffer[CMSG_SPACE(sizeof(dev_fd))];
		m.msg_control = buffer;
		m.msg_controllen = sizeof(buffer);

		struct cmsghdr *cmessage = CMSG_FIRSTHDR(&m);
		cmessage->cmsg_level = SOL_SOCKET;
		cmessage->cmsg_type = SCM_RIGHTS;
		cmessage->cmsg_len = CMSG_LEN(sizeof(dev_fd));

		m.msg_controllen = cmessage->cmsg_len;
		memcpy(CMSG_DATA(cmessage), dev_fd, sizeof dev_fd);
	}
	int done = sendmsg( cmd_fd, &m, 0 );
	return (done == -1);
}
