#include <curses.h>
#include <stdlib.h>
#include "card.h"

extern struct cardstack talon;
extern struct cardstack wastepile;
extern struct cardstack foundations[4];
extern struct cardstack tableau[7];

extern char *suit_chars[4];
extern char *rank_chars[13];

struct cardstack init_stack(int index) {
	struct cardstack newstack;
	struct card *card_i;

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

	return newstack;
}

struct cardstack init_talon() {
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

	return newstack;
}

void reset_talon() {
	while (wastepile.top != NULL) {
		struct cardstack card = { wastepile.top, wastepile.top, 1, -1 };
		move_card(&talon, &card);
		talon.top->face = DOWN;
	}
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

void draw_stack(struct cardstack stack) {
	struct card *card_i;
	int card_count, stack_i;

	card_i = stack.bottom;
	stack_i = stack.index;
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

void draw_foundations(int stack_i) {
	int x = FOUNDATION_X + STACK_SPACING * (stack_i+1) + CARD_WIDTH * stack_i;
	int y = FOUNDATION_Y;

	if (foundations[stack_i].size == 0) {
		draw_empty_stack(x, y);
		return;
	}

	draw_card(*foundations[stack_i].top, x, y);
}
