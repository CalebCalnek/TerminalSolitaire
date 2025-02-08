#include <curses.h>
#include <stdlib.h>
#include "card.h"

cardstack_t foundations[4];

int foundation_contains(MEVENT event, int i) {
	return contains(
		event.x,
		event.y,
		FOUNDATION_X + STACK_SPACING * (i + 1) + CARD_WIDTH * i,
		FOUNDATION_Y,
		FOUNDATION_X + STACK_SPACING * (i + 1) + CARD_WIDTH * (i + 1),
		FOUNDATION_Y + CARD_HEIGHT
	);
}

void draw_foundations(int stack_i) {
	int x = FOUNDATION_X + STACK_SPACING * (stack_i+1) + CARD_WIDTH * stack_i;
	int y = FOUNDATION_Y;

	if (foundations[stack_i].size == 0) {
		draw_empty_stack(x, y);
		return;
	}

	draw_card(*foundations[stack_i].top, x, y);
}
