
CC = gcc
CFLAGS = -g

all: vm

debug: all
	nasm test.asm
	./vm

vm: vm.o
vm.o: vm.c
