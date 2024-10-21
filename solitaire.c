#include <curses.h>

int card_x, card_y;
int drag_card;

void draw_card(void) {
	int x, y, i;

	x = card_x;
	y = card_y;

	attron(COLOR_PAIR(2));
	for (i = 0; i < 3; i++) {
		move(y + i, x);
		addch(' ');
		addch(' ');
		addch(' ');
	}
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

void init(void) {
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

	card_x = 0;
	card_y = 0;
	drag_card = 0;
}

void draw(void) {
	clear();
	draw_card();
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
