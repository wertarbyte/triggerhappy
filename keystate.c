/*
 * keystate.c
 * by Stefan Tomanek <stefan.tomanek@wertarbyte.de>
 */

#include <stdio.h>
#include <linux/input.h>
#include "eventnames.h"

static unsigned int keystate[KEY_MAX] = {};

/*
 * Keep track of a pressed or released key
 */
void change_keystate( struct input_event ev) {
	if (ev.type != EV_KEY)
		return;
	if (ev.code > KEY_MAX)
		return;

	switch(ev.value) {
		case 1: // pressed
			keystate[ev.code]++;
			break;
		case 0: // released
			if (keystate[ev.code] > 0) {
				keystate[ev.code]--;
			}
			break;
	}
}

/*
 * Print the concatenated names of all currently pressed keys
 */
void print_keystate() {
	int i;
	int n = 0;
	printf("STATE\t");
	for (i=0; i<=KEY_MAX; i++) {
		if (keystate[i] > 0) {
			printf("%s%s", (n>0?"+":""), KEY_NAME[i]);
			n++;
		}
	}
	printf("\n");
}
