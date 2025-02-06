#include <curses.h>
#include <stdlib.h>
#include "card.h"

struct card *deck[52];
int deck_size;

void init_deck(void) {
	int rank_i, suit_i;

	deck_size = 52;
	for (suit_i = 0; suit_i < 4; suit_i++) {
		for (rank_i = 0; rank_i < 13; rank_i++) {
			deck[suit_i * 13 + rank_i] = init_card(suit_i, rank_i);
		}
	}
}

struct card *deck_select() {
	int rand_index;
	struct card *card_i;

	if (deck_size == 0) return NULL;

	rand_index = RAND(deck_size);
	card_i = deck[rand_index];
	deck[rand_index] = deck[deck_size - 1];
	deck[deck_size - 1] = NULL;
	deck_size--;
	return card_i;
}
