PREFIX:=/usr/
DESTDIR:=/
BINDIR:=$(DESTDIR)/$(PREFIX)/sbin/
MANDIR:=$(DESTDIR)/$(PREFIX)/share/man/man1/

VERSION:=$(shell cat version.inc)

THD_COMPS := thd keystate trigger eventnames devices cmdsocket obey ignore uinput triggerparser
THCMD_COMPS := th-cmd cmdsocket

MAKEDEPEND = $(CC) -M -MG $(CPPFLAGS) -o $*.d $<

LINUX_INPUT_H := $(shell echo '\#include <linux/input.h>' | $(CC) $(CPPFLAGS) -M -E - | grep -o '[^ ]*linux/input.h')

all: thd th-cmd man

man: thd.1 th-cmd.1

thd: $(THD_COMPS:%=%.o)

th-cmd: $(THCMD_COMPS:%=%.o)

%.1: %.pod
	pod2man \
		--center="Triggerhappy daemon" \
		--section=1 \
		--release="$(VERSION)" \
		$< > $@

evtable_%.h: $(LINUX_INPUT_H)
	awk '/^#define $*_/ && $$2 !~ /_(MAX|CNT|VERSION)$$/ {print "EV_MAP("$$2"),"}' $< > $@

version.h: version.inc
	sed -r 's!(.*)!#define TH_VERSION "\1"!' $< > $@

clean:
	rm -f *.d
	rm -f *.o
	rm -f evtable_*.h
	rm -f version.h
	rm -f thd th-cmd
	rm -f thd.1 th-cmd.1

install: all
	install -D thd $(BINDIR)/thd
	install -D th-cmd $(BINDIR)/th-cmd
	install -D thd.1 $(MANDIR)/thd.1
	install -D th-cmd.1 $(MANDIR)/th-cmd.1

%.d : %.c
	$(MAKEDEPEND)

-include $(THD_COMPS:%=%.d) $(THCMD_COMPS:%=%.d)
