CC = gcc
CFLAGS = -Wall -g -I.
LDFLAGS = -lncursesw

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)

.PHONY: all clean

all: solitaire

clean:
	rm -rf solitaire build

build:
	mkdir -p build

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

solitaire: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o solitaire $(LDFLAGS)
