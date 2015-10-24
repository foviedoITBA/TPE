CC = gcc
CFLAGS = -ansi -pedantic -o
OUTPUT_FILE = 2048.out
FILES = 2048Front.c 2048Back.c getnum.c

default: 2048

2048: 2048Front.c 2048Back.c getnum.c
	$(CC) $(CFLAGS) $(OUTPUT_FILE) $(FILES)

back.o: back.c
	$(COMPILER) -c back.c

clean:
	rm -r $(OUTPUT_FILE) *.o *.h.gch

