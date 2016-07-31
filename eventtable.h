/* Copyright 2010 Stefan Tomanek <stefan.tomanek+th@wertarbyte.de>
 * You have permission to copy, modify, and redistribute under the
 * terms of the GPLv3 or any later version.
 * For full license terms, see COPYING.
 */

#include <linux/input.h>

#define EV_MAP( N ) [ N ] = #N

static const char *EV_NAME[EV_CNT] = {
#include "evtable_EV.inc"
};

static const char *KEY_NAME[KEY_CNT] = {
#include "evtable_KEY.inc"
#include "evtable_BTN.inc"
};

static const char *REL_NAME[REL_CNT] = {
#include "evtable_REL.inc"
};

static const char *ABS_NAME[ABS_CNT] = {
#include "evtable_ABS.inc"
};

static const char *SW_NAME[SW_CNT] = {
#include "evtable_SW.inc"
};
