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
	if (*list == NULL) {
		*list = handler;
	} else {
		append_handler( handler, &( (*list)->next ) );
	}
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
	if (list != NULL) {
		if ( type  == list->type &&
		     code  == list->code &&
		     value == list->value ) {
			fprintf(stderr, "Executing handler: %s\n", list->cmdline);
			int pid = fork();
			if (pid == 0 ) {
				system(	list->cmdline );
				exit(0);
			}
		}
		/* traverse down the chain */
		run_handlers(type, code, value, list->next);
	}
}

int count_handlers( eventhandler **list ) {
	if (*list == NULL) {
		return 0;
	} else {
		return 1 + count_handlers( &( (*list)->next ) );
	}
}

