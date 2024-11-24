#include <stdlib.h>
#include "card.h"

void push(struct cardstack basestack, struct cardstack newstack) {
	basestack.top->next = newstack.bottom;
	basestack.top = newstack.top;
}

void draw_stack(struct cardstack stack) {
	struct node *i;
	i = stack.top;
	while (i != NULL) {
		draw_card(*i);
	}
}
