int open_uinput(const char *path);
void close_uinput();
int send_event(const int type, const int code, const int value);
