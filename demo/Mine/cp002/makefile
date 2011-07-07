CC = gcc
LD = ld
CFLAGS = -I../demolino
LDFLAGS = -L../demolino -ldemolino -lglut

.PHONY: all clean

all: demo
clean:
	rm -f *.o
	rm -f demo

demo: demo.o
demo.o: demo.c

%: %.o
	$(CC) -o $@ $< $(LDFLAGS)
