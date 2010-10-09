#include <stdio.h>
#include <linux/input.h>
#include <string.h>
#include "eventnames.h"
#include "executer.h"

void launch_script( const char* basedir, struct input_event ev ) {
	// we only handle key nd switch events
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
	printf("Launching script '%s'\n", scriptpath);
}
