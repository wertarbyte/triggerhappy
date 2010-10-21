THD_COMPS := thd keystate trigger eventnames devices cmdsocket obey
THCMD_COMPS := th-cmd cmdsocket

MAKEDEPEND = gcc -M $(CPPFLAGS) -o $*.d $<

all: thd th-cmd

thd: $(THD_COMPS:%=%.o)

th-cmd: $(THCMD_COMPS:%=%.o)

clean:
	rm -f *.d
	rm -f *.o
	rm -f thd th-cmd

%.d : %.c
	$(MAKEDEPEND)

-include $(THD_COMPS:%=%.d) $(THCMD_COMPS:%=%.d)
