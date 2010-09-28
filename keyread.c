#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <linux/input.h>

#include "eventnames.h"

void print_event(struct input_event ev, char *evnames[], int maxcode) {
    char *typename = EV_NAME[ ev.type ];
    char *evname = (maxcode >= ev.code ? evnames[ ev.code ] : NULL);
    if ( evname != NULL ) {
        printf( "%s\t%s\t%d\n", typename, evname, ev.value );
    } else {
        fprintf( stderr, "Unknown %s event id: %d (value %d)\n", typename, ev.code, ev.value );
    }
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No input device file specified.\n");
        return 1;
    }
    char *devname = argv[1];
    int dev;
    dev = open(devname, O_RDONLY);
    if (dev < 0) {
        fprintf(stderr, "Unable to open device file '%s': %s\n", devname, strerror(errno));
        return 1;
    } else {
        struct input_event ev;
        while(1) {
            int n = read( dev, &ev, sizeof(ev) );
            if ( n != sizeof(ev) ) {
                fprintf(stderr, "Read error\n");
                break;
            }
            // key pressed
            if ( ev.type == EV_KEY) {
                print_event( ev, KEY_NAME, KEY_MAX );
            }
            if ( ev.type == EV_SW ) {
                print_event( ev, SW_NAME, SW_MAX );
            }
        }
        close(dev);
        return 0;
    }
}
