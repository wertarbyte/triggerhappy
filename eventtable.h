#include <linux/input.h>

#define EV_MAP( N ) [ N ] = #N

static const char *EV_NAME[EV_MAX] = {
#include "evtable_EV.h"
};

static const char *KEY_NAME[KEY_MAX] = {
#include "evtable_KEY.h"
};

static const char *SW_NAME[SW_MAX] = {
#include "evtable_SW.h"
};
