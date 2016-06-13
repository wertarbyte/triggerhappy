/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPLv3 or any later version.
 * For full license terms, see COPYING.
 */

#include <stdlib.h>
#include <linux/input.h>
#include <string.h>
#include "eventnames.h"
#include "eventtable.h"

const char *lookup_event_name_i( const int evtype, const int evcode ) {
	switch(evtype) {
		case EV_KEY:
			return (KEY_MAX >= evcode ? KEY_NAME[ evcode ] : NULL);
		case EV_SW:
			return (SW_MAX >= evcode ? SW_NAME[ evcode ] : NULL);
		case EV_REL:
			return (REL_MAX >= evcode ? REL_NAME[ evcode ] : NULL);
		default:
			return NULL;
	}
}

const char *lookup_event_name( const struct input_event ev ) {
	return lookup_event_name_i( ev.type, ev.code );
}

const char *lookup_type_name_i( const int evtype ) {
	return (EV_MAX >= evtype ? EV_NAME[ evtype ] : NULL);
}

const char *lookup_type_name( const struct input_event ev ) {
	return lookup_type_name_i( ev.type );
}

static int lookup_index(const char **map, const char *value, int max) {
	int i = 0;
	while (i < max) {
		if (map[i] != NULL &&
		    strcmp(value, map[i])  == 0) {
			return i;
		}
		i++;
	}
	return -1;
}

int lookup_event_code( const char *eventname ) {
	int res = -1;
	if (res == -1) res = lookup_index(KEY_NAME, eventname, KEY_MAX);
	if (res == -1) res = lookup_index(SW_NAME, eventname, SW_MAX);
	if (res == -1) res = lookup_index(REL_NAME, eventname, REL_MAX);
	return res;
}

int lookup_event_type( const char *eventname ) {
	if (lookup_index(KEY_NAME, eventname, KEY_MAX) != -1) {
		return EV_KEY;
	}
	if (lookup_index(SW_NAME, eventname, SW_MAX) != -1) {
		return EV_SW;
	}
	if (lookup_index(REL_NAME, eventname, REL_MAX) != -1) {
		return EV_REL;
	}
	return -1;
}
