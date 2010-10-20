thd: *.c *.h
	gcc -o $@ thd.c keystate.c trigger.c triggerdir.c eventnames.c

clean:
	rm thd
