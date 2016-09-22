all:
	gcc -o ppmrw helpers.c header.c p6.c p3.c ppmrw.c

clean:
	rm ppmrw