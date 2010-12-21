#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include "uinput.h"

static int uinput_fd = -1;

int open_uinput(const char *path) {
	close_uinput();
	struct uinput_user_dev device;
	uinput_fd = open(path, O_WRONLY);
	if (!uinput_fd) {
		return -1;
	}
	memset(&device, 0, sizeof device);
	strcpy(device.name,"triggerhappy");
	device.id.bustype = BUS_USB;
	device.id.vendor = 1;
	device.id.product = 1;
	device.id.version = 1;
	if (write(uinput_fd,&device,sizeof(device)) != sizeof(device)) {
		close_uinput();
		return -1;
	}

	if (ioctl(uinput_fd,UI_SET_EVBIT,EV_KEY) < 0) {
		close_uinput();
		return -1;
	}

	/* we can generate _any_ key event */
	int i;
	for (i=0; i<KEY_MAX && uinput_fd; i++) {
		if (ioctl(uinput_fd, UI_SET_KEYBIT, i) < 0) {
			close_uinput();
			return -1;
		}
	}

	if (ioctl(uinput_fd, UI_DEV_CREATE) < 0) {
		close_uinput();
		return -1;
	}

	return uinput_fd;
}

void close_uinput() {
	if (uinput_fd >= 0) {
		close(uinput_fd);
		uinput_fd = -1;
	}
}

int send_event(const int type, const int code, const int value) {
	if (!uinput_fd) {
		return -1;
	}
	struct input_event event;
	memset(&event, 0, sizeof event);
	event.type = type;
	event.code = code;
	event.value = value;
	if (write(uinput_fd, &event, sizeof(event)) != sizeof(event)) {
		fprintf(stderr, "Error on send_event\n");
		return -1;
	}
	return 0;
}

