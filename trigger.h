typedef struct trigger {
	int type;
	int code;
	int value;
	char *cmdline;
	struct trigger *next;
} trigger;

trigger* parse_trigger(char* line);
void append_trigger(trigger *t, trigger **list);
int read_triggerfile(const char* filename, trigger **list);
void run_triggers(int type, int code, int value, trigger *list);
