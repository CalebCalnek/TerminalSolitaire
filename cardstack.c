#include <curses.h>
#include <stdlib.h>
#include "card.h"

extern struct card *deck[52];
extern int deck_size;

extern struct cardstack talon;
extern struct cardstack wastepile;
extern struct cardstack foundations[4];

extern char *suit_chars[4];
extern char *rank_chars[13];

struct card *deck_select() {
	int rand_index = RAND(deck_size);
	struct card *card_i = deck[rand_index];
	deck[rand_index] = deck[deck_size - 1];
	deck[deck_size - 1] = NULL;
	deck_size--;
	return card_i;
}

struct cardstack init_stack(int index) {
	struct cardstack newstack;

	newstack.top = NULL;
	newstack.index = index;
	newstack.size = 0;

	for (int i = index; i >= 0; i--) {
		struct card *card_i = deck_select();
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

	newstack.top = NULL;
	newstack.index = -1;
	newstack.size = 0;

	while (deck[0] != NULL) {
		struct card *card_i = deck_select();
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
		move_card(&talon, wastepile.top, wastepile.top, -1, 1);
		talon.top->face = DOWN;
	}
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
