typedef struct device {
	int fd;
	char* devname;
	char* descr;
	struct device *next;
} device;

void add_device(char *dev, int fd);
int remove_device(char *dev);
void clear_devices(void);
int count_devices(void);

void for_each_device( void(*func)(device*) );
