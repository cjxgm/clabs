bin_PROGRAMS = karmen

karmen_SOURCES = \
    button.c \
    button.h \
    delete.xbm \
    global.h \
    grab.c \
    hints.c \
    hints.h \
    icccm.c \
    lib.c \
    lib.h \
    list.h \
    main.c \
    menu.c \
    menu.h \
    resizer.c \
    resizer.h \
    title.c \
    title.h \
    unmap.xbm \
    widget.c \
    widget.h \
    window.c \
    window.h

dist_man_MANS = karmen.1

karmen_CPPFLAGS =
karmen_CFLAGS = $(X_CFLAGS)
karmen_LDFLAGS = $(X_LIBS)

if CONFIG_EWMH
karmen_SOURCES+=ewmh.c
karmen_CPPFLAGS+=-DCONFIG_EWMH=1
endif

karmen_LDADD = $(X_PRE_LIBS) -lX11 $(X_EXTRA_LIBS)
