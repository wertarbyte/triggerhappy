typedef struct inputreader {
	pthread_t thread;
	char* devname;
} inputreader;

typedef struct readerlist {
	struct inputreader reader;
	struct readerlist *next;
} readerlist;

void add_device(char*, readerlist**);
int remove_device(char*, readerlist**);
int count_readers(readerlist**);
