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
#include <inttypes.h>

#include "devices.h"
#include "eventnames.h"

#define BITFIELD uint32_t

static char *get_device_description(int fd) {
	char descr[256] = "Unknown";
	if(ioctl(fd, EVIOCGNAME(sizeof(descr)), descr) < 0) {
		perror("evdev ioctl");
	}
	return strdup(descr);
}

static __inline__ int test_bit(int nr, BITFIELD * addr) {
	BITFIELD mask;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	return ((mask & *addr) != 0);
}

int device_is_suitable(int fd) {
	BITFIELD bits[32];
	int rc = ioctl(fd, EVIOCGBIT(0,sizeof(bits)), bits);
	return rc > 0 && (
		/* we only consider devices with keys or switches suitable */
		test_bit(EV_KEY, bits) || test_bit(EV_SW, bits)
	);
}

void add_device(char *dev, int fd, device **list) {
	device **p = list;
	// find end of list
	while (*p != NULL) {
		p = &( (*p)->next );
	}
	if (fd < 0) {
		fd = open( dev, O_RDONLY );
	}
	if (fd >= 0) {
		if (! device_is_suitable(fd) ) {
			fprintf(stderr, "Device %s not suitable.\n", dev);
			close(fd);
			return;
		}
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
