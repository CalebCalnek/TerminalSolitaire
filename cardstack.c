#include <curses.h>
#include <stdlib.h>
#include "card.h"

struct cardstack init_stack(int index) {
	struct cardstack newstack;

	newstack.top = NULL;
	newstack.index = index;

	for (int i = index; i >= 0; i--) {
		struct card *card_i = (struct card *) malloc(sizeof(struct card));
		card_i->face = i == 0 ? UP : DOWN;
		card_i->rank = i;
		card_i->suit = HEARTS;
		if (newstack.top != NULL) {
			card_i->prev = newstack.top;
			newstack.top->next = card_i;
		} else {
			newstack.bottom = card_i;
		}
		newstack.top = card_i;
	}

	return newstack;
}

void push(struct cardstack basestack, struct cardstack newstack) {
	basestack.top->next = newstack.bottom;
	basestack.top = newstack.top;
}

void draw_stack(struct cardstack stack) {
	struct card *card_i;
	int card_count;

	card_i = stack.bottom;
	card_count = 0;
	while (card_i != NULL) {
		draw_card(*card_i, stack.index, card_count);
		card_i = card_i->next;
		card_count++;
	}
}
