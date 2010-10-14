#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "eventnames.h"
#include "executer.h"

void launch_script( const char* basedir, struct input_event ev ) {
	// we only handle key and switch events
	if (! (ev.type == EV_KEY || ev.type == EV_SW))
		return;
	// we only launch script on pressing keys
	if (ev.type == EV_KEY && ev.code == 0)
		return;
	char *keyname = lookup_event_name(ev);
	char scriptpath[strlen(keyname)+strlen(basedir)+2];
	scriptpath[0] = '\0';
	strcat(scriptpath, basedir);
	strcat(scriptpath, "/");
	strcat(scriptpath, keyname);
	// check if a script is present
	if (access(scriptpath, X_OK) == 0) {
		int pid = fork();
		if (pid < 0) {
			fprintf(stderr, "Error forking\n");
		} else if (pid > 0) {
			// parent, life goes on...
		} else {
			execl( scriptpath, keyname, keyname, (char*)0 );
			fprintf(stderr, "Unable to execute '%s': %s\n", scriptpath, strerror(errno));
			exit(1);
		}
	}
}
