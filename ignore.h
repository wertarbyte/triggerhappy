/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPL. For full license terms, see COPYING.
 */

typedef struct ignore {
	int code;
	struct ignore *next;
} ignore;

void ignore_key( int code, ignore **list );
int is_ignored( int code, ignore *list );
void clear_ignore_list( ignore **list );
