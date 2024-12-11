#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include "card.h"

struct card *held, *held_top;
int held_i, held_size;
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
	int i, j;
	struct card *card_i, *tmp;

	if (event.bstate & BUTTON1_PRESSED) {
		for (i = 0; i < 7; i++) {
			if (tableau[i].size == 0) continue;
			if (contains(
				event.x,
				event.y,
				TABLEAU_X + STACK_SPACING * (i + 1) + CARD_WIDTH * i,
				TABLEAU_Y,
				TABLEAU_X + STACK_SPACING * (i + 1) + CARD_WIDTH * (i + 1),
				TABLEAU_Y + tableau[i].size + (CARD_HEIGHT - 1)
			)) {
				card_i = tableau[i].top;
				j = tableau[i].size - 1;
				while (card_i != NULL) {
					if (event.y >= TABLEAU_Y + j &&
					event.y < TABLEAU_Y + j + CARD_HEIGHT) {
						break;
					}
					card_i = card_i->prev;
					j--;
				}
				if (card_i->face != DOWN) {
					held = card_i;
					held_top = tableau[i].top;
					held_i = i;
					held_size = tableau[i].size - j;
				}
				return;
			}
		}
	} else if (event.bstate & BUTTON1_RELEASED) {
		if (held != NULL) {
			for (i = 0; i < 7; i++) {
				if (i == held_i) continue;
				if (contains(
					event.x,
					event.y,
					TABLEAU_X + STACK_SPACING * (i + 1) + CARD_WIDTH * i,
					TABLEAU_Y,
					TABLEAU_X + STACK_SPACING * (i + 1) + CARD_WIDTH * (i + 1),
					TABLEAU_Y + tableau[i].size + (CARD_HEIGHT - 1)
				)) {
					card_i = tableau[i].top;
					tmp = held->prev;

					if (card_i != NULL) {
						card_i->next = held;
						held->prev = card_i;
					} else {
						held->prev = NULL;
						tableau[i].bottom = held;
					}
					tableau[i].top = held_top;
					tableau[i].size += held_size;

					tableau[held_i].top = tmp;
					tableau[held_i].size -= held_size;
					if (tmp != NULL) {
						tableau[held_i].top->face = UP;
						tableau[held_i].top->next = NULL;
					} else {
						tableau[held_i].bottom = NULL;
					}

					held = NULL;
					return;
				}
			}
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
