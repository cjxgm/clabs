
CC = gcc
all: bfcc bfclean
debug: all
	cat 500.bf | ./bfcc > 500.c && gcc -o 500 500.c && ./500
	@rm -f 500.c
	./500 | wc -l
