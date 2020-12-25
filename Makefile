CC=gcc
CFLAGS=-lm -lasound
EXEC=alsatonic

all:
	$(CC) alsatonic.c -o $(EXEC) $(CFLAGS)
