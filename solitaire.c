#include <curses.h>

int main(void) {
	initscr();

	/* color */
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_BLUE);

	clear();
	bkgd(COLOR_PAIR(1));

	refresh();
	getch();

	endwin();
	return 0;
}
