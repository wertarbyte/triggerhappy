#include <linux/input.h>
typedef unsigned short keystate_holder[KEY_MAX+1];

void init_keystateholder(keystate_holder*);
void change_keystate(keystate_holder, struct input_event);
char *get_keystate(keystate_holder);
void print_keystate(keystate_holder);
