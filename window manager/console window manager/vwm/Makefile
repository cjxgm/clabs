CFLAGS = -std=gnu99 -Wall -O2 -ggdb
PKG_CFG = `pkg-config --cflags --libs glib-2.0 gmodule-2.0 gthread-2.0`
DEFS = -D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS -D_GNU_SOURCE -D_VWM_SCREENSAVER_TIMEOUT=5 -D_DEBUG
LIBS = -lncurses -lviper -lmenu -lform -lgpm -lpseudo
WLIBS = -lncursesw -lviper -lmenu -lform -lgpm -lpseudo
prefix = /usr/local
libdir = ${prefix}/lib
headerdir = ${prefix}/include
includedir = -I${headerdir} -I./
bindir = ${prefix}/bin
moddir = /usr/lib/vwm/modules
moddir_wide = /usr/lib/vwm/modules_wide

makefile: all

all: clean vwm keycode_tool vwmterm

wide: clean vwm_wide keycode_tool_wide vwmterm_wide

.PHONY: vwmterm vwmterm_wide keycode_tool keycode_tool_wide

vwm_wide:
	sh -c "echo '#ifndef _VIPER_WIDE'; echo '#define _VIPER_WIDE 1';echo '#endif';echo '#include <vwm.h>'" > vwm_wide.h
	gcc $(CFLAGS) $(DEFS) $(WLIBS) $(PKG_CFG) $(includedir) -D_VIPER_WIDE *.c -o vwm_wide

vwm:
	gcc $(CFLAGS) $(DEFS) $(LIBS) $(PKG_CFG) $(includdir) *.c -o vwm

vwmterm:
	cd modules/vwmterm3 && $(MAKE)

vwmterm_wide:
	cd modules/vwmterm3 && $(MAKE) wide

keycode_tool:
	cd keycodes && $(MAKE)

keycode_tool_wide:
	cd keycodes && $(MAKE) wide

clean: 
	rm -f *.o
	rm -f vwm
	rm -f vwm_wide
	rm -f vwm_wide.h

install:
	mkdir -p $(moddir)
	chmod 644 vwm.h
	cp -f vwm.h $(headerdir)
	chmod 755 vwm
	cp -f vwm $(bindir)
	cd modules/vwmterm3 && $(MAKE) install

install_wide:
	mkdir -p $(moddir_wide)
	chmod 644 vwm.h
	cp -f vwm.h $(headerdir)
	chmod 644 vwm_wide.h
	cp -f vwm_wide.h $(headerdir)
	chmod 755 vwm_wide
	cp -f vwm_wide $(bindir)
	cd modules/vwmterm3 && $(MAKE) install_wide
