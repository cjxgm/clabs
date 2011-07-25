CC = gcc
CFLAGS = -Idemolino -Ofast -Wall
LDFLAGS = -s
LIBS = -Ldemolino -ldemolino -lglut -lGLU

all: demo
debug: all
	./demo
clean:
	rm -f *.o
	rm -f demo
cleanall: clean
	make clean -C demolino
rebuild: clean all
rebuildall: cleanall all

demo: demo.o pixmap.o demolino/libdemolino.a
	$(CC) $(LDFLAGS) -o $@ demo.o pixmap.o $(LIBS)
demo.o: demo.c pixmap.h
pixmap.o: pixmap.c pixmap.h

demolino/libdemolino.a:
	make -C demolino

