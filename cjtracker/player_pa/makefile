
CC = gcc
CFLAGS = -g
LDFLAGS = -lm -lpulse-simple

all: cjtmplay
debug: all
	./cjtmplay
clean:
	rm *.o
	rm cjtmplay

cjtmplay: cjtmplay.o instrument.o player.o loader.o driver_pa.o
cjtmplay.o: cjtmplay.c type.h
instrument.o: instrument.c instrument.h type.h
player.o: player.c player.h type.h
loader.o: loader.c loader.h type.h
driver_pa.o: driver_pa.c driver_pa.h type.h

