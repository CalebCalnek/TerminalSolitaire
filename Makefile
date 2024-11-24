CC = gcc
CFLAGS = -Wall -g -I.
LDFLAGS = -lncursesw

SRC = solitaire.c card.c cardstack.c

.PHONY: all clean

all: solitaire

clean:
	rm -f solitaire

solitaire: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o solitaire $(LDFLAGS)
