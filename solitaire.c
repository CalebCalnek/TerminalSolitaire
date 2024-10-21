#include <curses.h>

int card_x, card_y;
int drag_card;

void draw(void) {
	clear();
	attron(COLOR_PAIR(2));
	mvprintw(card_y, card_x, " ");
	attroff(COLOR_PAIR(2));
}

void handle_mouse_event(MEVENT event) {
	if (event.bstate & BUTTON1_PRESSED) {
		if (event.x == card_x && event.y == card_y) {
			drag_card = 1;
		}
	} else if (event.bstate & BUTTON1_RELEASED) {
		if (drag_card) {
			card_x = event.x;
			card_y = event.y;
			drag_card = 0;
		}
	}
}

int main(void) {
	initscr();
	keypad(stdscr, TRUE);
	noecho();

	curs_set(0);

	/* mouse settup */
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	mouseinterval(0);

	/* color */
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_BLACK, COLOR_RED);
	bkgd(COLOR_PAIR(1));

	card_x = 0;
	card_y = 0;
	drag_card = 0;

	refresh();

	draw();

	int ch;
	MEVENT event;

	/* loop */
	while (ch != 'q') {
		ch = getch();
		if (ch == KEY_MOUSE) {
			if (getmouse(&event) == OK) {
				handle_mouse_event(event);
			}
		}
		draw();
	}

	endwin();
	return 0;
}
