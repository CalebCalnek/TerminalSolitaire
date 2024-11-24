#ifndef CARD_H
#define CARD_H

#define CARD_WIDTH 7
#define CARD_HEIGHT 5

enum suit {
	CLUBS,
	SPADES,
	HEARTS,
	DIAMONDS
};

enum rank {
	ACE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,
};

struct card {
	enum suit suit;
	enum rank rank;
	int x, y;
};

struct cardstack {
	struct node *top, *bottom;
	int size;
	int index;
};

struct node {
	struct card value;
	struct node *next;
};

void draw_card(struct card card);

struct cardstack init_stack();
void push(struct cardstack basestack, struct cardstack newstack);
void draw_stack(struct cardstack stack);

#endif
