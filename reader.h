struct inputreader {
	pthread_t thread;
	char* devname;
};

struct readerlist {
	struct inputreader reader;
	struct readerlist *next;
};

void spawn_reader(char*, struct readerlist**);
void join_readers(struct readerlist**);
