CC = gcc
CFLAGS = -Idemolino -Ofast -Wall
LDFLAGS = -s
LIBS = -Ldemolino -ldemolino -lglut -lGLU -Lui -lui

.PHONY: all clean

all: demo
debug: all
	./demo
clean:
	rm -f *.o
	rm -f demo
cleanall: clean
	make clean -C ui
	make clean -C demolino
rebuild: clean all
rebuildall: cleanall all

demo: demo.o
	$(CC) $(LDFLAGS) -o $@ demo.o $(LIBS)
demo.o: demo.c demolino/libdemolino.a ui/libui.a
demolino/libdemolino.a:
	make -C demolino
ui/libui.a:
	make -C ui

