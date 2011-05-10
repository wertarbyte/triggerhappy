/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPLv3 or any later version.
 * For full license terms, see COPYING.
 */

#include "devtag.h"

typedef struct device {
	int fd;
	char *devname;
	int exclusive;
	char tag[TH_DEVICE_TAG_LENGTH];
	struct device *next;
} device;

void add_device(char *dev, int fd, int excl, char *tag);
int remove_device(char *dev);
void clear_devices(void);
int count_devices(void);

void for_each_device( void(*func)(device*) );

char *get_device_description(int fd);
