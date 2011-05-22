/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPLv3 or any later version.
 * For full license terms, see COPYING.
 */

#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "eventnames.h"
#include "keystate.h"
#include "devices.h"
#include "trigger.h"
#include "uinput.h"

#include "triggerparser.h"

static trigger *trigger_list = NULL;

static int triggers_are_enabled = 1;

static char *trigger_mode = NULL;

char *get_trigger_mode() {
	return trigger_mode;
}

void change_trigger_mode(const char *mode) {
	if (trigger_mode != NULL) {
		free(trigger_mode);
		/* suppress the message on the first change to default mode */
		fprintf(stderr, "Changing trigger mode to <%s>\n", mode);
	}
	trigger_mode = strdup(mode);
}

void triggers_enabled( int status ) {
	triggers_are_enabled = status;
}

void append_trigger(trigger *t) {
	trigger **p = &trigger_list;
	while (*p != NULL) {
		p = &( (*p)->next );
	}
	*p = t;
}

static int read_triggerfile(const char *filename) {
	FILE *conf;
	size_t len = 0;
	char *line = NULL;
	ssize_t read;

	if(!filename)
		return 2;

	conf = fopen(filename, "r");
	if (conf == NULL) {
		return 1;
	}
	while ((read = getline(&line, &len, conf)) != -1) {
		char *copy = strdup(line);
		if (!copy) {
			fprintf(stderr, "Error allocating memory for trigger parsing!\n");
			break;
		}
		trigger *t = parse_trigger(copy);
		if (t) {
			append_trigger(t);
		} else if (strlen(copy) > 0) {
			/* perhaps it was just an empty line */
			fprintf(stderr, "Unable to parse trigger line: %s\n", line);
		}
		free(copy);
	}
	fclose(conf);
	conf=NULL;
	free(line);
	line=NULL;
	return 0;
}

static int accept_triggerfile(const struct dirent *entry) {
	const char *suffix = ".conf";
	const char *name = entry->d_name;
	char *end = strstr( name, suffix );
	if ( end && end[ strlen(suffix) ] == '\0' ) {
		return 1;
	} else {
		return 0;
	}
}

int read_triggers(const char *path) {
	/* check whether filename is a directory */
	struct stat sb;
	if (stat(path, &sb) == -1) {
		perror("stat");
		return 1;
	}
	if (S_ISDIR(sb.st_mode)) {
		/* dive into it */
		struct dirent **namelist;
		int n;
		n = scandir(path, &namelist, accept_triggerfile, alphasort);
		if ( n < 0) {
			perror("scandir");
			return 1;
		} else {
			while (n--) {
				struct stat sf;
				char *file = namelist[n]->d_name;
				char *sep = "/";
				char fpath[strlen(path)+strlen(sep)+strlen(file) + 1];
				strcpy(fpath, path);
				strcat(fpath, sep);
				strcat(fpath, file);
				if (stat(fpath, &sf) == -1) {
					perror("stat");
				}
				if (S_ISREG(sf.st_mode)) {
					read_triggerfile(fpath);
				}
				free(namelist[n]);
			}
			free(namelist);
			return 0;
		}
	} else {
		return read_triggerfile( path );
	}
}

static int devtag_equal(const char *d_tag, const char *t_tag) {
	if (strlen(t_tag) == 0) {
		return 1;
	}
	if (strcmp(t_tag, d_tag) == 0) {
		return 1;
	}
	return 0;
}

static int mods_equal(keystate_holder ksh, trigger_modifier tm, int ignore_key) {
	int n = 0;
	while ( n < TRIGGER_MODIFIERS_MAX ) {
		int code = tm[n]; /* this key must be pressed */
		if (code < 0) {
			break;
		} else if (ksh[code] <= 0) {
			return 0;
		}
		n++;
	}
	/* Now n is equal to the number of modifiers needed
	 * we can check whether any additional keys are pressed
	 * by counting the number of pressed keys instead of having
	 * to cross-examine every key against the modifier list
	 */
	int x;
	for (x=0; x<=KEY_MAX; x++) {
		if (x != ignore_key && ksh[x] > 0) {
			n--;
		}
	}
	/* if n is zero, we have the exact number of needed modifiers pressed */
	return (n == 0);
}

static int correct_mode( const char *tmode ) {
	/* not mode specified, valid in every mode */
	return (tmode == NULL) || (strcmp( get_trigger_mode(), tmode ) == 0);
}

void run_triggers(int type, int code, int value, keystate_holder ksh, device *dev) {
	if (triggers_are_enabled == 0) {
		return;
	}
	if (dev && dev->tag) {
	}
	trigger *et = trigger_list;
	while (et != NULL) {
		if ( type  == et->type &&
		     code  == et->code &&
		     value == et->value &&
		     et->action &&
		     correct_mode( et->mode ) &&
		     devtag_equal(&(dev->tag[0]), &(et->devtag[0])) &&
		     mods_equal(ksh, et->modifiers, (type==EV_KEY?code:-1) )) {
			fprintf(stderr, "Executing trigger action: %s\n", et->action);
			/* switch trigger mode or execute program? */
			if (et->action[0] == '@') {
				change_trigger_mode( &(et->action)[1] );
				/* we now stop checking for additional triggers
				 * to avoid changing back to the original mode
				 */
				break;
			} else if (et->action[0] == '<' ) {
				char *keyname = &(et->action[1]);
				fprintf(stderr, "Sending event: %s\n", keyname);
				int type = lookup_event_type( keyname );
				int code = lookup_event_code( keyname );
				if (type && code) {
					send_event( type, code, 1 );
					send_event( type, code, 0 );
					send_event( EV_SYN, 0, 0 );
				}
			} else {
				int pid = fork();
				if (pid == 0 ) {
					/* adjust environment */
					setenv( "TH_DEVICE", dev->devname, 1 );
					setenv( "TH_KEYSTATE", get_keystate(ksh), 1 );
					const char *en = lookup_event_name_i( et->type, et->code );
					setenv( "TH_EVENT", en, 1 );
					char ev[8];
					sprintf( &(ev[0]), "%d", et->value );
					setenv( "TH_VALUE", &(ev[0]), 1 );
					/* default SIGCHLD is not automatically
					   restored and most applications would
					   fail if this signal is ignored */
					signal(SIGCHLD, SIG_DFL);
					system(et->action);
					exit(0);
				} else if (pid < 0) {
					fprintf(stderr, "Unable to fork!\n");
				}
			}
		}
		et = et->next;
	}
}

int count_triggers( trigger **list ) {
	int n = 0;
	if(!list)
		return 0;
	trigger *p = *list;
	while ( p != NULL ) {
		n++;
		p = p->next;
	}
	return n;
}

void clear_triggers() {
	trigger *p = trigger_list;
	while (p != NULL) {
		trigger *next = p->next;
		free(p->action);
		free(p->mode);
		free(p);
		p = next;
	}
	trigger_list = NULL;
}
