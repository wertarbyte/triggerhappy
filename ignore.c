/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPL. For full license terms, see COPYING.
 */

#include <stdio.h>
#include <stdlib.h>
#include "ignore.h"

void ignore_key( int code, ignore **list ) {
	ignore **p = list;
	while ( *p != NULL ) {
		p = &( (*p)->next );
	}
	*p = malloc( sizeof(ignore) );
	if (*p) {
		(*p)->next = NULL;
		(*p)->code = code;
	} else {
		fprintf(stderr, "Unable to allocate memory for ignored key!\n");
	}
}

void print_ignores( ignore *list ) {
	ignore *p = list;
	while ( p != NULL ) {
		p = p->next;
	}
}

int is_ignored( int code, ignore *list ) {
	ignore *p = list;
	while ( p != NULL ) {
		if (p->code == code) {
			return 1;
		}
		p = p->next;
	}
	return 0;
}
