#include <curses.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "card.h"

// { ♣, ♠, ♥, ♦ };
wchar_t suit_chars[4] = { 0x2663, 0x2660, 0x2665, 0x2666 };
char *rank_chars[13] = { "A", "2", "3", "4", "5", "6", "7",
	"8", "9", "10", "J", "Q", "K" };

void print_card(struct card c) {
	wchar_t suit;
	char *rank;
	suit = suit_chars[c.suit];
	rank = rank_chars[c.rank];
	wprintf(L"%s %lc\n", rank, suit);
}

void draw_card(struct card card) {
	int x, y, i;

	x = card.x;
	y = card.y;

	attron(COLOR_PAIR(2));
	for (i = 0; i < 3; i++) {
		move(y + i, x);
		addch(' ');
		mvaddstr(y + i, x + 1, "♠♣♥♦");
		addch(' ');
	}
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
