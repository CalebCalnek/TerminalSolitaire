CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lncurses

.PHONY: all clean

all: solitaire

clean:
	rm -f solitaire

solitaire: solitaire.c
	$(CC) $(CFLAGS) $(LDFLAGS) solitaire.c -o solitaire
