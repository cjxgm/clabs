# Clanjor Utilities Library Makefile
# Under GPLv3, NO WARRANTY!!!
# Copyleft (C) eXerigumo Clanjor, 2010-2011.

AR = ar
CC = gcc
DEBUG = -g
CFLAGS = $(DEBUG) -I./include
OBJS = cju_str.o

debug: all test
	./test

test: test.c lib/libcju.a include/cju.h
	$(CC) $(CFLAGS) -o $@ $< -L./lib -lcju

all: lib/libcju.a

clean:
	rm *.o

lib/libcju.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

cju_str.o: src/cju_str.c include/cju_str.h include/cju_type.h
	$(CC) $(CFLAGS) -c $<


