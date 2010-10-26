#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/hiddev.h>
#include <sys/ioctl.h>

#include "devices.h"

static char *get_device_description(int fd) {
	char descr[256] = "Unknown";
	if(ioctl(fd, EVIOCGNAME(sizeof(descr)), descr) < 0) {
		perror("evdev ioctl");
	}
	return strdup(descr);
}


void add_device(char *dev, device **list) {
	device **p = list;
	// find end of list
	while (*p != NULL) {
		p = &( (*p)->next );
	}
	int fd = open( dev, O_RDONLY );
	if (fd >= 0) {
		*p = malloc(sizeof(**list));
		(*p)->devname = strdup(dev);
		(*p)->descr = get_device_description(fd);
		(*p)->fd = fd;
		(*p)->next = NULL;
	} else {
		fprintf( stderr, "Error opening '%s': %s\n", dev, strerror(errno) );
	}
}

int remove_device(char *dev, device **list) {
	device **p = list;
	while (*p != NULL) {
		if ( strcmp( (*p)->devname, dev ) == 0 ) {
			device *copy = *p;
			/* bypass the list item */
			*p = copy->next;
			close(copy->fd);
			free(copy->devname);
			free(copy->descr);
			free(copy);
			return 1;
		}
		/* advance to the next node */
		p = &( (*p)->next );
	}
	/* reached the end of the list */
	return 0;
}

void clear_devices(device **list) {
	device *p = *list;
	while (p != NULL) {
		device *next = p->next;
		close(p->fd);
		free(p->descr);
		free(p->devname);
		free(p);
		p = next;
	}
	*list = NULL;
}

int count_devices(device **list) {
	int n = 0;
	device **p = list;
	while (*p != NULL) {
		n++;
		p = &( (*p)->next );
	}
	return n;
}
