CC = gcc
CFLAGS = -Idemolino -O3 -Wall
LDFLAGS = -s
LIBS = -Ldemolino -ldemolino -L. -lminifmod -lpthread -lglut -lGL -lm -lGLU

.PHONY: all clean

all: libdemolino demo
debug: all
	./demo
clean:
	rm -f *.o
	rm -f demo
	rm -f sound_conv
	rm -f sound.res
rebuild: clean all

demo: demo.o sound.o
	$(CC) $(LDFLAGS) -o $@ demo.o sound.o $(LIBS)
	upx --ultra-brute demo
sound.o: sound.c sound.h sound.res

sound.res: sound.xm sound_conv
	cat $< | ./sound_conv > $@
libdemolino:
	(cd demolino; make)
