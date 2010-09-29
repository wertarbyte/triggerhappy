keyread: keyread.c eventnames.h
	gcc -DTHREADED -o $@ -lpthread $<

clean:
	rm keyread
