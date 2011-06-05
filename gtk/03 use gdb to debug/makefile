
CC = gcc
DEBUG = -g
LIBS = gtk+-2.0

PROG_NAME = gtkapp
INCS = 
SRCS = ${PROG_NAME}.c

OBJS = ${SRCS:.c=.o}

CFLAGS = `pkg-config --cflags ${LIBS}` $(DEBUG) -Wall
LDFLAGS = `pkg-config --libs ${LIBS}` $(DEBUG) -Wall

all: ${PROG_NAME}
debug: all
	./${PROG_NAME}

${PROG_NAME}: ${OBJS}
	${CC} -o ${PROG_NAME} ${OBJS} ${LDFLAGS}

${OBJS}:${INCS}

.c.o:
	${CC} -c $< ${CFLAGS}

clean:
	rm -f *.o ${PROG_NAME}

rebuild: clean all

