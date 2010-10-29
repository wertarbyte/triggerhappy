typedef struct device {
	int fd;
	char* devname;
	char* descr;
	struct device *next;
} device;

void add_device(char *dev, int fd, device **list);
int remove_device(char *dev, device **list);
void clear_devices(device **list);
int count_devices(device **list);
