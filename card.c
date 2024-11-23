#include <curses.h>
#include <stdio.h>
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

	attron(COLOR_PAIR(2));

	mvaddstr(y, x, rank);
	mvaddstr(y, x + 1, "       ");
	mvaddstr(y + 1, x, "        ");
	mvaddstr(y + 2, x, "   ");
	mvaddstr(y + 2, x + 3, suit);
	mvaddstr(y + 2, x + 4, "    ");
	mvaddstr(y + 3, x, "        ");
	mvaddstr(y + 4, x, "       ");
	mvaddstr(y + 4, x + 7, rank);

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
