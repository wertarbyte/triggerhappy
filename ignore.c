#include <stdio.h>
#include <stdlib.h>
#include "ignore.h"

void ignore_key( int code, ignore **list ) {
	ignore **p = list;
	while ( *p != NULL ) {
		p = &( (*p)->next );
	}
	*p = malloc( sizeof(ignore) );
	(*p)->next = NULL;
	(*p)->code = code;
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
