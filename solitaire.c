#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include "card.h"

struct card *held;
struct card *deck[52];
int deck_size;
struct cardstack tableau[7];
struct cardstack foundations[4];
struct cardstack talon;
struct cardstack wastepile;

int contains(int mx, int my, int x1, int y1, int x2, int y2) {
	return mx >= x1 && mx < x2 && my >= y1 && my < y2;
}

void handle_mouse_event(MEVENT event) {
	static int prev_x = 0;
	static int prev_y = 0;
	int i;
	struct card *card_i;

	if (event.bstate & BUTTON1_PRESSED) {
		for (i = 0; i < 7; i++) {

			if (contains(
					event.x,
					event.y,
					STACK_SPACING * (i + 1) + CARD_WIDTH * i,
					1,
					STACK_SPACING * (i + 1) + CARD_WIDTH * i + CARD_WIDTH,
					1 + tableau[i].size + (CARD_HEIGHT - 1)
			)) {
				printf("stack %d\n", i);
				return;
			}

			card_i = tableau[i].top;
			while (card_i != NULL) {
				if (contact_card(event, *card_i)) {
					prev_x = event.x;
					prev_y = event.y;
					held = card_i;
					return;
				}
				card_i = card_i->prev;
			}
		}
	} else if (event.bstate & BUTTON1_RELEASED) {
		if (held != NULL) {
			held->x += event.x - prev_x;
			held->y += event.y - prev_y;
			held = NULL;
		}
	}
}

void init_deck(void) {
	int rank_i, suit_i;

	deck_size = 52;
	for (suit_i = 0; suit_i < 4; suit_i++) {
		for (rank_i = 0; rank_i < 13; rank_i++) {
			deck[suit_i * 13 + rank_i] = init_card(suit_i, rank_i);
		}
	}
}

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
	bkgd(COLOR_PAIR(1));

	init_deck();
	for (int i = 0; i < 7; i++) {
		tableau[i] = init_stack(i);
	}
}

void draw(void) {
	erase();
	for (int i = 0; i < 7; i++) {
		draw_stack(tableau[i]);
	}
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
