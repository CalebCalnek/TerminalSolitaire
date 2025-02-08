#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include "card.h"

char *suit_chars[4] = { "♣", "♠", "♥", "♦" };
char *rank_chars[13] = { "A", "2", "3", "4", "5", "6", "7",
	"8", "9", "10", "J", "Q", "K" };

extern cardstack_t talon;
extern cardstack_t wastepile;
extern cardstack_t foundations[4];
extern cardstack_t tableau[7];

card_t *init_card(int suit, int rank) {
	card_t *newcard;
	newcard = (card_t *) malloc(sizeof(card_t));
	newcard->suit = suit;
	newcard->rank = rank;
	return newcard;
}

int can_move(cardstack_t *dst, card_t card, int index) {
	if (dst == &foundations[index]) {
		if (dst->size == 0) return card.rank == ACE;
		return card.suit == dst->top->suit && card.rank == dst->top->rank + 1;
	} else if (dst == &tableau[index]) {
		if (dst->size == 0) return card.rank == KING;
		return (card.suit & 2) != (dst->top->suit & 2) &&
			card.rank == dst->top->rank - 1;
	}

	return -1;
}

void move_card(cardstack_t *dst, cardstack_t *mv_stack) {
	card_t *src_top = mv_stack->bottom->prev;
	cardstack_t *src;

	if (mv_stack->top == talon.top) {
		src = &talon;
	} else if (mv_stack->top == wastepile.top) {
		src = &wastepile;
	} else if (mv_stack->top == tableau[mv_stack->index].top) {
		src = &tableau[mv_stack->index];
	}

	if (dst->size > 0) {
		dst->top->next = mv_stack->bottom;
		mv_stack->bottom->prev = dst->top;
	} else {
		dst->bottom = mv_stack->bottom;
		mv_stack->bottom->prev = NULL;
	}
	dst->top = mv_stack->top;
	dst->size += mv_stack->size;

	src->top = src_top;
	src->size -= mv_stack->size;
	if (src == &talon) {
		dst->top->face = UP;
	} else if (src == &tableau[mv_stack->index] && src->top != NULL) {
		src->top->face = UP;
	}
	if (src->size > 0) {
		src->top->next = NULL;
	} else {
		src->bottom = NULL;
	}
}

int contains(int mx, int my, int x1, int y1, int x2, int y2) {
	return mx >= x1 && mx < x2 && my >= y1 && my < y2;
}

void draw_card(card_t card, int x, int y) {
	char *suit = suit_chars[card.suit];
	char *rank = rank_chars[card.rank];

	int end_x = card.rank == TEN ? CARD_WIDTH - 2 : CARD_WIDTH - 1;
	int end_y = CARD_HEIGHT - 1;
	int mid_x = CARD_WIDTH / 2;
	int mid_y = CARD_HEIGHT / 2;

	char *fill_char;

	if (card.face == UP) {
		fill_char = " ";
		attron(COLOR_PAIR(card.suit & 2 ? 3 : 2));
	} else {
		fill_char = "▒";
		attron(COLOR_PAIR(4));	/* face down, color blue */
	}

	// draw blank card
	for (int i = 0; i < CARD_HEIGHT; i++) {
		for (int j = 0; j < CARD_WIDTH; j++) {
			mvaddstr(y + i, x + j, fill_char);
		}
	}

	// draw symbols
	if (card.face == UP) {
		mvaddstr(y, x, rank);
		if (card.next != NULL) {
			/* card is coverd, draw second suit symbol */
			mvaddstr(y, x + mid_x + 2, suit);
		}
		mvaddstr(y + mid_y, x + mid_x, suit);
		mvaddstr(y + end_y, x + end_x, rank);
	}

	attroff(COLOR_PAIR(2));
}
