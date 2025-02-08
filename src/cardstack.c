#include <curses.h>
#include <stdlib.h>
#include "card.h"

struct cardstack talon;
struct cardstack wastepile;
/* struct cardstack foundations[4]; */
/* struct cardstack tableau[7]; */

void init_talon() {
	struct cardstack newstack;
	struct card *card_i;

	newstack.top = NULL;
	newstack.index = -1;
	newstack.size = 0;

	while ((card_i = deck_select()) != NULL) {
		card_i->face = DOWN;
		if (newstack.top != NULL) {
			card_i->prev = newstack.top;
			newstack.top->next = card_i;
		} else {
			newstack.bottom = card_i;
		}
		newstack.top = card_i;
		newstack.size++;
	}

	talon = newstack;
}

void reset_talon() {
	while (wastepile.top != NULL) {
		struct cardstack card = { wastepile.top, wastepile.top, 1, -1 };
		move_card(&talon, &card);
		talon.top->face = DOWN;
	}
}

void pop_talon() {
	if (talon.size == 0) {
		reset_talon();
		return;
	}
	struct cardstack card = { talon.top, talon.top, 1, -1 };
	move_card(&wastepile, &card);
}

int talon_contains(MEVENT event) {
	return contains(
		event.x,
		event.y,
		TALON_X + STACK_SPACING,
		TALON_Y,
		TALON_X + STACK_SPACING + CARD_WIDTH,
		TALON_Y + CARD_HEIGHT
	);
}

int wastepile_contains(MEVENT event) {
	return contains(
		event.x,
		event.y,
		WASTE_X + STACK_SPACING,
		WASTE_Y,
		WASTE_X + STACK_SPACING + CARD_WIDTH,
		WASTE_Y + CARD_HEIGHT
	);
}

void draw_empty_stack(int x, int y) {
	int i;
	int end_x = CARD_WIDTH - 1;
	int end_y = CARD_HEIGHT - 1;

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

void draw_talon() {
	int x = TALON_X + STACK_SPACING;
	int y = TALON_Y;

	if (talon.size == 0) {
		draw_empty_stack(x, y);
		return;
	}

	draw_card(*talon.top, x, y);
}

void draw_waste() {
	int x = WASTE_X + STACK_SPACING;
	int y = WASTE_Y;

	if (wastepile.size == 0) {
		draw_empty_stack(x, y);
		return;
	}

	draw_card(*wastepile.top, x, y);
}
