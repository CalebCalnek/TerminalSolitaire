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
	char *src_rank = args[0];
	char *src_suit = args[1];
	char *dst_stack = args[2];
	int dst_i = strtol(args[3], NULL, 10) - 1;
	if (can_move(&tableau[dst_i], *wastepile.top, dst_i)) {
		cardstack_t card;
		card.top = wastepile.top;
		card.bottom = wastepile.top;
		card.size = 1;
		card.index = -1;
		move_card(&tableau[dst_i], &card);
	}
}

void parse_cmd() {
	regex_t regex;
	regmatch_t matches[5];
	char *pattern = "^([2-9]|10|[ajqk])([cdhs]) (t|f)([1-7])$";
	regcomp(&regex, pattern, REG_EXTENDED);
	if (regexec(&regex, command, 5, matches, 0) == 0) {
		char src_rank[3], src_suit[2], dst_stack[2], dst_i[2];

		extract_value(matches[1], src_rank);
		extract_value(matches[2], src_suit);
		extract_value(matches[3], dst_stack);
		extract_value(matches[4], dst_i);

		char *exec_args[] = { src_rank, src_suit, dst_stack, dst_i };
		exec_cmd(exec_args);
		/* printf("\n%s%s%s%s\n", src_rank, src_suit, dst_stack, dst_i); */
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
		} else if (strcmp(command, "talon") == 0) {
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
