#include <stdio.h>
#include "devices.h"
#include "command.h"
#include "keystate.h"
#include "trigger.h"

int obey_command( struct command *cmd, device **list ) {
	if (cmd->type == CMD_ADD) {
		add_device( &(cmd->param[0]), -1, list );
		return 0;
	}
	if (cmd->type == CMD_PASSFD) {
		add_device( &(cmd->param[0]), cmd->fd, list );
		return 0;
	}
	if (cmd->type == CMD_REMOVE) {
		remove_device( &(cmd->param[0]), list );
		return 0;
	}
	if (cmd->type == CMD_QUIT) {
		cleanup();
		return 0;
	}
	if (cmd->type == CMD_ENABLE) {
		triggers_enabled(1);
		return 0;
	}
	if (cmd->type == CMD_DISABLE) {
		triggers_enabled(0);
		return 0;
	}
	return 1;
}
