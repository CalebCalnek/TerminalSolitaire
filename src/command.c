#include <curses.h>
#include <string.h>
#include <regex.h>
#include "card.h"

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

void mv_cmd() {
	regex_t regex;
	regmatch_t matches[5];
	char *pattern = "^([2-9]|10|[ajqk])([cdhs]) (t|f)([1-7])$";
	regcomp(&regex, pattern, REG_EXTENDED);
	if (regexec(&regex, command, 5, matches, 0) == 0) {
		char src_rank[3];
		char src_suit[2];
		char dst_stack[2];
		char dst_i[2];
		int start, end;

		start = matches[1].rm_so;
		end = matches[1].rm_eo;
		snprintf(src_rank, end - start + 1, "%s", command + start);

		start = matches[2].rm_so;
		end = matches[2].rm_eo;
		snprintf(src_suit, end - start + 1, "%s", command + start);

		start = matches[3].rm_so;
		end = matches[3].rm_eo;
		snprintf(dst_stack, end - start + 1, "%s", command + start);

		start = matches[4].rm_so;
		end = matches[4].rm_eo;
		snprintf(dst_i, end - start + 1, "%s", command + start);
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
			mv_cmd();
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
