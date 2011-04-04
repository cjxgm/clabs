CFLAGS = -Wall
PKG_CFG = `pkg-config --cflags --libs glib-2.0`
DEFS = -D_GNU_SOURCE 
LIBS = -lncurses 
WLIBS = -lncursesw

makefile: all

all: clean keycodes
	strip keycodes

wide: clean keycodes_wide
	strip keycodes_wide

keycodes:
	gcc $(CFLAGS) $(DEFS) $(LIBS) $(PKG_CFG) -g *.c -o keycodes

keycodes_wide:
	gcc $(CFLAGS) $(DEFS) $(WLIBS) $(PKG_CFG) -g -D_VIPER_WIDE *.c -o keycodes_wide

clean:
	rm -f *.o
	rm -f keycodes
	rm -f keycodes_wide
