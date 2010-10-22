typedef struct trigger {
	int type;
	int code;
	int value;
	char *cmdline;
	struct trigger *next;
} trigger;

trigger* parse_trigger(char* line);
void append_trigger(trigger *t);
int read_triggerfile(const char *filename);
void run_triggers(int type, int code, int value, keystate_holder ksh);

extern trigger *TRIGGER_LIST;
