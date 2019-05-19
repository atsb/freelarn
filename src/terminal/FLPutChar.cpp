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

#include "FLPutChar.hpp"

void
ansiterm_putchar(int c) {
	if (c == '\n') {
		int y, x;
		getyx(stdscr, y, x);
		move(y + 1, 0);
		return;
	}
	if (c == '\t') {
		addstr("    ");
		return;
	}
	addch(c);
}

void
llrefresh(void) {
	refresh();
}
