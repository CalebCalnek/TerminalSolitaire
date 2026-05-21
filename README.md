# Terminal Solitaire

This project is an implementation of the klondike solitaire game. It is written
in C and uses the ncurses library for rendering graphical components. The
program runs in a terminal emulator and it supports both mouse controls and
command-line commands.

## Installation

1. clone the repository
2. run `make` in the root of the project directory

## Usage

- run the `solitaire` binary
- mouse controls work intuitively
- command-line controls:
	- Type `:`. This will start the command mode.
	- command format: `<rank><suit>[dst]`
		- rank: `{ 2-10,j,q,k,a }`
		- suit: `{ d,h,s,c }`
		- dst: `{ f,d }` (foundations, tableau)
	- Enter a singular `t` to turn a card from the talon pile.
	- Exit command mode by hitting backspace on a blank line.
	- Exit the program by entering a singular `q`.

