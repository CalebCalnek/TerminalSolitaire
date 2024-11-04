#include <curses.h>
#include <locale.h>
#include <card.h>

int card_x, card_y, card_width, card_height;
int drag_card;

void draw_card(void) {
	int x, y, i;

	x = card_x;
	y = card_y;

	attron(COLOR_PAIR(2));
	for (i = 0; i < 3; i++) {
		move(y + i, x);
		addch(' ');
		mvaddstr(y + i, x + 1, "♠♣♥♦");
		addch(' ');
	}
	attroff(COLOR_PAIR(2));
}

int mouse_inbounds(int mx, int my, int x1, int y1, int x2, int y2) {
	return mx >= x1 && mx <= x2 && my >= y1 && my <= y2;
}

void handle_mouse_event(MEVENT event) {
	if (event.bstate & BUTTON1_PRESSED) {
		if (mouse_inbounds(
			event.x, event.y,
			card_x, card_y,
			card_x + card_width, card_y + card_height
		)) {
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

	card_x = 0;
	card_y = 0;
	card_width = 3;
	card_height = 3;
	drag_card = 0;
}

void draw(void) {
	erase();
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
