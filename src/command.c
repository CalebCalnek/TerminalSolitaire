#include <curses.h>
#include <string.h>
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
