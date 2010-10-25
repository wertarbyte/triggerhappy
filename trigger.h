#define TRIGGER_MODIFIERS_MAX 5

typedef int trigger_modifier[TRIGGER_MODIFIERS_MAX];

typedef struct trigger {
	int type;
	int code;
	int value;
	trigger_modifier modifiers;
	char *cmdline;
	struct trigger *next;
} trigger;

trigger* parse_trigger(char* line);
void append_trigger(trigger *t);
int read_triggerfile(const char *filename);
void run_triggers(int type, int code, int value, keystate_holder ksh);
void clear_triggers();

extern trigger *TRIGGER_LIST;
