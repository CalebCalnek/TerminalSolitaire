#ifndef CARD_H
#define CARD_H

#define CARD_WIDTH 7
#define CARD_HEIGHT 5
#define STACK_SPACING 3

#define TALON_X 0
#define TALON_Y 1

#define WASTE_X (STACK_SPACING + CARD_WIDTH)
#define WASTE_Y 1

#define FOUNDATION_X (3 * (STACK_SPACING + CARD_WIDTH))
#define FOUNDATION_Y 1

#define TABLEAU_X 0
#define TABLEAU_Y (TALON_Y + CARD_HEIGHT + 1)

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

void move_card(struct cardstack *dst, struct card *held, struct card *held_top, int held_i, int held_size);

struct card *init_card(int suit, int rank);
int mouse_inbounds(int mx, int my, int x1, int y1, int x2, int y2);
int contact_card(MEVENT event, struct card card);
void draw_card(struct card card, int x, int y);

struct cardstack init_stack(int index);
struct cardstack init_talon();
void reset_talon();
void draw_stack(struct cardstack stack);
void draw_talon();
void draw_waste();
void draw_foundations(int stack_i);

#endif
