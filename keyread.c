/*
 * keyread.c
 * by Stefan Tomanek <stefan.tomanek@wertarbyte.de>
 */

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
#include <assert.h>

#include "eventnames.h"

#ifndef NOTHREADS

#include <pthread.h>

pthread_mutex_t keystate_mutex = PTHREAD_MUTEX_INITIALIZER;

#define LOCK(mutex) pthread_mutex_lock(mutex)
#define UNLOCK(mutex) pthread_mutex_unlock(mutex)
#else
#define LOCK(mutex)
#define UNLOCK(mutex)
#endif

unsigned int keystate[KEY_MAX] = { 0 };

/*
 * Keep track of a pressed or released key
 */
void change_keystate( int key, int value ) {
    LOCK( &keystate_mutex );
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
    UNLOCK( &keystate_mutex );
}

/*
 * Print the concatenated names of all currently pressed keys
 */
void print_keystate() {
    int i;
    int n = 0;
    printf("STATE\t");
    LOCK( &keystate_mutex );
    for (i=0; i<=KEY_MAX; i++) {
        if (keystate[i] > 0) {
            printf("%s%s", (n>0?"+":""), KEY_NAME[i]);
            n++;
        }
    }
    UNLOCK( &keystate_mutex );
    printf("\n");
}

/*
 * Look up event and key names and print them to STDOUT
 */
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

/*
 * Read events from device file, decode them and print them to STDOUT as well
 * as keep track of key status
 */
int read_events(char *devname) {
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
                return 1;
            }
            // key event
            if ( ev.type == EV_KEY) {
                print_event( ev, KEY_NAME, KEY_MAX );
                change_keystate( ev.code, ev.value );
                // print name of all currently held keys
                print_keystate();
            }
            // switch event
            if ( ev.type == EV_SW ) {
                print_event( ev, SW_NAME, SW_MAX );
            }
        }
        close(dev);
    }
    return 0;
}

#ifndef NOTHREADS
void* reader_thread(void* ptr) {
    char *devname;
    devname = (char *) ptr;
    read_events( devname );
}
#endif

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No input device file specified.\n");
        return 1;
    } else {
#ifndef NOTHREADS
        // create one thread for every device file supplied
        pthread_t threads[ argc-1 ];
        int i;
        for (i=0; i<argc-1; i++) {
            char *dev = argv[i+1];
            int rc = pthread_create( &threads[i], NULL, &reader_thread, (void *)dev );
            assert(rc==0);
        }
        for (i=0; i<argc-1; i++) {
            int rc = pthread_join(threads[i], NULL);
            assert(rc==0);
        }
#else
        // without threading, we only handle the first device file named
        read_events( argv[1] );
#endif
    }
    return 0;
}
