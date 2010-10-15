#include "conf.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "eventnames.h"

static int strint(const char *str) {
	if (str == NULL) {
		return -1;
	} else {
		return atoi(str);
	}
}

/* copy token */
static char* ct(char *token) {
	if (token == NULL) {
		return NULL;
	} else {
		return strdup(token);
	}
}

eventhandler* parse_eventhandler(char* line) {
	char *token = NULL;
	char *sptr = NULL;
	char *cp = strdup(line);

	/* ignore everything behind # */
	char *comment = strchr(line, '#');
	if ( comment != NULL ) {
		*comment = '\0';
	}

	char *delim = " \t\n";

	char *evname = ct( strtok_r(cp, delim, &sptr) );
	int value = strint( strtok_r(NULL, delim, &sptr) );
	char *cmd = ct( strtok_r(NULL, "\n", &sptr) );
	free(cp);

	/* all fields filled? */
	if (evname && cmd && (value >= 0)) {
		eventhandler *eh = malloc( sizeof(eventhandler) );
		eh->type = lookup_event_type( evname );
		eh->code = lookup_event_code( evname );
		eh->value = value;
		eh->cmdline = cmd;
		eh->next = NULL;
		return eh;
	} else {
		free(evname);
		free(cmd);
		return NULL;
	}
}

void append_handler(eventhandler *handler, eventhandler **list) {
	eventhandler **p = list;
	while (*p != NULL) {
		p = &( (*p)->next );
	}
	*p = handler;
}

int read_eventhandlers(const char *filename, eventhandler **list) {
        FILE *conf;
        int len = 0;
        char *line = NULL;
        ssize_t read;
        conf = fopen(filename, "r");
        if (conf == NULL) {
		return 1;
        }
        while ((read = getline(&line, &len, conf)) != -1) {
		eventhandler *h = parse_eventhandler( line );
		if (h) {
			append_handler( h, list );
		} else {
			fprintf(stderr, "Unable to parse config line: %s\n", line);
		}
	}
	fclose(conf);
	free(line);
	return 0;
}

void run_handlers(int type, int code, int value, eventhandler *list) {
	eventhandler *eh = list;
	while (eh != NULL) {
		if ( type  == eh->type &&
		     code  == eh->code &&
		     value == eh->value ) {
			fprintf(stderr, "Executing handler: %s\n", eh->cmdline);
			int pid = fork();
			if (pid == 0 ) {
				system(	eh->cmdline );
				exit(0);
			}
		}
		eh = eh->next;
	}
}

int count_handlers( eventhandler **list ) {
	int n = 0;
	eventhandler *p = *list;
	while ( p != NULL ) {
		n++;
		p = p->next;
	}
	return n;
}

