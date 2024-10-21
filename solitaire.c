#include <curses.h>

int card_x, card_y;

void draw(void) {
	clear();
	attron(COLOR_PAIR(2));
	mvprintw(card_y, card_x, " ");
	attroff(COLOR_PAIR(2));
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
	card_y = 1;

	refresh();

	draw();

	int ch;
	MEVENT event;
	while (ch != 'q') {
		ch = getch();
		if (ch == KEY_MOUSE) {
			if (getmouse(&event) == OK) {
				if (event.bstate & BUTTON1_PRESSED) {
					draw();
					mvprintw(0, 0, "mouseX: %d; mouseY: %d", event.x, event.y);
				} else if (event.bstate & BUTTON1_RELEASED) {
					if (event.x == card_x && event.y == card_y) {
						mvprintw(2, 0, "clicked card");
					}
				}
			}
		}
	}

	endwin();
	return 0;
}
