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
	int i;

	int x = STACK_SPACING * (stack_i + 1) + CARD_WIDTH * stack_i;
	int y = 1 + card_count;

	int end_x = CARD_WIDTH - 1;
	int end_y = CARD_HEIGHT - 1;

	// draw edges
	for (i = 0; i < CARD_HEIGHT; i++) {
		mvaddstr(y + i, x, "│");
		mvaddstr(y + i, x + end_x, "│");
	}
	for (i = 0; i < CARD_WIDTH; i++) {
		mvaddstr(y, x + i, "─");
		mvaddstr(y + end_y, x + i, "─");
	}

	mvaddstr(y, x, "┌");
	mvaddstr(y, x + end_x, "┐");
	mvaddstr(y + end_y, x, "└");
	mvaddstr(y + end_y, x + end_x, "┘");
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
