#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include "card.h"

char *suit_chars[4] = { "♣", "♠", "♥", "♦" };
char *rank_chars[13] = { "A", "2", "3", "4", "5", "6", "7",
	"8", "9", "10", "J", "Q", "K" };

struct card *init_card(int suit, int rank) {
	struct card *newcard;
	newcard = (struct card *) malloc(sizeof(struct card));
	newcard->suit = suit;
	newcard->rank = rank;
	return newcard;
}

int contains(int mx, int my, int x1, int y1, int x2, int y2) {
	return mx >= x1 && mx < x2 && my >= y1 && my < y2;
}

void draw_card(struct card card, int x, int y) {
	char *suit = suit_chars[card.suit];
	char *rank = rank_chars[card.rank];

	int end_x = card.rank == TEN ? CARD_WIDTH - 2 : CARD_WIDTH - 1;
	int end_y = CARD_HEIGHT - 1;
	int mid_x = CARD_WIDTH / 2;
	int mid_y = CARD_HEIGHT / 2;

	char *fill_char;

	if (card.face == UP) {
		fill_char = " ";
		attron(COLOR_PAIR(card.suit & 2 ? 3 : 2));
	} else {
		fill_char = "▒";
		attron(COLOR_PAIR(4));	/* face down, color blue */
	}

	// draw blank card
	for (int i = 0; i < CARD_HEIGHT; i++) {
		for (int j = 0; j < CARD_WIDTH; j++) {
			mvaddstr(y + i, x + j, fill_char);
		}
	}

	// draw symbols
	if (card.face == UP) {
		mvaddstr(y, x, rank);
		mvaddstr(y + mid_y, x + mid_x, suit);
		mvaddstr(y + end_y, x + end_x, rank);
	}

	attroff(COLOR_PAIR(2));
}
