#include <curses.h>
#include <stdlib.h>
#include "card.h"

extern struct card *deck[52];
extern int deck_size;

struct cardstack init_stack(int index) {
	struct cardstack newstack;

	newstack.top = NULL;
	newstack.index = index;
	newstack.size = 0;

	for (int i = index; i >= 0; i--) {
		int rand_index = RAND(deck_size);
		struct card *card_i = deck[rand_index];
		deck[rand_index] = deck[deck_size - 1];
		deck[deck_size - 1] = NULL;
		deck_size--;

		card_i->face = i == 0 ? UP : DOWN;
		if (newstack.top != NULL) {
			card_i->prev = newstack.top;
			newstack.top->next = card_i;
		} else {
			newstack.bottom = card_i;
		}
		newstack.top = card_i;
		newstack.size++;
	}

	return newstack;
}

void push(struct cardstack basestack, struct cardstack newstack) {
	basestack.top->next = newstack.bottom;
	basestack.top = newstack.top;
}

void draw_empty_stack(int stack_i, int card_count) {
	int x = STACK_SPACING * (stack_i + 1) + CARD_WIDTH * stack_i;
	int y = 1 + card_count;
	for (int i = 0; i < CARD_HEIGHT; i++) {
		for (int j = 0; j < CARD_WIDTH; j++) {
			mvaddstr(y + i, x + j, "a");
		}
	}
}

void draw_stack(struct cardstack stack) {
	struct card *card_i;
	int card_count;

	card_i = stack.bottom;
	card_count = 0;

	if (card_i == NULL) {
		draw_empty_stack(stack.index, card_count);
		return;
	}

	while (card_i != NULL) {
		draw_card(*card_i, stack.index, card_count);
		card_i = card_i->next;
		card_count++;
	}
}
