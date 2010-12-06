typedef struct device {
	int fd;
	char* devname;
	int exclusive;
	char* descr;
	struct device *next;
} device;

void add_device(char *dev, int fd, int excl);
int remove_device(char *dev);
void clear_devices(void);
int count_devices(void);

void for_each_device( void(*func)(device*) );
