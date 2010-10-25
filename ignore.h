typedef struct ignore {
	int code;
	struct ignore *next;
} ignore;

void ignore_key( int code, ignore **list );
int is_ignored( int code, ignore *list );
void clear_ignore_list( ignore **list );
