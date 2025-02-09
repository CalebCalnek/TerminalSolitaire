#include <curses.h>
#include <stdlib.h>
#include "card.h"

cardstack_t *held;

extern cardstack_t talon;
extern cardstack_t wastepile;
extern cardstack_t foundations[4];
extern cardstack_t tableau[7];

void set_held(card_t *top, card_t *bottom, int size, int i) {
	held = (cardstack_t *) malloc(sizeof(cardstack_t));
	*held = (cardstack_t) { top, bottom, size, i };
}

void handle_mouse_event(MEVENT event) {
	int i, j;
	card_t *card_i;

	if (event.bstate & BUTTON1_PRESSED) {
		/* check click talon*/
		if (talon_contains(event)) {
			pop_talon();
			return;
		}

		/* check click waste */
		if (wastepile.size > 0 && wastepile_contains(event)) {
			set_held(wastepile.top, wastepile.top, 1, -1);
		}

		/* check click tableau */
		for (i = 0; i < 7; i++) {
			if (tableau[i].size == 0) continue;
			if (tableau_contains(event, i)) {
				card_i = tableau[i].top;
				j = tableau[i].size - 1;
				while (card_i != NULL) {
					if (event.y >= TABLEAU_Y + j &&
					event.y < TABLEAU_Y + j + CARD_HEIGHT) {
						break;
					}
					card_i = card_i->prev;
					j--;
				}
				if (card_i->face != DOWN) {
					set_held(tableau[i].top, card_i, tableau[i].size - j, i);
				}
				return;
			}
		}
	} else if (event.bstate & BUTTON1_RELEASED && held != NULL) {
		for (i = 0; i < 7; i++) {
			// skip check for drop in same stack
			if (i == held->index) continue;

			if (tableau_contains(event, i)) {
				if (can_move(&tableau[i], *held->bottom, i)) {
					move_card(&tableau[i], held);
				}
				free(held);
				held = NULL;
				return;
			}
		}

		for (i = 0; i < 4; i++) {
			if (foundation_contains(event, i)) {
				if (can_move(&foundations[i], *held->bottom, i)) {
					move_card(&foundations[i], held);
				}
				free(held);
				held = NULL;
				return;
			}
		}

		free(held);
		held = NULL;
	}
}
