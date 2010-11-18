PREFIX:=/usr/
DESTDIR:=/
BINDIR:=$(DESTDIR)/$(PREFIX)/sbin/
MANDIR:=$(DESTDIR)/$(PREFIX)/share/man/man1/

THD_COMPS := thd keystate trigger eventnames devices cmdsocket obey ignore
THCMD_COMPS := th-cmd cmdsocket

MAKEDEPEND = $(CC) -M -MG $(CPPFLAGS) -o $*.d $<

LINUX_INPUT_H := $(shell echo '\#include <linux/input.h>' | $(CC) $(CPPFLAGS) -M -E - | awk 'NR==1 {print $$2}')

all: thd th-cmd man

man: thd.1 th-cmd.1

thd: $(THD_COMPS:%=%.o)

th-cmd: $(THCMD_COMPS:%=%.o)

%.1: %.pod
	pod2man \
		--center="Triggerhappy daemon" \
		--section=1 \
		--release="" \
		$< > $@

evtable_%.h: $(LINUX_INPUT_H)
	awk '/^#define $*_/ && $$2 !~ /_(MAX|CNT|VERSION)$$/ {print "EV_MAP("$$2"),"}' $< > $@

clean:
	rm -f *.d
	rm -f *.o
	rm -f evtable_*.h
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
