/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPLv3 or any later version.
 * For full license terms, see COPYING.
 */

#include <linux/input.h>
typedef unsigned short keystate_holder[KEY_MAX+1];

void init_keystate_holder(keystate_holder**);
void change_keystate(keystate_holder, struct input_event);
char *get_keystate(keystate_holder);
char *get_keystate_ignore_key(keystate_holder, int ignore_key);
