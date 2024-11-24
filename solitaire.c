#include <curses.h>
#include <locale.h>
#include "card.h"

int drag_card;
struct card *held;
struct cardstack stacks[7];

void handle_mouse_event(MEVENT event) {
	static int prev_x = 0;
	static int prev_y = 0;
	int i;
	struct card *card_i;

	if (event.bstate & BUTTON1_PRESSED) {
		for (i = 0; i < 7; i++) {
			card_i = stacks[i].top;
			while (card_i != NULL) {
				if (contact_card(event, *card_i)) {
					prev_x = event.x;
					prev_y = event.y;
					drag_card = 1;
					held = card_i;
					return;
				}
				card_i = card_i->prev;
			}
		}
	} else if (event.bstate & BUTTON1_RELEASED) {
		if (drag_card) {
			held->x += event.x - prev_x;
			held->y += event.y - prev_y;
			drag_card = 0;
		}
	}
}

void init(void) {
    setlocale(LC_ALL, "");
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);

	/* mouse settup */
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	mouseinterval(0);

	/* color */
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_RED, COLOR_WHITE);
	bkgd(COLOR_PAIR(1));

	drag_card = 0;

	stacks[0] = init_stack();
}

void draw(void) {
	erase();
	draw_stack(stacks[0]);
	/*
	for (int i = 0; i < 7; i++) {
		draw_stack(stacks[i]);
	}
	*/
}

int main(void) {
	int ch;
	MEVENT event;

	init();

	/* loop */
	while (ch != 'q') {
		draw();
		ch = getch();
		if (ch == KEY_MOUSE) {
			if (getmouse(&event) == OK) {
				handle_mouse_event(event);
			}
		}
	}

	endwin();
	return 0;
}
