#include <stdio.h>
#include <wchar.h>
#include <locale.h>

enum suit {
	CLUBS,
	SPADES,
	HEARTS,
	DIAMONDS
};

enum rank {
	ACE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,
};

// { ♣, ♠, ♥, ♦ };
wchar_t suit_chars[4] = { 0x2663, 0x2660, 0x2665, 0x2666 };
char *rank_chars[13] = { "A", "2", "3", "4", "5", "6", "7",
	"8", "9", "10", "J", "Q", "K" };

struct card {
	enum suit suit;
	enum rank rank;
	int x, y;
};

void print_card(struct card c) {
	wchar_t suit;
	char *rank;
	suit = suit_chars[c.suit];
	rank = rank_chars[c.rank];
	wprintf(L"%s %lc\n", rank, suit);
}

int main() {
	struct card mycard;
	setlocale(LC_ALL, "");

	mycard.suit = DIAMONDS;
	mycard.rank = KING;

	print_card(mycard);
}
