CSTANDARD = -ansi
CFLAGS = -Wall -pedantic

compile: clean ibadcnt igoodcnt 

ibadcnt: ./src/ibadcnt.c
	gcc ./src/ibadcnt.c -o ibadcnt -lpthread

igoodcnt: ./src/igoodcnt.c
	gcc -o igoodcnt ./src/igoodcnt.c -lpthread -lrt

clean:
	-rm -f ibadcnt
	-rm -f igoodcnt

run: compile
