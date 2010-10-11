typedef struct eventhandler {
	int type;
	int code;
	int value;
	char *cmdline;
	struct eventhandler *next;
} eventhandler;

eventhandler* parse_handler( char* );
void append_handler(eventhandler*, eventhandler**);
int read_eventhandlers(const char*, eventhandler**);
void run_handlers(int type, int code, int value, eventhandler*);
