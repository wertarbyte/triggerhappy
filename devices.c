#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "devices.h"

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
	device **p = list;
	while (*p != NULL) {
		device *copy = *p;
		*p = copy->next;
		close(copy->fd);
		fprintf(stderr, "Removing %s\n", copy->devname);
		free(copy->devname);
		free(copy);
		p = &( (*p)->next );
	}
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
