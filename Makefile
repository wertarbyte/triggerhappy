thd: thd.c keystate.c eventnames.h executer.h executer.c
	gcc -o $@ -lpthread thd.c keystate.c executer.c

clean:
	rm thd
