#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <stdlib.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <assert.h>

#include "eventnames.h"

pthread_mutex_t keystate_mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned int keystate[KEY_MAX] = { 0 };

void change_keystate( int key, int value ) {
    pthread_mutex_lock( &keystate_mutex );
    switch(value) {
        case 1: // pressed
            keystate[key]++;
            break;
        case 0: // released
            if (keystate[key] > 0) {
                keystate[key]--;
            }
            break;
    }
    pthread_mutex_unlock( &keystate_mutex );
}

void print_keystate() {
    int i;
    int n = 0;
    printf("STATE\t");
    pthread_mutex_lock( &keystate_mutex );
    for (i=0; i<=KEY_MAX; i++) {
        if (keystate[i] > 0) {
            printf("%s%s", (n>0?"+":""), KEY_NAME[i]);
            n++;
        }
    }
    pthread_mutex_unlock( &keystate_mutex );
    printf("\n");
}

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

void* read_events(void *nameptr) {
    char *devname;
    devname = (char *) nameptr;
    int dev;
    dev = open(devname, O_RDONLY);
    if (dev < 0) {
        fprintf(stderr, "Unable to open device file '%s': %s\n", devname, strerror(errno));
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
                change_keystate( ev.code, ev.value );
                print_keystate();
            }
            if ( ev.type == EV_SW ) {
                print_event( ev, SW_NAME, SW_MAX );
            }
        }
        close(dev);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No input device file specified.\n");
        return 1;
    } else {
        pthread_t threads[ argc-1 ];
        int i;
        for (i=0; i<argc-1; i++) {
            char *dev = argv[i+1];
            int rc = pthread_create( &threads[i], NULL, &read_events, (void *)dev );
            assert(rc==0);
        }
        for (i=0; i<argc-1; i++) {
            int rc = pthread_join(threads[i], NULL);
            assert(rc==0);
        }
    }
    return 0;
}
