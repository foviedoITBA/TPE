CC = gcc
CFLAGS = -ansi -pedantic
TARGET = 2048.out

default: 2048

2048: 2048Front.c 2048Back.c getnum.c
	$(CC) $(CFLAGS) -o 2048.out 2048Front.c 2048Back.c getnum.c

