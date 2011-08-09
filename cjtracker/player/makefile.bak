
CC = gcc
CFLAGS = -g
LDFLAGS = -lpthread -lm

all: cjtmplay
debug: all
	./cjtmplay
clean:
	rm *.o
	rm cjtmplay

cjtmplay: cjtmplay.o instrument.o player.o loader.o
cjtmplay.o: cjtmplay.c type.h
instrument.o: instrument.c instrument.h type.h
player.o: player.c player.h type.h
loader.o: loader.c loader.h type.h

