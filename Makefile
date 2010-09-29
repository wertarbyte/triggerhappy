keyread: keyread.c eventnames.h
	gcc -o $@ -lpthread $<

clean:
	rm keyread
