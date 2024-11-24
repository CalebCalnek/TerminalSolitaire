#include <curses.h>
#include <locale.h>
#include "card.h"

int drag_card;
struct card mycard;
struct cardstack stacks[7];

int mouse_inbounds(int mx, int my, int x1, int y1, int x2, int y2) {
	return mx >= x1 && mx < x2 && my >= y1 && my < y2;
}

void handle_mouse_event(MEVENT event) {
	static int prev_x = 0;
	static int prev_y = 0;
	int i;
	int card_x, card_y;
	struct node *node_i;

	if (event.bstate & BUTTON1_PRESSED) {
		for (i = 0; i < 7; i++) {
			for (node_i = stacks[i].top; node_i != NULL;
					node_i = node_i->prev) {
				card_x = node_i->value.x;
				card_y = node_i->value.y;

				if (mouse_inbounds(
					event.x, event.y,
					card_x, card_y,
					card_x + CARD_WIDTH, card_y + CARD_HEIGHT
				)) {
					prev_x = event.x;
					prev_y = event.y;
					drag_card = 1;
					printf("test: clicked %d\t", node_i->value.rank);
					break;
				}
			}
		}
	} else if (event.bstate & BUTTON1_RELEASED) {
		if (drag_card) {
			mycard.x += event.x - prev_x;
			mycard.y += event.y - prev_y;
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

	mycard.x = 0;
	mycard.y = 0;
	mycard.suit = DIAMONDS;
	mycard.rank = KING;

	drag_card = 0;

	stacks[0] = init_stack();
}

void draw(void) {
	erase();
	/* draw_card(mycard); */
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
