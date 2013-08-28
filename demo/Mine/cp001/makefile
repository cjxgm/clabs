
all: cp001
debug: all
	./cp001
clean:
	rm -f *.o
	rm -f cp001
rebuild: clean all
cp001: cp001.c
	gcc -std=gnu11 -march=native -o cp001 cp001.c -lglut -lGL
