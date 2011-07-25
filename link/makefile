
CC = gcc
CFLAGS = -g#-Ofast -s

all: link
clean:
	rm -f *.o
	rm -f link
rebuild: clean all
debug: all
	./link

link: main.o link.o
link.o: link.c link.h
main.o: main.c link.h

