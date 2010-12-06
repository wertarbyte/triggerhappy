enum command_type {
	CMD_ADD,
	CMD_REMOVE,
	CMD_QUIT,
	CMD_DISABLE,
	CMD_ENABLE,
	CMD_PASSFD,
	CMD_CLEARDEVS,
	CMD_CHANGEMODE
};

struct command {
	enum command_type type;
	/* udev pathnames are long, but not that long */
	char param[256];
	int fd;
	int exclusive;
};
