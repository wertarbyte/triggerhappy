THD_COMPONENTS := thd keystate trigger eventnames devices cmdsocket obey
THD_OBJS := $(THD_COMPONENTS:%=%.o)
MAKEDEPEND = gcc -M $(CPPFLAGS) -o $*.d $<

all: thd th-cmd

thd: $(THD_OBJS)

th-cmd: th-cmd.o cmdsocket.o

clean:
	rm *.d
	rm *.o
	rm thd th-cmd

%.d : %.c
	$(MAKEDEPEND)

include $(THD_COMPONENTS:%=%.d)
