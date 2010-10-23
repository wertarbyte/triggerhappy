#include <stdio.h>
#include "devices.h"
#include "command.h"

int obey_command( struct command *cmd, device **list ) {
	if (cmd->type == CMD_ADD) {
		add_device( &(cmd->param[0]), list );
		return 0;
	}
	if (cmd->type == CMD_REMOVE) {
		remove_device( &(cmd->param[0]), list );
		return 0;
	}
	if (cmd->type == CMD_QUIT) {
		cleanup();
	}
	return 1;
}
