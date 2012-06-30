
CC = g++
CXXFLAGS = `fltk-config --cxxflags`
LDFLAGS  = `fltk-config --ldstaticflags`

all: app
clean:
	rm -f *.o
	rm -f app
rebuild: clean all
debug: all
	./app

app: main.o app.o
	g++ -o $@ $^ $(LDFLAGS)

.o: .cc
	g++ -c -o $@ $^ $(CXXFLAGS)

