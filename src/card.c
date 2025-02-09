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

int compare_card(card_t card1, enum rank rank2, enum suit suit2) {
	return (card1.rank == rank2) && (card1.suit == suit2);
}

int can_move(cardstack_t *dst, card_t card, int index) {
	if (dst == &foundations[index]) {
		if (card.next != NULL) return 0;
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

enum rank str_to_rank(char *rank_str) {
	switch (rank_str[0]) {
		case 'a': return ACE; break;
		case '2': return TWO; break;
		case '3': return THREE; break;
		case '4': return FOUR; break;
		case '5': return FIVE; break;
		case '6': return SIX; break;
		case '7': return SEVEN; break;
		case '8': return EIGHT; break;
		case '9': return NINE; break;
		case '1': return TEN; break;
		case 'j': return JACK; break;
		case 'q': return QUEEN; break;
		case 'k': return KING; break;
		default: return -1;
	}
}

enum suit str_to_suit(char *suit_str) {
	switch (suit_str[0]) {
		case 'c': return CLUBS; break;
		case 's': return SPADES; break;
		case 'h': return HEARTS; break;
		case 'd': return DIAMONDS; break;
		default: return -1;
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
