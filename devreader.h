typedef struct devreader {
	pthread_t thread;
	char* devname;
	struct devreader *next;
} devreader;

void add_device(char *dev, devreader **list);
int remove_device(char *dev, devreader **list);
int count_readers(devreader **list);
