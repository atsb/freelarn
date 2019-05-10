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

#include "../player/FLPlayerActions.hpp"
#include "../config/FLIncludeData.hpp"
#include "../templates/FLTMath.hpp"
#include "FLDungeon.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../core/FLInventory.hpp"
#include "../core/FLTermCapIO.hpp"
#include "../core/FLStore.hpp"
#include "../core/FLFuncs.hpp"

static void fch(int, long *);
static void move_cursor(int *, int *, int);

/*
    subroutine to process a throne object
*/
void
fl_extra_object_func_throne(int arg) {
	fl_display_message("\nDo you (p) pry off jewels, (s) sit down");
	fl_display_message(", or (i) ignore it? ");
	for (;;) {
		for (;;) {
			switch (ttgetch()) {
				case 'p':
					fl_display_message(" pry off");
					fl_a_remove_the_gems(arg);
					return;
				case 's':
					fl_display_message(" sit down");
					fl_a_sit_on_throne(arg);
					return;
				case 'i':
				case '\33':
					fl_display_message("ignore\n");
					return;
			};
		}
	}
}

/*
    a subroutine to raise or lower character levels
    if x > 0 they are raised   if x < 0 they are lowered
*/
void
fl_change_char_level(int how) {
	FLCoreFuncs CoreFuncs;
	int j;
	fl_output_single_byte_to_output_buffer('\n');
	switch (TRnd(9)) {
		case 1:
			fl_display_message("Your strength");
			fch(how, &cdesc[FL_STRENGTH]);
			break;
		case 2:
			fl_display_message("Your intelligence");
			fch(how, &cdesc[FL_INTELLIGENCE]);
			break;
		case 3:
			fl_display_message("Your wisdom");
			fch(how, &cdesc[FL_WISDOM]);
			break;
		case 4:
			fl_display_message("Your constitution");
			fch(how, &cdesc[FL_CONSTITUTION]);
			break;
		case 5:
			fl_display_message("Your dexterity");
			fch(how, &cdesc[FL_DEXTERITY]);
			break;
		case 6:
			fl_display_message("Your charm");
			fch(how, &cdesc[FL_CHARISMA]);
			break;
		case 7:
			j = TRnd(level + 1);
			if (how < 0) {
				lprintf("You lose %d hit point", (int) j);
				if (j > 1) {
					fl_display_message("s!");
				} else {
					fl_output_single_byte_to_output_buffer('!');
				}
				FL_LOSEMAXHEALTH((int) j);
			} else {
				lprintf("You gain %d hit point", (int) j);
				if (j > 1) {
					fl_display_message("s!");
				} else {
					fl_output_single_byte_to_output_buffer('!');
				}
				FL_RAISEMAXHEALTH((int)j);
			}
			bottomline();
			break;
		case 8:
			j = TRnd(level + 1);
			if (how > 0) {
				lprintf("You just gained %d spell", (int) j);
				FL_RAISEMAXHEALTH((int)j);
				if (j > 1) {
					fl_display_message("s!");
				} else {
					fl_output_single_byte_to_output_buffer('!');
				}
			} else {
				lprintf("You just lost %d spell", (int) j);
				FL_LOSEMAXSPELLS((int) j);
				if (j > 1) {
					fl_display_message("s!");
				} else {
					fl_output_single_byte_to_output_buffer('!');
				}
			}
			bottomline();
			break;
		case 9:
			j = 5 * TRnd((level + 1) * (level + 1));
			if (how < 0) {
				lprintf("You just lost %d experience point", (int) j);
				if (j > 1) {
					fl_display_message("s!");
				} else {
					fl_output_single_byte_to_output_buffer('!');
				}
				CoreFuncs.DecreaseExperience(j);
			} else {
				lprintf("You just gained %d experience point", (int) j);
				if (j > 1) {
					fl_display_message("s!");
				} else {
					fl_output_single_byte_to_output_buffer('!');
				}
				CoreFuncs.IncreaseExperience(j);
			}
			break;
	}
	fl_termcap_cursor_position(1, 24);
}



