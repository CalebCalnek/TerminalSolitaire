#include <curses.h>
#include <stdlib.h>
#include "card.h"

cardstack_t tableau[7];

void init_tableau(int index) {
	cardstack_t newstack;
	card_t *card_i;

	newstack.top = NULL;
	newstack.index = index;
	newstack.size = 0;

	for (int i = index; i >= 0; i--) {
		card_i = deck_select();
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

	tableau[index] = newstack;
}

int tableau_contains(MEVENT event, int i) {
	return contains(
		event.x,
		event.y,
		TABLEAU_X + STACK_SPACING * (i + 1) + CARD_WIDTH * i,
		TABLEAU_Y,
		TABLEAU_X + STACK_SPACING * (i + 1) + CARD_WIDTH * (i + 1),
		TABLEAU_Y + tableau[i].size + (CARD_HEIGHT - 1)
	);
}

void draw_tableau(int stack_i) {
	card_t *card_i;
	int card_count;

	card_i = tableau[stack_i].bottom;
	card_count = 0;

	int x = TABLEAU_X + STACK_SPACING * (stack_i + 1) + CARD_WIDTH * stack_i;
	int y = TABLEAU_Y + card_count;

	if (card_i == NULL) {
		draw_empty_stack(x, y);
		return;
	}

	while (card_i != NULL) {
		draw_card(*card_i, x, y);
		card_i = card_i->next;
		card_count++;
		y++;
	}
}
