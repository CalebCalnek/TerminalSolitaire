#include <stdlib.h>
#include "card.h"

struct cardstack init_stack() {
	struct cardstack newstack;
	newstack.top = NULL;
	for (int i = 0; i < 5; i++) {
		struct card *card_i = (struct card *) malloc(sizeof(struct card));
		struct node *node_i = (struct node *) malloc(sizeof(struct node));
		card_i->x = 0;
		card_i->y = 5 - i;
		card_i->rank = i;
		card_i->suit = HEARTS;
		node_i->value = *card_i;
		node_i->next = NULL;
		if (newstack.top != NULL) {
			node_i->next = newstack.top;
		}
		newstack.top = node_i;
	}
	return newstack;
}

void push(struct cardstack basestack, struct cardstack newstack) {
	basestack.top->next = newstack.bottom;
	basestack.top = newstack.top;
}

void draw_stack(struct cardstack stack) {
	struct node *i;
	i = stack.top;
	while (i != NULL) {
		draw_card(i->value);
		i = i->next;
	}
}