/*
    subroutine to process an up/down of a character attribute for ofountain
*/
static void
fch(int how, long *x) {
	if (how < 0) {
		fl_display_message(" went down by one!");
		-- (*x);
	} else {
		fl_display_message(" went up by one!");
		(*x)++;
	}
	bottomline();
}

/*
    For command mode.  Perform opening an object (door, chest).
*/
void
fl_open_an_object(void) {
	int x, y;			/* direction to open */
	char tempc;			/* result of prompting to open a chest */
	fl_termcap_cursor_position(1, 24);
	/*  check for confusion.
	*/
	if (cdesc[FL_CONFUSE]) {
		fl_display_message("You're too confused!");
		return;
	}
	/*  check for player standing on a chest.  If he is, prompt for and
	    let him open it.  If player ESCs from prompt, quit the Open
	    command.
	*/
	if (object_identification[player_horizontal_position][player_vertical_position] == OCHEST) {
		fl_display_message("There is a chest here.  Open it?");
		if ((tempc = getyn()) == 'y') {
			fl_a_open_chest(player_horizontal_position, player_vertical_position);
			dropflag =
			    1;		/* prevent player from picking back up if fail */
			return;
		} else if (tempc != 'n') {
			return;
		}
	}
	/*  get direction of object to open.  test 'openability' of object
	    indicated, call common command/prompt mode routines to actually open.
	*/
	fl_direction(&x, &y);
	switch (object_identification[x][y]) {
		case OOPENDOOR:
			fl_display_message("The door is already open!");
			break;
		case OCHEST:
			fl_a_open_chest(x, y);
			break;
		case OCLOSEDDOOR:
			fl_a_open_dungeon_door(x, y);
			break;
		/* This message is rephrased to handle other scenarios. -Gibbon */
		default:
			fl_display_message("\nNothing happens..");
			break;
	}
}




/*
    For command mode.  Perform the action of closing fl_create_rand_object_around_player_locationdoor).
*/
void
fl_close_an_object(void) {
	int x, y;
	fl_termcap_cursor_position(1, 24);
	/*  check for confusion.
	*/
	if (cdesc[FL_CONFUSE]) {
		fl_display_message("You're too confused!");
		return;
	}
	/*  get direction of object to close.  test 'closeability' of object
	    indicated.
	*/
	fl_direction(&x, &y);
	switch (object_identification[x][y]) {
		case OCLOSEDDOOR:
			fl_display_message("The door is already closed!");
			break;
		case OOPENDOOR:
			if (monster_identification[x][y]) {
				fl_display_message("Theres a monster in the way!");
				return;
			}
			object_identification[x][y] = OCLOSEDDOOR;
			been_here_before[x][y] = 0;
			object_argument[x][y] = 0;
			break;
		default:
			fl_display_message("You can't close that!");
			break;
	}
}

/*  perform the actions of identifying the object/monster associated with a
    character typed by the user.  assumes cursors().
*/
void
specify_obj_nocurs(void) {
	char i;
	int j, flag;
	fl_display_message("\nType object character:");
	switch (i = ttgetch()) {
		case '\33':
		case '\n':
			return;
		case '@':
			lprintf("\n@: %s", logname);
			return;
		case ' ':
			lprintf("\n : An as-yet-unseen place in the dungeon");
			return;
		default:
			if (i == floorc) {
				fl_output_single_byte_to_output_buffer('\n');
				fl_output_single_byte_to_output_buffer(floorc);
				lprintf(": the floor of the dungeon");
				return;
			}
			flag = 0;
			for (j = 0; j < FL_MAX_MONSTERS_IN_GAME + 8; j++)
				if (i == monstnamelist[j]) {
					lprintf("\n%c: %s", i, monster[j].name);
					flag = 1;
				}
			/*  check for spurious object character
			*/
			if (i != '_')
				for (j = 0; j < MAXOBJECT; j++)
					if (i == objnamelist[j]) {
						fl_output_single_byte_to_output_buffer('\n');
						fl_output_single_byte_to_output_buffer(i);
						lprintf(": %s", objectname[j]);
						flag = 1;
					}
			if (!flag) {
				lprintf("\n%c: unknown monster/object", i);
			}
			return;
	}
}

