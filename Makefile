PREFIX:=/usr/
DESTDIR:=/
BINDIR:=$(DESTDIR)/$(PREFIX)/sbin/
MANDIR:=$(DESTDIR)/$(PREFIX)/share/man/man1/

PKGCONFIG = pkg-config
HAVE_PKGCONFIG = $(shell $(PKGCONFIG) --version 2>/dev/null || echo no)
ifneq ($(HAVE_PKGCONFIG),no)
HAVE_SYSTEMD = $(shell $(PKGCONFIG) --exists libsystemd && echo 1 || echo 0)
ifeq ($(HAVE_SYSTEMD),1)
CPPFLAGS += -DHAVE_SYSTEMD=1
CFLAGS += $(shell $(PKGCONFIG) --cflags libsystemd)
LDLIBS += $(shell $(PKGCONFIG) --libs libsystemd)
endif
endif

VERSION:=$(shell cat version.inc)

THD_COMPS := thd keystate trigger eventnames devices cmdsocket obey ignore uinput triggerparser
THCMD_COMPS := th-cmd cmdsocket

MAKEDEPEND = $(CC) -M -MG $(CPPFLAGS) -o $*.d $<

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

linux_input_defs_gen.inc:
	echo "#include <linux/input.h>" | $(CC) $(CPPFLAGS) -dM -E - > $@

evtable_%.inc: linux_input_defs_gen.inc
	awk '/^#define $*_/ && $$2 !~ /_(MAX|CNT|VERSION)$$/ {print "EV_MAP("$$2"),"}' $< > $@

version.h: version.inc
	sed -r 's!(.*)!#define TH_VERSION "\1"!' $< > $@

clean:
	rm -f *.d
	rm -f *.o
	rm -f linux_input_defs_gen.inc
	rm -f evtable_*.inc
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
