AR = ar
CC = gcc
CFLAGS = -Ofast -s -Wall #-g
OBJS = demofw.o demoutil.o demostr.o demosnd.o

all: libdemolino.a
clean:
	rm -f *.o
	rm -f *.a
rebuild: clean all
debug: all

libdemolino.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)
demofw.o: demofw.c demofw.h
demoutil.o: demoutil.c demoutil.h
demostr.o: demostr.c demostr.h
demosnd.o: demosnd.c demosnd.h

