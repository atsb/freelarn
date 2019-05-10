/*  Copyright 2017 Gibbon aka 'atsb'
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "FLTerminal.hpp"

void
ansiterm_init(void) {
	initscr();
	cbreak();
	noecho();
	nonl();
	intrflush(stdscr, 0);
	keypad(stdscr, 1);
	curs_set(0);
	#if defined WINDOWS || WINDOWS_VS
	PDC_save_key_modifiers(1);
	#endif
}

void
ansiterm_clean_up(void) {
	nocbreak();
	nl();
	echo();
	endwin();
}


void
ansiterm_command(int ansi_cmd, const char *param1,
                 const char *param2) {
	if (ansi_cmd == 'H') {
		int y, x;
		if (*param1 == '\0') {
			y = 0;
		} else {
			y = std::stoi(param1) - 1;
		}
		if (*param2 == '\0') {
			x = 0;
		} else {
			x = std::stoi(param2) - 1;
		}
		move(y, x);
	} else if (ansi_cmd == 'J') {
		clear();
	} else if (ansi_cmd == 'M') {
		int i, n_lines;
		if (*param1 != '\0') {
			n_lines = 1;
		} else {
			n_lines = std::stoi(param1);
		}
		for (i = 0; i < n_lines; i++) {
			move(0, 0);
			clrtoeol();
		}
	} else if (ansi_cmd == 'K') {
		clrtoeol();
	} else if (ansi_cmd == 'm') {
		int attribute;
		if (*param1 == '\0') {
			attribute = 0;
		} else {
			attribute = std::stoi(param1);
		}
		if (attribute == 0) {
			attrset(A_NORMAL);
		} else if (attribute == 1) {
			attrset(A_REVERSE);
		}
	} else {
		return;
	}
}

