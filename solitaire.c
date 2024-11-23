#include <curses.h>
#include <locale.h>
#include "card.h"

int drag_card;
struct card mycard;

int mouse_inbounds(int mx, int my, int x1, int y1, int x2, int y2) {
	return mx >= x1 && mx < x2 && my >= y1 && my < y2;
}

void handle_mouse_event(MEVENT event) {
	if (event.bstate & BUTTON1_PRESSED) {
		if (mouse_inbounds(
			event.x, event.y,
			mycard.x, mycard.y,
			mycard.x + CARD_WIDTH, mycard.y + CARD_HEIGHT
		)) {
			drag_card = 1;
		}
	} else if (event.bstate & BUTTON1_RELEASED) {
		if (drag_card) {
			mycard.x = event.x;
			mycard.y = event.y;
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
	bkgd(COLOR_PAIR(1));

	mycard.x = 0;
	mycard.y = 0;
	mycard.suit = DIAMONDS;
	mycard.rank = KING;

	drag_card = 0;
}

void draw(void) {
	erase();
	draw_card(mycard);
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
