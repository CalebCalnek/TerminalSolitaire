#include <curses.h>
#include <string.h>
#include "card.h"

extern int show_cmdbar;
extern char command[MAX_CMD];
extern int cmd_len;

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
