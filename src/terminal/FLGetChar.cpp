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

#include "FLGetChar.hpp"

static int llgetch(void);

static int
llgetch(void) {
	int key;
	key = wgetch(stdscr);
	#ifdef WINDOWS
	if (PDC_get_key_modifiers() & PDC_KEY_MODIFIER_SHIFT) {
		switch (key) {
			case '1':
				return 'B';
			case '2':
				return 'J';
			case '3':
				return 'N';
			case '4':
				return 'H';
			case '5':
				return '.';
			case '6':
				return 'L';
			case '7':
				return 'Y';
			case '8':
				return 'K';
			case '9':
				return 'U';
		}
	}
	#endif
	switch (key) {
		case KEY_UP:
			return 'k';
		case KEY_DOWN:
			return 'j';
		case KEY_LEFT:
			return 'h';
		case KEY_RIGHT:
			return 'l';
			#ifdef WINDOWS
		case KEY_A2:
			return 'k';
		case KEY_B1:
			return 'h';
		case KEY_B3:
			return 'l';
		case KEY_C2:
			return 'j';
		case PADENTER:
			return 13;
			#endif
		case KEY_A1:
			return 'y';
		case KEY_A3:
			return 'u';
		case KEY_C1:
			return 'b';
		case KEY_C3:
			return 'n';
		case KEY_B2:
			return '.';
		case KEY_ENTER:
			return 13;
		default:
			return key;
	}
}

/*
    get char (with echo)
*/
int
ansiterm_getche(void) {
	int key;
	echo();
	key = llgetch();
	noecho();
	return key;
}

/*
    get char
*/
int
ansiterm_getch(void) {
	return llgetch();
}


/* wgetch() is the modern way. -Gibbon */

void
ansiterm_delch(void) {
	delch();
}
