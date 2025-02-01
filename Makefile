CC = gcc
CFLAGS = -Wall -g -I.
LDFLAGS = -lncursesw

SRC = src/solitaire.c src/card.c src/cardstack.c

.PHONY: all clean

all: solitaire

clean:
	rm -f solitaire

solitaire: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o solitaire $(LDFLAGS)
