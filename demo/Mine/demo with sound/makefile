CC = gcc
LD = ld
CFLAGS = -I../demolino -O3
LDFLAGS = -L../demolino -ldemolino -L. -lminifmod -lpthread -lglut -s

.PHONY: all clean

debug: all
	./demo
all: demo
clean:
	rm *.o
rebuild: clean all

demo: demo.o
demo.o: demo.c

%: %.o
	$(CC) -o $@ $< $(LDFLAGS)
