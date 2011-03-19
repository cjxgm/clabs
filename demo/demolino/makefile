AR = ar
CC = gcc
CFLAGS = -g
OBJS = demofw.o demoutil.o demostr.o demosnd.o

.PHONY: all clean

debug: test
	./test
test: test.c libdemolino.a
	gcc -g -I. -o test test.c -L. -ldemolino -lglut
all: libdemolino.a
clean:
	rm *.o
	rm *.a

libdemolino.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)
demofw.o: demofw.c demofw.h
demoutil.o: demoutil.c demoutil.h
demostr.o: demostr.c demostr.h
demosnd.o: demosnd.c demosnd.h

