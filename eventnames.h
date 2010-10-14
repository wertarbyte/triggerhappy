#include <linux/input.h>

char* lookup_event_name_i(int evtype, int evcode);
char* lookup_event_name(struct input_event ev);

char* lookup_type_name_i(int evtype);
char* lookup_type_name(struct input_event ev);

int lookup_event_code(char* eventname);
int lookup_event_type(char* eventname);
