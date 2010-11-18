#include <linux/input.h>
typedef unsigned short keystate_holder[KEY_MAX+1];

void init_keystateholder(keystate_holder*);
void change_keystate(keystate_holder, struct input_event);
char *get_keystate(keystate_holder);
char *get_keystate_ignore_key(keystate_holder, int ignore_key);
