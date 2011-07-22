CC = gcc
CFLAGS = -Idemolino -O3 -Wall
LDFLAGS = -s
LIBS = -Ldemolino -ldemolino -lglut -lGLU

.PHONY: all clean

all: demo
debug: all
	./demo
clean:
	rm -f *.o
	rm -f demo
rebuild: clean all

demo: demo.o
	$(CC) $(LDFLAGS) -o $@ demo.o $(LIBS)
demo.o: demo.c button.c

