CC = gcc
SDL = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_gfx
DEBUG = #-g
CFLAGS = $(DEBUG) $(SDL) -std=c99

syobon: DxLib.o main.o loadg.o
	$(CC) $(CFLAGS) -o $@ DxLib.o loadg.o main.o
main.o: main.c main.h
loadg.o: loadg.c
DxLib.o: DxLib.c DxLib.h

clean:
	rm *.o

