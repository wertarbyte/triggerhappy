thd: thd.c keystate.c eventnames.h executer.h executer.c reader.h
	gcc -o $@ -lpthread thd.c keystate.c executer.c

clean:
	rm thd
