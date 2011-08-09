CC = g++

all: class
clean:
	rm -f *.o
	rm -f class
rebuild: clean all
debug: all
	./class

class: class.o Vector.o
class.o: class.cc Vector.h
Vector.o: Vector.cc Vector.h

