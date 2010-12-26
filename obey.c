/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPL. For full license terms, see COPYING.
 */

#include <stdio.h>
#include "devices.h"
#include "command.h"
#include "keystate.h"
#include "trigger.h"
#include "thd.h"

int obey_command( struct command *cmd ) {
	if (cmd->type == CMD_ADD) {
		add_device( cmd->param, -1, cmd->exclusive, cmd->tag );
		return 0;
	}
	if (cmd->type == CMD_PASSFD) {
		add_device( cmd->param, cmd->fd, cmd->exclusive, cmd->tag );
		return 0;
	}
	if (cmd->type == CMD_REMOVE) {
		remove_device( cmd->param );
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
		change_trigger_mode( cmd->param );
		return 0;
	}
	return 1;
}
