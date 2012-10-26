# vim: ft=make noet ts=4 sw=4 sts=0

CC = gcc
CFLAGS = -Ofast -Wall -Werror
LDFLAGS = -lasound

.PHONY: cidi clean cleanall rebuild reconf test commit

cidi: build build/cidi
clean:
	rm -rf build
cleanall: clean
	rm -f makefile
rebuild: clean cidi
reconf:
	./configure
test: cidi
	./build/cidi
commit: cleanall
	git add .
	git commit -a || true
	./configure

build:
	mkdir build
build/cidi.o: src/cidi.c
	$(CC) -c -o $@ $< $(CFLAGS)
build/cidi: build/cidi.o
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

