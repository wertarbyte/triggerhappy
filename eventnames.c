#include <linux/input.h>
#include "eventnames.h"
#include "eventtable.h"

char* lookup_event_name_i(int evtype, int evcode) {
	if (evtype == EV_KEY) {
		return (KEY_MAX >= evcode ? KEY_NAME[ evcode ] : NULL);
	}
	if (evtype == EV_SW) {
		return (SW_MAX >= evcode ? SW_NAME[ evcode ] : NULL);
	}
	return NULL;
}

char* lookup_event_name(struct input_event ev) {
	return lookup_event_name_i( ev.type, ev.code );
}

char* lookup_type_name_i(int evtype) {
	return (EV_MAX >= evtype ? EV_NAME[ evtype ] : NULL);
}

char* lookup_type_name(struct input_event ev) {
	return lookup_type_name_i( ev.type );
}

int lookup_event_code(char* eventname) {
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

int lookup_event_type( char* eventname ) {
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
