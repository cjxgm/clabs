CC = gcc
LD = ld
CFLAGS = -I../demolino
LDFLAGS = -L../demolino -ldemolino -lglut -s

.PHONY: all clean

debug: all
	./demo
all: demo
clean:
	rm *.o

demo: demo.o
demo.o: demo.c

%: %.o
	$(CC) -o $@ $< $(LDFLAGS)
