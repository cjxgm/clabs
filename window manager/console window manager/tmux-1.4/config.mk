# Linux
CFLAGS+= -std=c99 -D_GNU_SOURCE -D_POSIX_SOURCE
LIBS+= -lncurses -lutil -levent -lrt
SRCS+= osdep-linux.c 	compat/closefrom.c 	compat/fgetln.c 	compat/strlcat.c 	compat/strlcpy.c 	compat/strtonum.c 	compat/getopt.c 	compat/vis.c 	compat/unvis.c 	compat/imsg-buffer.c 	compat/imsg.c
