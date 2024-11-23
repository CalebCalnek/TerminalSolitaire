#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include "card.h"

char *suit_chars[4] = { "♣", "♠", "♥", "♦" };
char *rank_chars[13] = { "A", "2", "3", "4", "5", "6", "7",
	"8", "9", "10", "J", "Q", "K" };

void draw_card(struct card card) {
	int x = card.x;
	int y = card.y;
	char *suit = suit_chars[card.suit];
	char *rank = rank_chars[card.rank];

	int end_x = card.rank == TEN ? CARD_WIDTH - 2 : CARD_WIDTH - 1;
	int mid_x = CARD_WIDTH / 2;
	int end_y = CARD_HEIGHT - 1;
	int mid_y = CARD_HEIGHT / 2;

	attron(COLOR_PAIR(card.suit & 2 ? 3 : 2));

	// draw blank card
	for (int i = 0; i < CARD_HEIGHT; i++) {
		char space_buffer[CARD_WIDTH + 1];
		memset(space_buffer, ' ', CARD_WIDTH);
		space_buffer[CARD_WIDTH] = '\0';
		mvaddstr(y + i, x, space_buffer);
	}

	// draw symbols
	mvaddstr(y, x, rank);
	mvaddstr(y + mid_y, x + mid_x, suit);
	mvaddstr(y + end_y, x + end_x, rank);

	attroff(COLOR_PAIR(2));
}

/*
int main() {
	struct card mycard;
	setlocale(LC_ALL, "");

	mycard.suit = DIAMONDS;
	mycard.rank = KING;

	print_card(mycard);
}
*/
