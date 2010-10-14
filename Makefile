thd: thd.c keystate.c eventnames.h executer.h executer.c readerlist.h conf.c conf.h
	gcc -o $@ -lpthread thd.c keystate.c executer.c conf.c eventnames.c

clean:
	rm thd
