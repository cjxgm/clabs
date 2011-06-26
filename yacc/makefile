
CC = gcc

all: calc
debug: all
	./calc
clean:
	rm -f calc.c
	rm -f calc.lex.c
	rm -f *.o
	rm -f calc
rebuild: clean all

calc: calc.o
calc.o: calc.c calc.lex.c
calc.c: calc.y
calc.lex.c: calc.l
	lex $<
	mv -f lex.yy.c $@

