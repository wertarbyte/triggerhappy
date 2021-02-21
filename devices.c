/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPLv3 or any later version.
 * For full license terms, see COPYING.
 */

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

static device *device_list = NULL;

char *get_device_description(int fd) {
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
		test_bit(EV_KEY, bits) || test_bit(EV_SW, bits) ||
		test_bit(EV_REL, bits) || test_bit(EV_ABS, bits)
	);
}

void add_device(char *dev, int fd, int excl, char *tag) {
	device **p = &device_list;
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
		/* grab device exclusively */
		if (excl) {
			if (ioctl(fd, EVIOCGRAB, 1) == -1) {
				fprintf(stderr, "Device %s cannot be grabbed exclusively.\n", dev);
				close(fd);
				return;
			}
		}
		*p = malloc(sizeof(device));
		if (*p == NULL) goto nomem_cleanup;
		memset(*p, 0, sizeof(device));
		(*p)->devname = strdup(dev);
		if ((*p)->devname == NULL) goto nomem_cleanup;
		(*p)->fd = fd;
		(*p)->exclusive = excl;
		if (tag) {
			strncpy((*p)->tag, tag, TH_DEVICE_TAG_LENGTH);
			(*p)->tag[TH_DEVICE_TAG_LENGTH-1] = '\0';
		}
		(*p)->next = NULL;
	} else {
		fprintf( stderr, "Error opening '%s': %s\n", dev, strerror(errno) );
	}
	return;
nomem_cleanup:
	fprintf(stderr, "Unable to allocate memory for new device!\n");
	if (*p) {
		free( (*p)->devname );
	}
	free(*p);
	*p = NULL;
}

int remove_device(char *dev) {
	device **p = &device_list;
	while (*p != NULL) {
		if ( strcmp( (*p)->devname, dev ) == 0 ) {
			device *copy = *p;
			/* bypass the list item */
			*p = copy->next;
			close(copy->fd);
			free(copy->devname);
			free(copy);
			return 1;
		}
		/* advance to the next node */
		p = &( (*p)->next );
	}
	/* reached the end of the list */
	return 0;
}

void clear_devices(void) {
	device *p = device_list;
	while (p != NULL) {
		device *next = p->next;
		close(p->fd);
		free(p->devname);
		free(p);
		p = next;
	}
	device_list = NULL;
}

int count_devices(void) {
	int n = 0;
	device *p = device_list;
	while (p != NULL) {
		n++;
		p = p->next;
	}
	return n;
}

void for_each_device( void(*func)(device*) ) {
	device *p = device_list;
	while (p != NULL) {
		func( p );
		p = p->next;
	}
}
