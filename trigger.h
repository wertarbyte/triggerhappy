#define TRIGGER_MODIFIERS_MAX 5

typedef int trigger_modifier[TRIGGER_MODIFIERS_MAX];

typedef struct trigger {
	int type;
	int code;
	int value;
	trigger_modifier modifiers;
	char *mode;
	char *cmdline;
	struct trigger *next;
} trigger;

trigger* parse_trigger(char* line);
void append_trigger(trigger *t);
int read_triggers(const char *filename);
void run_triggers(int type, int code, int value, keystate_holder ksh);
void clear_triggers();

void triggers_enabled( int status );

void change_trigger_mode( const char *tmode );
