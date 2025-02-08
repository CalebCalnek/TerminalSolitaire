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
	struct card *next, *prev;
	int face;
};

struct cardstack {
	struct card *top, *bottom;
	int size;
	int index;
};

/* card */
struct card *init_card(int suit, int rank);
int can_move(struct cardstack *dst, struct card card, int index);
void move_card(struct cardstack *dst, struct cardstack *mv_stack);
int contains(int mx, int my, int x1, int y1, int x2, int y2);
void draw_card(struct card card, int x, int y);

/* cardstack */
void init_talon();
void reset_talon();
void pop_talon();
int talon_contains(MEVENT event);
int wastepile_contains(MEVENT event);
void draw_empty_stack(int x, int y);
void draw_talon();
void draw_waste();

/* foundations */
int foundation_contains(MEVENT event, int i);
void draw_foundations(int stack_i);

/* tableau */
void init_tableau(int index);
int tableau_contains(MEVENT event, int i);
void draw_tableau(int stack_i);

/* deck */
void init_deck(void);
struct card *deck_select();

/* mouse */
void set_held(struct card *top, struct card *bottom, int size, int i);
void handle_mouse_event(MEVENT event);

/* command */
void init_cmdbar();
void draw_cmdbar();
int handle_keyboard(char ch);

#endif
