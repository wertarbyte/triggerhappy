/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPL. For full license terms, see COPYING.
 */

int bind_cmdsocket(char *name);
struct command *read_command(int cmd_fd);
int send_command(int cmd_fd, enum command_type type, char *param, int passfd, int exclusive, char *tag);
int connect_cmdsocket(char *name);
