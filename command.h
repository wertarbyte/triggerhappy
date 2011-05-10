/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPLv3 or any later version.
 * For full license terms, see COPYING.
 */

#include "devtag.h"
#define TH_COMMAND_PARAM_LENGTH 256

enum command_type {
	CMD_NOP,
	CMD_ADD,
	CMD_REMOVE,
	CMD_QUIT,
	CMD_DISABLE,
	CMD_ENABLE,
	CMD_PASSFD,
	CMD_CLEARDEVS,
	CMD_CHANGEMODE
};

struct command {
	enum command_type type;
	/* udev pathnames are long, but not that long */
	char param[TH_COMMAND_PARAM_LENGTH];
	int fd;
	int exclusive;
	char tag[TH_DEVICE_TAG_LENGTH];
};
