
CC = gcc
CFLAGS = -Ofast -s -Wall -I../demolino
AR = ar
OBJS = link.o ui.o object.o button.o adjust.o

all: libui.a
clean:
	rm -f *.o
	rm -f *.a
rebuild: clean all
debug: all

libui.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

link.o: link.c link.h
ui.o: ui.c ui.h object.h button.h adjust.h link.h
object.o: object.c object.h
button.o: button.c button.h object.h
adjust.o: adjust.c adjust.h object.h button.h

