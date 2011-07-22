AR = ar
CC = gcc
CFLAGS = #-g
OBJS = demofw.o demoutil.o demostr.o demosnd.o

.PHONY: all clean

all: libdemolino.a
debug: all
clean:
	rm -f *.o
	rm -f *.a
rebuild: clean all

libdemolino.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)
demofw.o: demofw.c demofw.h
demoutil.o: demoutil.c demoutil.h
demostr.o: demostr.c demostr.h
demosnd.o: demosnd.c demosnd.h

