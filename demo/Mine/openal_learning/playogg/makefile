
CC = gcc
CFLAGS = -Wall -Werror -s -O3 -lopenal -lvorbisfile

all: playogg
debug: all
	./playogg music.ogg
clean:
	rm -f *.o
	rm -f playogg
rebuild: clean all

playogg: playogg.c

