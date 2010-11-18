#include <stdio.h>
#include <linux/input.h>
#include <stdlib.h>
#include <string.h>
#include "eventnames.h"
#include "keystate.h"

void init_keystate_holder(keystate_holder **ksh) {
	if (*ksh == NULL) {
		*ksh = malloc(sizeof(keystate_holder));
	}
	int i;
	for (i=0; i<=KEY_MAX; i++) {
		(**ksh)[i] = 0;
	}
}

/*
 * Keep track of a pressed or released key
 */
void change_keystate( keystate_holder ksh, struct input_event ev ) {
	if (ev.type != EV_KEY)
		return;
	if (ev.code > KEY_MAX)
		return;
	switch(ev.value) {
		case 1: // pressed
			ksh[ev.code]++;
			break;
		case 0: // released
			if (ksh[ev.code] > 0) {
				ksh[ev.code]--;
			}
			break;
	}
}

char *get_keystate(keystate_holder ksh) {
	return get_keystate_ignore_key( ksh, -1 );
}

char *get_keystate_ignore_key(keystate_holder ksh, int ignore_key) {
	const int bsize = 1024;
	char *KS = "+";

	char buf[bsize];
	buf[0] = '\0';

	int i;
	int n = 0;
	for (i=0; i<=KEY_MAX; i++) {
		if (i != ignore_key && ksh[i] > 0) {
			if (n>0) {
				strncat( buf, KS, bsize-1-strlen(KS) );
			}
			const char *name = lookup_event_name_i(EV_KEY, i);
			strncat( &(buf[0]), name, bsize-1-strlen(buf) );
			n++;
		}
	}
	char *res = malloc( sizeof(char) * (1+strlen(buf)) );
	strcpy( res, buf );
	return res;
}
