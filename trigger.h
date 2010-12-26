/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPL. For full license terms, see COPYING.
 */

#include "devtag.h"
#define TRIGGER_MODIFIERS_MAX 5

typedef int trigger_modifier[TRIGGER_MODIFIERS_MAX];

typedef struct trigger {
	int type;
	int code;
	int value;
	trigger_modifier modifiers;
	char *mode;
	char devtag[TH_DEVICE_TAG_LENGTH];
	char *action;
	struct trigger *next;
} trigger;

trigger* parse_trigger(char* line);
void append_trigger(trigger *t);
int read_triggers(const char *filename);
void run_triggers(int type, int code, int value, keystate_holder ksh, device *dev);
void clear_triggers();

void triggers_enabled( int status );

void change_trigger_mode( const char *tmode );
