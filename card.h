#ifndef CARD_H
#define CARD_H

#define CARD_WIDTH 7
#define CARD_HEIGHT 5
#define STACK_SPACING 3

#define DOWN 0
#define UP 1

#define RAND(n) (rand() % (n))

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
	struct card *next, *prev;
	int face;
};

struct cardstack {
	struct card *top, *bottom;
	int size;
	int index;
};

struct card *init_card(int suit, int rank);
int mouse_inbounds(int mx, int my, int x1, int y1, int x2, int y2);
int contact_card(MEVENT event, struct card card);
void draw_card(struct card card, int stack_i, int card_count);

struct cardstack init_stack(int index);
void push(struct cardstack basestack, struct cardstack newstack);
void draw_stack(struct cardstack stack);

#endif
