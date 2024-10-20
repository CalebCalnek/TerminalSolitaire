#include <curses.h>

int main(void) {
	initscr();
	keypad(stdscr, TRUE);
	noecho();

	/* mouse settup */
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	mouseinterval(0);

	/* color */
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	bkgd(COLOR_PAIR(1));

	refresh();

	MEVENT event;
	while (1) {
		if (getch() == KEY_MOUSE) {
			if (getmouse(&event) == OK) {
				if (event.bstate & BUTTON1_PRESSED) {
					break;
				}
			}
		}
	}

	endwin();
	return 0;
}
