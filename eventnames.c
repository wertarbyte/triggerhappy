/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPL. For full license terms, see COPYING.
 */

#include <stdlib.h>
#include <linux/input.h>
#include <string.h>
#include "eventnames.h"
#include "eventtable.h"

const char *lookup_event_name_i( const int evtype, const int evcode ) {
	if (evtype == EV_KEY) {
		return (KEY_MAX >= evcode ? KEY_NAME[ evcode ] : NULL);
	}
	if (evtype == EV_SW) {
		return (SW_MAX >= evcode ? SW_NAME[ evcode ] : NULL);
	}
	return NULL;
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

int lookup_event_code( const char *eventname ) {
	int i = 0;
	while (i < KEY_MAX) {
		if (KEY_NAME[i] != NULL &&
		    strcmp(eventname, KEY_NAME[i]) == 0) {
			return i;
		}
		i++;
	}
	i = 0;
	while (i<SW_MAX) {
		if (SW_NAME[i] != NULL &&
		    strcmp(eventname, SW_NAME[i]) == 0) {
			return i;
		}
		i++;
	}
	return -1;
}

int lookup_event_type( const char *eventname ) {
	int i = 0;
	while (i<KEY_MAX) {
		if (KEY_NAME[i] != NULL &&
		    strcmp(eventname, KEY_NAME[i]) == 0) {
			return EV_KEY;
		}
		i++;
	}
	i = 0;
	while (i<SW_MAX) {
		if (SW_NAME[i] != NULL &&
		    strcmp(eventname, SW_NAME[i]) == 0) {
			return EV_SW;
		}
		i++;
	}
	return -1;
}
