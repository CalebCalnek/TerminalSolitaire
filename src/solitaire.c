#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "card.h"

struct cardstack talon;
struct cardstack wastepile;
struct cardstack foundations[4];
struct cardstack tableau[7];

void init(void) {
	setlocale(LC_ALL, "");
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);

	srand(time(NULL));

	/* mouse settup */
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	mouseinterval(0);

	/* color */
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_RED, COLOR_WHITE);
	init_pair(4, COLOR_BLUE, COLOR_WHITE);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	bkgd(COLOR_PAIR(1));

	init_cmdbar();

	/* deal cards */
	init_deck();
	for (int i = 0; i < 7; i++) {
		tableau[i] = init_stack(i);
	}
	talon = init_talon();
}

void draw(void) {
	int i;

	erase();

	/* draw cardstacks */
	draw_talon();
	draw_waste();
	for (i = 0; i < 4; i++) {
		draw_foundations(i);
	}
	for (i = 0; i < 7; i++) {
		draw_stack(tableau[i]);
	}
	draw_cmdbar();
}

int main(void) {
	int ch;
	MEVENT event;

	init();

	/* game loop */
	while (1) {
		draw();
		ch = getch();
		if (ch == KEY_MOUSE) {
			if (getmouse(&event) == OK) {
				handle_mouse_event(event);
			}
		} else {
			if (handle_keyboard(ch)) {
				break;
			}
		}
	}

	endwin();
	return 0;
}
