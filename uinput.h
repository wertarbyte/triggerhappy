/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPL. For full license terms, see COPYING.
 */

int open_uinput(const char *path);
void close_uinput();
int send_event(const int type, const int code, const int value);
