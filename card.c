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

int mouse_inbounds(int mx, int my, int x1, int y1, int x2, int y2) {
	return mx >= x1 && mx < x2 && my >= y1 && my < y2;
}

int contact_card(MEVENT event, struct card card) {
	return mouse_inbounds(
			event.x, event.y,
			card.x, card.y,
			card.x + CARD_WIDTH, card.y + CARD_HEIGHT
			);
}

void draw_card(struct card card, int stack_i, int card_count) {
	int x = TABLEAU_X + STACK_SPACING * (stack_i + 1) + CARD_WIDTH * stack_i;
	int y = TABLEAU_Y + card_count;
	char *suit = suit_chars[card.suit];
	char *rank = rank_chars[card.rank];

	int end_x = card.rank == TEN ? CARD_WIDTH - 2 : CARD_WIDTH - 1;
	int mid_x = CARD_WIDTH / 2;
	int end_y = CARD_HEIGHT - 1;
	int mid_y = CARD_HEIGHT / 2;

	attron(COLOR_PAIR(card.suit & 2 ? 3 : 2));

	// draw blank card
	char *fill_char = card.face == UP ? " " : "▒";
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
