int bind_cmdsocket(char *name);
struct command *read_command(int cmd_fd);
int send_command(int cmd_fd, enum command_type type, char *param, int passfd, int exclusive, char *tag);
int connect_cmdsocket(char *name);
