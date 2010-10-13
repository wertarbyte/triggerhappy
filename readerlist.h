struct inputreader {
	pthread_t thread;
	char* devname;
};

struct readerlist {
	struct inputreader reader;
	struct readerlist *next;
};

void add_device(char*, struct readerlist**);
int remove_device(char*, struct readerlist**);
int count_readers(struct readerlist**);
