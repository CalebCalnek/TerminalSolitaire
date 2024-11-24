#include <stdlib.h>
#include "card.h"

struct cardstack init_stack() {
	struct cardstack newstack;
	newstack.top = NULL;
	for (int i = 4; i >= 0; i--) {
		struct card *card_i = (struct card *) malloc(sizeof(struct card));
		card_i->x = 0;
		card_i->y = 5 - i;
		card_i->rank = i;
		card_i->suit = HEARTS;
		/* card_i->next = NULL; */
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
	struct card *i;
	i = stack.bottom;
	while (i != NULL) {
		draw_card(*i);
		i = i->next;
	}
}
