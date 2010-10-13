/*
 * keystate.c
 * by Stefan Tomanek <stefan.tomanek@wertarbyte.de>
 */

#include <stdio.h>
#include <linux/input.h>
#include <stdlib.h>
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

/*
 * Print the concatenated names of all currently pressed keys
 */
void print_keystate(keystate_holder ksh) {
	int i;
	int n = 0;
	printf("STATE\t");
	for (i=0; i<=KEY_MAX; i++) {
		if (ksh[i] > 0) {
			printf("%s%s", (n>0?"+":""), KEY_NAME[i]);
			n++;
		}
	}
	printf("\n");
}
