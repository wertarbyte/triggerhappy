THD_COMPONENTS := thd keystate trigger eventnames devices
THD_OBJS := $(THD_COMPONENTS:%=%.o)
MAKEDEPEND = gcc -M $(CPPFLAGS) -o $*.d $<

all: thd

thd: $(THD_OBJS)

clean:
	rm $(THD_COMPONENTS:%=%.d)
	rm $(THD_COMPONENTS:%=%.o)
	rm thd

%.d : %.c
	$(MAKEDEPEND)

include $(THD_COMPONENTS:%=%.d)
