all: ish
ish: ish.o lex.yy.o
	gcc -o ish ish.o lex.yy.o -ll  
ish.o: ./src/ish.c
	gcc -Wall -g -c ./src/ish.c
lex.yy.o: ./src/lex.yy.c
	gcc -Wall -g -c lex.yy.c
lex.yy.c: ./src/lex.c
	flex ./src/lex.c
clean:
	rm -f *.o
	rm -f lex.yy.c
	rm -f ish