void
specify_obj_cursor(void) {
	int objx, objy;
	int i;
	fl_display_message("\nMove the cursor to an unknown object.");
	fl_display_message("\n(For instructions type a ?)");
	objx = player_horizontal_position;
	objy = player_vertical_position;
	fl_termcap_cursor_position(objx + 1, objy + 1);
	/*  make cursor visible.
	*/
	for (;;) {
		switch (ttgetch()) {
			case '?':
				fl_termcap_cursor_position(1, 24);
				fl_display_message
				("\nUse [hjklnbyu] to move the cursor to the unknown object.");
				fl_display_message("\nType a . when the cursor is at the desired place.");
				fl_display_message("\nType q, Return, or Escape to exit.");
				fl_termcap_cursor_position(objx + 1, objy + 1);
				break;
			case '\33':
			case 'q':
			case '\n':
				/*  reset cursor
				*/
				fl_termcap_cursor_position(player_horizontal_position + 1, player_vertical_position + 1);
				return;
			case '.':
				/*  reset cursor
				*/
				fl_termcap_cursor_position(player_horizontal_position + 1, player_vertical_position + 1);
				fl_termcap_cursor_position(1, 24);
				if ((objx == player_horizontal_position) && (objy == player_vertical_position)) {
					lprintf("\n@: %s", logname);
					return;
				}
				i = monster_identification[objx][objy];
				if (i && (been_here_before[objx][objy] & KNOWHERE))
					/*  check for invisible monsters and not display
					*/
					if (monstnamelist[i] != floorc) {
						lprintf("\n%c: %s", monstnamelist[i], monster[i].name);
						return;
					}
				/*  handle floor separately so as not to display traps, etc.
				*/
				i = object_identification[objx][objy];
				if (i == 0) {
					fl_output_single_byte_to_output_buffer('\n');
					fl_output_single_byte_to_output_buffer(floorc);
					lprintf(": the floor of the dungeon");
					return;
				}
				if (been_here_before[objx][objy] & HAVESEEN) {
					fl_output_single_byte_to_output_buffer('\n');
					fl_output_single_byte_to_output_buffer(objnamelist[i]);
					lprintf(": %s", objectname[i]);
					return;
				}
				lprintf("\n : An as-yet-unseen place in the dungeon");
				return;
			case 'H':
			case 'h':
				move_cursor(&objx, &objy, 4);
				break;
			case 'J':
			case 'j':
				move_cursor(&objx, &objy, 1);
				break;
			case 'K':
			case 'k':
				move_cursor(&objx, &objy, 3);
				break;
			case 'L':
			case 'l':
				move_cursor(&objx, &objy, 2);
				break;
			case 'B':
			case 'b':
				move_cursor(&objx, &objy, 8);
				break;
			case 'N':
			case 'n':
				move_cursor(&objx, &objy, 7);
				break;
			case 'Y':
			case 'y':
				move_cursor(&objx, &objy, 6);
				break;
			case 'U':
			case 'u':
				move_cursor(&objx, &objy, 5);
				break;
			default:
				break;
		}
	}
}



static void
move_cursor(int *xx, int *yy, int cdir) {
	*xx += diroffx[cdir];
	*yy += diroffy[cdir];
	if (*yy < 0) {
		*yy = FL_MAX_VERTICAL_POSITION - 1;
	}
	if (*yy > FL_MAX_VERTICAL_POSITION - 1) {
		*yy = 0;
	}
	if (*xx < 0) {
		*xx = FL_MAX_HORIZONTAL_POSITION - 1;
	}
	if (*xx > FL_MAX_HORIZONTAL_POSITION - 1) {
		*xx = 0;
	}
	fl_termcap_cursor_position(*xx + 1, *yy + 1);
}
