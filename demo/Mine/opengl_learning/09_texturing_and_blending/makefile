CC = gcc
CFLAGS = -I../../demolino2 -O3 -Wall
LDFLAGS = -s
LIBS = -L../../demolino2 -ldemolino -L. -lminifmod -lpthread -lglut -lGLU

.PHONY: all clean

all: demo
debug: all
	aoss ./demo
clean:
	rm -f *.o
	rm -f demo
	rm -f sound_conv
	rm -f sound.res
rebuild: clean all

demo: demo.o sound.o
	$(CC) $(LDFLAGS) -o $@ demo.o sound.o $(LIBS)
	#upx --ultra-brute demo
sound.o: sound.c sound.h sound.res

sound.res: sound.xm sound_conv
	cat $< | ./sound_conv > $@
