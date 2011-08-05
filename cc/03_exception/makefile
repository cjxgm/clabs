CC = g++

all: exception
clean:
	rm -f *.o
	rm -f exception
rebuild: clean all
debug: all
	./exception

exception: exception.o Vector.o Exception.o
exception.o: exception.cc Vector.h
Vector.o: Vector.cc Vector.h Exception.h
Exception.o: Exception.cc Exception.h

