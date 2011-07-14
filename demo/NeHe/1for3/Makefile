# Makefile for Linux, IRIX, Solaris and Mac OSX

all: cubes grid quark stars

cubes: cubes.c
	$(CC) -O2 -I/usr/X11R6/include -L/usr/X11R6/lib \
	   -o cubes main.c cubes.c -lGLU -lGL -lX11 -lm
	strip cubes

grid: grid.c
	$(CC) -O2 -I/usr/X11R6/include -L/usr/X11R6/lib \
	   -o grid  main.c grid.c  -lGLU -lGL -lX11 -lm
	strip grid

quark: quark.c
	$(CC) -O2 -I/usr/X11R6/include -L/usr/X11R6/lib \
	   -o quark main.c quark.c -lGLU -lGL -lX11 -lm
	strip quark

stars: stars.c
	$(CC) -O2 -I/usr/X11R6/include -L/usr/X11R6/lib \
	   -o stars main.c stars.c -lGLU -lGL -lX11 -lm
	strip stars

clean:
	rm -f *.o cubes grid quark stars

