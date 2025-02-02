#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include "card.h"

struct card *deck[52];
int deck_size;

struct cardstack *held;

struct cardstack talon;
struct cardstack wastepile;
struct cardstack foundations[4];
struct cardstack tableau[7];

int show_cmdbar;
char command[8];
int cmd_len;

void set_held(struct card *top, struct card *bottom, int size, int i) {
	held = (struct cardstack *) malloc(sizeof(struct cardstack));
	*held = (struct cardstack) { top, bottom, size, i };
}

void handle_mouse_event(MEVENT event) {
	int i, j;
	struct card *card_i;

	if (event.bstate & BUTTON1_PRESSED) {
		/* check click talon*/
		if (talon_contains(event)) {
			if (talon.size == 0) {
				reset_talon();
				return;
			}
			struct cardstack card = { talon.top, talon.top, 1, -1 };
			move_card(&wastepile, &card);
			return;
		}

		/* check click waste */
		if (wastepile.size > 0 && wastepile_contains(event)) {
			set_held(wastepile.top, wastepile.top, 1, -1);
		}

		/* check click tableau */
		for (i = 0; i < 7; i++) {
			if (tableau[i].size == 0) continue;
			if (tableau_contains(event, i)) {
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
					set_held(tableau[i].top, card_i, tableau[i].size - j, i);
				}
				return;
			}
		}
	} else if (event.bstate & BUTTON1_RELEASED && held != NULL) {
		for (i = 0; i < 7; i++) {
			// skip check for drop in same stack
			if (i == held->index) continue;

			if (tableau_contains(event, i)) {
				if (can_move(&tableau[i], *held->bottom, i)) {
					move_card(&tableau[i], held);
				}
				free(held);
				held = NULL;
				return;
			}
		}

		for (i = 0; i < 4; i++) {
			if (held->size == 1 && foundation_contains(event, i)) {
				if (can_move(&foundations[i], *held->bottom, i)) {
					move_card(&foundations[i], held);
				}
				free(held);
				held = NULL;
				return;
			}
		}

		free(held);
		held = NULL;
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
	init_pair(4, COLOR_BLUE, COLOR_WHITE);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	bkgd(COLOR_PAIR(1));

	show_cmdbar = 0;
	cmd_len = 0;

	/* deal cards */
	init_deck();
	for (int i = 0; i < 7; i++) {
		tableau[i] = init_stack(i);
	}
	talon = init_talon();
}

void draw_cmdbar() {
	int i, col_end, row_end;

	getmaxyx(stdscr, row_end, col_end);
	attron(COLOR_PAIR(5));
	for (i = 0; i < col_end; i++) {
		mvaddstr(row_end - 1, i, " ");
	}
	mvaddch(row_end - 1, 0, ':');
	mvaddstr(row_end - 1, 1, command);
	attroff(COLOR_PAIR(5));
}

void draw(void) {
	int i;

	erase();

	/* draw cardstacks */
	draw_talon();
	draw_waste();
	for (i = 0; i < 4; i++) {
		draw_foundations(i);
	}
	for (i = 0; i < 7; i++) {
		draw_stack(tableau[i]);
	}

	if (show_cmdbar) {
		draw_cmdbar();
	}
}

int main(void) {
	int ch;
	MEVENT event;

	init();

	/* game loop */
	while (ch != 'q') {
		draw();
		ch = getch();
		if (ch == KEY_MOUSE) {
			if (getmouse(&event) == OK) {
				handle_mouse_event(event);
			}
		} else if ((char) ch == ':') {
			show_cmdbar ^= 1;
		} else if (show_cmdbar && cmd_len < 7) {
			command[cmd_len++] = ch;
		}
	}

	endwin();
	return 0;
}
