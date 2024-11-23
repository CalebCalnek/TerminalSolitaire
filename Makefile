CC = gcc
CFLAGS = -Wall -g -I.
LDFLAGS = -lncursesw

.PHONY: all clean

all: solitaire

clean:
	rm -f solitaire

solitaire: solitaire.c card.c
	$(CC) $(CFLAGS) solitaire.c card.c -o solitaire $(LDFLAGS)
