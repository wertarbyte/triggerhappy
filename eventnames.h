#include <linux/input.h>

const char *lookup_event_name_i(const int evtype, const int evcode);
const char *lookup_event_name(const struct input_event ev);

const char *lookup_type_name_i(const int evtype);
const char *lookup_type_name(const struct input_event ev);

int lookup_event_code(const char *eventname);
int lookup_event_type(const char *eventname);
