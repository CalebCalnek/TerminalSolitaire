.PHONY: all clean

all: solitaire

clean:
	rm -f solitaire

solitaire: solitaire.c
	gcc solitaire.c -o solitaire
