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

void push(struct cardstack basestack, struct cardstack newstack) {
	basestack.top->next = newstack.bottom;
	basestack.top = newstack.top;
}

void draw_empty_stack(int stack_i, int card_count) {
	int i;

	int x = TABLEAU_X + STACK_SPACING * (stack_i + 1) + CARD_WIDTH * stack_i;
	int y = TABLEAU_Y + card_count;
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

void draw_talon() {
	int i;

	int x = TALON_X + STACK_SPACING;
	int y = TALON_Y;
	int end_x = CARD_WIDTH - 1;
	int end_y = CARD_HEIGHT - 1;

	if (talon.size > 0) {
		attron(COLOR_PAIR(4));
		char *fill_char = "▒";
		for (int i = 0; i < CARD_HEIGHT; i++) {
			for (int j = 0; j < CARD_WIDTH; j++) {
				mvaddstr(y + i, x + j, fill_char);
			}
		}
		attroff(COLOR_PAIR(2));
		return;
	}

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

void draw_waste() {
	int i;

	int x = WASTE_X + STACK_SPACING;
	int y = WASTE_Y;

	int end_x = CARD_WIDTH - 1;
	int end_y = CARD_HEIGHT - 1;

	if (wastepile.size > 0) {
		struct card *card = wastepile.top;

		if (card->rank == TEN) {
			end_x = CARD_WIDTH - 2;
		}

		int mid_x = CARD_WIDTH / 2;
		int mid_y = CARD_HEIGHT / 2;

		char *suit = suit_chars[card->suit];
		char *rank = rank_chars[card->rank];

		attron(COLOR_PAIR(card->suit & 2 ? 3 : 2));
		char *fill_char = " ";
		for (int i = 0; i < CARD_HEIGHT; i++) {
			for (int j = 0; j < CARD_WIDTH; j++) {
				mvaddstr(y + i, x + j, fill_char);
			}
		}

		mvaddstr(y, x, rank);
		mvaddstr(y + mid_y, x + mid_x, suit);
		mvaddstr(y + end_y, x + end_x, rank);

		attroff(COLOR_PAIR(2));
		return;
	}

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

void draw_foundations(int stack_i) {
	int i;

	int x = FOUNDATION_X + STACK_SPACING * (stack_i+1) + CARD_WIDTH * stack_i;
	int y = FOUNDATION_Y;
	int end_x = CARD_WIDTH - 1;
	int end_y = CARD_HEIGHT - 1;

	if (foundations[stack_i].size > 0) {
		struct card *card = foundations[stack_i].top;

		if (card->rank == TEN) {
			end_x = CARD_WIDTH - 2;
		}

		int mid_x = CARD_WIDTH / 2;
		int mid_y = CARD_HEIGHT / 2;

		char *suit = suit_chars[card->suit];
		char *rank = rank_chars[card->rank];

		attron(COLOR_PAIR(card->suit & 2 ? 3 : 2));
		char *fill_char = " ";
		for (int i = 0; i < CARD_HEIGHT; i++) {
			for (int j = 0; j < CARD_WIDTH; j++) {
				mvaddstr(y + i, x + j, fill_char);
			}
		}

		mvaddstr(y, x, rank);
		mvaddstr(y + mid_y, x + mid_x, suit);
		mvaddstr(y + end_y, x + end_x, rank);

		attroff(COLOR_PAIR(2));
		return;
	}

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
