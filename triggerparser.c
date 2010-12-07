#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "eventnames.h"
#include "keystate.h"
#include "trigger.h"
#include "triggerparser.h"

static int parse_evdef(char *evdef, trigger *t) {
	/* first we split the mode (if present) */
	char *mode = strchr(evdef, '@');
	if (mode) {
		*mode = '\0';
		mode++;
	}
	/* place a copy of the mode string or NULL in the struct */
	t->mode = (mode ? strdup(mode) : NULL);

	/* now we can start to separate the triggering event from the modifiers */
	char *sptr = NULL;
	char *s_trigger  = strtok_r(evdef, "+", &sptr);

	int n = 0;
	while (n < TRIGGER_MODIFIERS_MAX) {
		char *s_mod = strtok_r(NULL, "+", &sptr);
		t->modifiers[n] = s_mod ? lookup_event_code(s_mod) : -1;
		n++;
	}

	int type = lookup_event_type( s_trigger );
	int code = lookup_event_code( s_trigger );

	if (type < 0 || code < 0) {
		return 1;
	}
	t->type = type;
	t->code = code;

	return 0;
}

trigger* parse_trigger(char* line) {
	/* ignore everything behind # */
	char *comment = strchr(line, '#');
	if ( comment != NULL ) {
		*comment = '\0';
	}

	/* no we must have at least 3 tokens left */
	char *delim = " \t\n";
	char *sptr = NULL;
	/* event definition including modifiers and daemon mode */
	char *s_evdef  = strtok_r(line, delim, &sptr);
	/* value of the event (0, 1, 2) */
	char *s_value  = strtok_r(NULL, delim, &sptr);
	/* the action that should take place filling the rest of the line */
	char *s_action = strtok_r(NULL, "", &sptr);

	/* if we do not have 3 tokens, we can abort */
	if (! (s_evdef && s_value && s_action)) {
		return NULL;
	}
	trigger *t = malloc( sizeof(trigger) );
	memset( t, 0, sizeof(t) );

	t->value = atoi(s_value);

	int err = parse_evdef( s_evdef, t );
	if (!err) {
		/* remove trailing whitespaces from the end of the command line */
		char *end = s_action + strlen(s_action) - 1;
		while (end >= s_action && (isspace(*end) || *end == '\n')) {
			end --;
		}
		*(end+1) = '\0';
		/* now copy the strings (mode is already copied parse_evdef */
		t->action = strdup(s_action);
	} else {
		/* free the allocated memory */
		if (t->mode) free(t->mode);
		if (t->action) free(t->action);
		free(t);
		t = NULL;
	}
	return t;
}

