all : circ1\
      circ2\
      circ3\
      circ4\
      circ5\
      circ6\
      circ7\
      circ8\
      circ9\
      circ10\
      circ11\
      circ12
CC = gcc
CFLAGS=-O `allegro-config --cflags`
LDLIBS=`allegro-config --shared`
LDFLAGS=-s

% : %o
	$(CC) $(LIBS) -s -o $@ $<

%o : %c
	$(CC) -c $(OPTIONS) $<

