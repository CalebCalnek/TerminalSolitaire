#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "card.h"

extern cardstack_t talon;
extern cardstack_t wastepile;
extern cardstack_t foundations[4];
extern cardstack_t tableau[7];

#define MAX_CMD 8

int show_cmdbar;
char command[MAX_CMD];
int cmd_len;

void init_cmdbar() {
	show_cmdbar = 0;
	cmd_len = 0;
}

void draw_cmdbar() {
	int i, col_end, row_end;

	if (!show_cmdbar) return;

	getmaxyx(stdscr, row_end, col_end);
	attron(COLOR_PAIR(5));
	for (i = 0; i < col_end; i++) {
		mvaddstr(row_end - 1, i, " ");
	}
	mvaddch(row_end - 1, 0, ':');
	mvaddstr(row_end - 1, 1, command);
	attroff(COLOR_PAIR(5));
}

void extract_value(regmatch_t match, char *buffer) {
	int start, end;
	start = match.rm_so;
	end = match.rm_eo;
	snprintf(buffer, end - start + 1, "%s", command + start);
}

void exec_cmd(char *args[]) {
	enum rank src_rank = str_to_rank(args[0]);
	enum suit src_suit = str_to_suit(args[1]);
	cardstack_t src_card = (cardstack_t) { NULL, NULL, 0, 0 };
	if (wastepile.size > 0
	&& compare_card(*wastepile.top, src_rank, src_suit)) {
		src_card = (cardstack_t) { wastepile.top, wastepile.top, 1, -1 };
	} else {
		int i, j;
		card_t *card_i;
		for (i = 0; i < 7; i++) {
			if (tableau[i].size == 0) continue;
			card_i = tableau[i].top;
			j = tableau[i].size - 1;
			while (card_i != NULL) {
				if (compare_card(*card_i, src_rank, src_suit)) {
					if (card_i->face != DOWN) {
						src_card = (cardstack_t) {
							tableau[i].top,
							card_i,
							tableau[i].size - j,
							i
						};
					}
				}
				card_i = card_i->prev;
				j--;
			}
		}
	}
	if (src_card.size == 0) return;

	cardstack_t *dst_stack = NULL;
	int dst_i = -1;

	/* assume auto-move */
	int i;
	for (i = 0; i < 7; i++) {
		if (i == src_card.index) continue;
		if (can_move(&tableau[i], *src_card.bottom, i)) {
			dst_stack = tableau;
			dst_i = i;
			break;
		}
	}
	for (i = 0; i < 4; i++) {
		if (dst_stack != NULL && args[2][0] != 'f') break;
		if (can_move(&foundations[i], *src_card.bottom, i)) {
			dst_stack = foundations;
			dst_i = i;
			break;
		}
	}
	if (dst_stack == NULL) return;

	/* set dst manually if requested */
	switch (args[2][0]) {
		case 't': dst_stack = tableau; break;
		case 'f': dst_stack = foundations; break;
		case '\0': break;
		default: return;
	}
	if (args[3][0] != '\0') {
		dst_i = strtol(args[3], NULL, 10) - 1;
	}
	if (dst_stack == foundations && dst_i > 3) return;

	if (can_move(&dst_stack[dst_i], *src_card.bottom, dst_i)) {
		move_card(&dst_stack[dst_i], &src_card);
	}
}

void parse_cmd() {
	regex_t regex;
	regmatch_t matches[5];
	char *pattern = "^([2-9]|10|[ajqk])([cdhs]) ?(t|f)?([1-7])?$";
	regcomp(&regex, pattern, REG_EXTENDED);
	if (regexec(&regex, command, 5, matches, 0) == 0) {
		char src_rank[3], src_suit[2], dst_stack[2], dst_i[2];

		extract_value(matches[1], src_rank);
		extract_value(matches[2], src_suit);
		extract_value(matches[3], dst_stack);
		extract_value(matches[4], dst_i);

		char *exec_args[] = { src_rank, src_suit, dst_stack, dst_i };
		exec_cmd(exec_args);
	}

	regfree(&regex);
}

int handle_keyboard(char ch) {
	if (!show_cmdbar) {
		if (ch == ':') {
			show_cmdbar = 1;
		}
		return 0;
	}

	if (ch == 7 || ch == 263 || ch == KEY_BACKSPACE) {
		/* detect backspace */
		if (cmd_len > 0) {
			command[--cmd_len] = '\0';
		} else {
			show_cmdbar = 0;
		}
	} else if (ch == '\n')  {
		/* detect enter */
		if (strcmp(command, "q") == 0) {
			/* detect quit command */
			return 1;
		} else if (strcmp(command, "talon") == 0 || strcmp(command, "t") == 0) {
			/* draw from talon */
			pop_talon();
		} else {
			parse_cmd();
		}
		memset(command, '\0', MAX_CMD);
		cmd_len = 0;
		show_cmdbar = 0;
	} else if (cmd_len < MAX_CMD - 1) {
		/* record character */
		command[cmd_len++] = ch;
	}

	return 0;
}
