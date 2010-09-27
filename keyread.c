#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <linux/input.h>

#include "eventnames.h"

void print_key_event(struct input_event ev) {
    char *typename = EV_NAME[ ev.type ];
    char *keyname = KEY_NAME[ ev.code ];
    printf( "%s\t%s\t%d\n", typename, keyname, ev.value );
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
        fprintf(stderr, "Unable to open device file\n");
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
                print_key_event( ev );
            }
        }
        close(dev);
        return 0;
    }
}
