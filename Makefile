thd: thd.c keystate.c eventnames.h
	gcc -o $@ -lpthread thd.c keystate.c

clean:
	rm thd
