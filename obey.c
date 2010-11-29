#include <stdio.h>
#include "devices.h"
#include "command.h"
#include "keystate.h"
#include "trigger.h"
#include "thd.h"

int obey_command( struct command *cmd ) {
	if (cmd->type == CMD_ADD) {
		add_device( &(cmd->param[0]), -1 );
		return 0;
	}
	if (cmd->type == CMD_PASSFD) {
		add_device( &(cmd->param[0]), cmd->fd );
		return 0;
	}
	if (cmd->type == CMD_REMOVE) {
		remove_device( &(cmd->param[0]) );
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
	if (cmd->type == CMD_CLEARDEVS) {
		clear_devices();
		return 0;
	}
	if (cmd->type == CMD_CHANGEMODE) {
		change_trigger_mode( &(cmd->param[0] ) );
		return 0;
	}
	return 1;
}
