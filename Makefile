keyread: keyread.c eventnames.h
	gcc -o $@ $<

clean:
	rm keyread
