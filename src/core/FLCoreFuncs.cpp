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

#include "FLInventory.hpp"
#include "FLScores.hpp"
#include "FLSystemDependant.hpp"
#include "FLFuncs.hpp"
#include "FLTermCapIO.hpp"
#include "../config/FLIncludeData.hpp"
#include "../templates/FLTMath.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../dungeon/FLDungeon.hpp"

/*
     fl_create_monster(lev)
         int lev;

    function to return monster number for a randomly selected monster
    for the given cave level

    FIXED by ~Gibbon.  Randomness was removed prior to 12.4alpha2 for some reason, so
    monsters were being taken based on the int of monstlevel (5,11 etc).

    Basically unchanged from 12.3 but cleaned up.  Don't fix what isn't broken.
*/
int
fl_create_monster(int lev) {
	int x, tmp;
	if (lev < 1) {
		lev = 1;
	} else if (lev > 12) {
		lev = 12;
	}
	if (lev < 5) {
		tmp = TRnd((x = monstlevel[lev - 1]) ? x : 1);
	} else {
		tmp =
		    TRnd((x =
		              monstlevel[lev - 1] - monstlevel[lev - 4]) ? x : 1) +
		    monstlevel[lev - 4];
	}
	while (monster[tmp].genocided && tmp < FL_MAX_MONSTERS_IN_GAME) {
		tmp++;			/* genocided? */
	}
	return tmp;
}

/*
    fl_place_the_player()

    Insure player is not in a wall or on top of a monster.  Could be more
    intelligent about what kinds of objects the player can land on.
*/
void
fl_place_the_player(void) {
	/*  The previous code was conflicting with changes I made
	    to maze generation and it caused a crash every time
	    LV10 was reached. ~Gibbon
	*/
	int pp_try = 2;
	while ((object_identification[player_horizontal_position][player_vertical_position] || monster_identification[player_horizontal_position][player_vertical_position]) && (pp_try))
		if (++player_horizontal_position >= FL_MAX_HORIZONTAL_POSITION - 1) {
			player_horizontal_position = 1;
			if (++player_vertical_position >= FL_MAX_VERTICAL_POSITION - 1) {
				player_vertical_position = 1;
				--pp_try;
			}
		}
	if (pp_try == 0)
		fl_display_message("Failure in fl_place_the_player()\n");
}

void fl_recalculate_armor_class(void) {
	int i, j, k;
	cdesc[FL_AC] = cdesc[FL_MOREDEFENSES];
	if (cdesc[FL_WEAR] >= 0)
		switch (iven[cdesc[FL_WEAR]]) {
			case OSHIELD:
				cdesc[FL_AC] += 2 + ivenarg[cdesc[FL_WEAR]];
				break;
			case OLEATHER:
				cdesc[FL_AC] += 2 + ivenarg[cdesc[FL_WEAR]];
				break;
			case OSTUDLEATHER:
				cdesc[FL_AC] += 3 + ivenarg[cdesc[FL_WEAR]];
				break;
			case ORING:
				cdesc[FL_AC] += 5 + ivenarg[cdesc[FL_WEAR]];
				break;
			case OCHAIN:
				cdesc[FL_AC] += 6 + ivenarg[cdesc[FL_WEAR]];
				break;
			case OSPLINT:
				cdesc[FL_AC] += 7 + ivenarg[cdesc[FL_WEAR]];
				break;
			case OPLATE:
				cdesc[FL_AC] += 9 + ivenarg[cdesc[FL_WEAR]];
				break;
			case OPLATEARMOR:
				cdesc[FL_AC] += 10 + ivenarg[cdesc[FL_WEAR]];
				break;
			case OSSPLATE:
				cdesc[FL_AC] += 12 + ivenarg[cdesc[FL_WEAR]];
				break;
			case OFIRETUNIC:
				cdesc[FL_FIRERESISTANCE] += 20 + ivenarg[cdesc[FL_WEAR]];
		}
	if (cdesc[FL_SHIELD] >= 0)
		if (iven[cdesc[FL_SHIELD]] == OSHIELD) {
			cdesc[FL_AC] += 2 + ivenarg[cdesc[FL_SHIELD]];
		}
	if (cdesc[FL_WIELD] < 0) {
		cdesc[FL_WCLASS] = 0;
	} else {
		i = ivenarg[cdesc[FL_WIELD]];
		switch (iven[cdesc[FL_WIELD]]) {
			case ODAGGER:
				cdesc[FL_WCLASS] = 3 + i;
				break;
			case OBELT:
				cdesc[FL_WCLASS] = 7 + i;
				break;
			case OSHIELD:
				cdesc[FL_WCLASS] = 8 + i;
				break;
			case OSPEAR:
				cdesc[FL_WCLASS] = 10 + i;
				break;
			case OBATTLEAXE:
				cdesc[FL_WCLASS] = 17 + i;
				break;
			case OGREATSWORD:
				cdesc[FL_WCLASS] = 19 + i;
				break;
			case OLONGSWORD:
				cdesc[FL_WCLASS] = 22 + i;
				break;
			case O2SWORD:
				cdesc[FL_WCLASS] = 26 + i;
				break;
			case OHSWORD:
				cdesc[FL_WCLASS] = 25 + i;
				cdesc[FL_AWARENESS] += 160;
				break;
			case OSWORD:
				cdesc[FL_WCLASS] = 32 + i;
				break;
			case OSWORDofSLASHING:
				cdesc[FL_WCLASS] = 30 + i;
				break;
			case OHAMMER:
				cdesc[FL_WCLASS] = 35 + i;
				break;
			default:
				cdesc[FL_WCLASS] = 0;
		}
	}
	cdesc[FL_WCLASS] += cdesc[FL_MORE_DAMAGE];
	/*  now for regeneration abilities based on rings   */
	cdesc[FL_REGEN] = 1;
	cdesc[FL_ENERGY] = 0;
	j = 0;
	for (k = 25; k > 0; k--)
		if (iven[k]) {
			j = k;
			k = 0;
		}
	for (i = 0; i <= j; i++) {
		switch (iven[i]) {
			case OPROTRING:
				cdesc[FL_AC] += ivenarg[i] + 1;
				break;
			case ODAMRING:
				cdesc[FL_WCLASS] += ivenarg[i] + 1;
				break;
			case OBELT:
				cdesc[FL_WCLASS] += ((ivenarg[i] << 1)) + 2;
				break;
			case OREGENRING:
				cdesc[FL_REGEN] += ivenarg[i] + 1;
				break;
			case ORINGOFEXTRA:
				cdesc[FL_REGEN] += 5 * (ivenarg[i] + 1);
				break;
			case OENERGYRING:
				cdesc[FL_ENERGY] += ivenarg[i] + 1;
				break;
		}
	}
}

void
fl_exit_the_game(void) {
	int i;
	fl_termcap_cursor_position(1, 24);
	strcpy(lastmonst, "");
	fl_display_message("\nDo you really want to quit?");
	for (;;) {
		i = ttgetch();
		if ((i == 'y') || (i == 'Y')) {
			fl_clear_terminal();
			exit(EXIT_SUCCESS);
			return;
		}
		if ((i == 'n') || (i == 'N') || (i == '\33')) {
			fl_display_message(" no");
			fl_output_buffer_flush();
			return;
		}
	}
}

int
fl_more_info(char select_allowed) {
	int i;
	fl_display_message("\n  --- press ");
	fl_invert_color_space("space");
	fl_display_message(" to continue --- ");
	if (select_allowed) {
		fl_display_message("letter to select --- ");
	}
	for (;;) {
		if ((i = ttgetch()) == ' ' || i == '\n') {
			return 0;
		}
		if (i == '\x1B') {
			return 1;
		}
		if (select_allowed) {
			if (isupper(i)) {
				i = tolower(i);
			}
			if ((i >= 'a' && i <= 'z') || i == '.') {
				return i;
			}
		}
	}
}

void
fl_enchant_armor(void) {
	int tmp;
	if (cdesc[FL_WEAR] < 0) {
		if (cdesc[FL_SHIELD] < 0) {
			fl_termcap_cursor_position(1, 24);
			fl_display_message("\nYou feel a sense of loss");
			return;
		} else {
			tmp = iven[cdesc[FL_SHIELD]];
			if (tmp != OSCROLL)
				if (tmp != OPOTION) {
					ivenarg[cdesc[FL_SHIELD]]++;
					bottomline();
				}
		}
	}
	tmp = iven[cdesc[FL_WEAR]];
	if (tmp != OSCROLL)
		if (tmp != OPOTION) {
			ivenarg[cdesc[FL_WEAR]]++;
			bottomline();
		}
}

void
fl_enchant_weapon(void) {
	int tmp;
	if (cdesc[FL_WIELD] < 0) {
		fl_termcap_cursor_position(1, 24);
		fl_display_message("\nYou feel a sense of loss");
		return;
	}
	tmp = iven[cdesc[FL_WIELD]];
	if (tmp != OSCROLL)
		if (tmp != OPOTION) {
			ivenarg[cdesc[FL_WIELD]]++;
			if (tmp == OCLEVERRING) {
				cdesc[FL_INTELLIGENCE]++;
			} else if (tmp == OSTRRING) {
				cdesc[FL_EXTRA_STRENGTH]++;
			} else if (tmp == ODEXRING) {
				cdesc[FL_DEXTERITY]++;
			}
			bottomline();
		}
}

int
fl_monster_closeby(void) {
	int tmp, tmp2;
	for (tmp = player_horizontal_position - 1; tmp < player_horizontal_position + 2; tmp++)
		for (tmp2 = player_vertical_position - 1; tmp2 < player_vertical_position + 2; tmp2++)
			if (monster_identification[tmp][tmp2]) {
				return (1);    /* if monster nearby */
			}
	return 0;
}

int
fl_stolen(void) {
	int i, j;
	j = 100;
	for (;;) {
		i = TRund(MAXINVEN);
		if (iven[i] && cdesc[FL_WEAR] != i &&
		        cdesc[FL_WIELD] != i && cdesc[FL_SHIELD] != i) {
			fl_display_inventory(i, 0);
			adjustcvalues(iven[i], ivenarg[i]);
			iven[i] = 0;
			return 1;
		}
		if (--j <= 0) {
			return 0;
		}
	}
}

int
fl_empty_handed(void) {
	int i;
	for (i = 0; i < 26; i++) {
		if (iven[i] && i != cdesc[FL_WIELD] &&
		        i != cdesc[FL_WEAR] && i != cdesc[FL_SHIELD]) {
			return 0;
		}
	}
	return 1;
}

void
fl_create_gem(void) {
	int i, j;
	switch (TRnd(4)) {
		case 1:
			i = ODIAMOND;
			j = 50;
			break;
		case 2:
			i = ORUBY;
			j = 40;
			break;
		case 3:
			i = OEMERALD;
			j = 30;
			break;
		default:
			i = OSAPPHIRE;
			j = 20;
			break;
	};
	fl_create_an_item(i, TRnd(j) + j / 10);
}



/*
    function to change character levels as needed when dropping an object
    that affects these characteristics
*/
void
adjustcvalues(int itm, int arg) {
	int flag;
	flag = 0;
	switch (itm) {
		case ODEXRING:
			cdesc[FL_DEXTERITY] -= arg + 1;
			flag = 1;
			break;
		case OSTRRING:
			cdesc[FL_EXTRA_STRENGTH] -= arg + 1;
			flag = 1;
			break;
		case OCLEVERRING:
			cdesc[FL_INTELLIGENCE] -= arg + 1;
			flag = 1;
			break;
		case OHAMMER:
			cdesc[FL_DEXTERITY] -= 10;
			cdesc[FL_EXTRA_STRENGTH] -= 10;
			cdesc[FL_INTELLIGENCE] += 10;
			flag = 1;
			break;
		case OSWORDofSLASHING:
			cdesc[FL_DEXTERITY] -= 5;
			flag = 1;
			break;
		case OORBOFDRAGON:
			--cdesc[FL_SLAYING];
			return;
		case OSPIRITSCARAB:
			--cdesc[FL_NEGATE_SPIRIT];
			return;
		case OCUBEofUNDEAD:
			--cdesc[FL_CUBE_OF_UNDEAD];
			return;
		case ONOTHEFT:
			--cdesc[FL_NO_THEFT];
			return;
		case OGREATSWORD:
			cdesc[FL_GREAT_SWORD_OF_DEATH] = 0;
			return;
		case OPOTION:
		case OSCROLL:
			return;
		default:
			flag = 1;
	};
	if (flag) {
		bottomline();
	}
}

/*
    subroutine to get a yes or no response from the user
    returns y or n
*/
char
getyn(void) {
	int i = 0;
	while (i != 'y' && i != 'n' && i != '\33') {
		i = ttgetch();
	}
	return (char) i;
}



/*
    function to calculate the pack weight of the player
    returns the number of pounds the player is carrying
*/
int
packweight(void) {
	int i, j = 25, k;
	k = cdesc[FL_GOLD] / 1000;
	while ((iven[j] == 0) && (j > 0)) {
		--j;
	}
	for (i = 0; i <= j; i++)
		switch (iven[i]) {
			case 0:
				break;
			case OSSPLATE:
			case OPLATEARMOR:
				k += 40;
				break;
			case OPLATE:
				k += 35;
				break;
			case OHAMMER:
				k += 30;
				break;
			case OSPLINT:
				k += 26;
				break;
			case OSWORDofSLASHING:
			case OCHAIN:
			case OBATTLEAXE:
			case O2SWORD:
			case OHSWORD:
				k += 23;
				break;
			case OLONGSWORD:
			case OSWORD:
			case ORING:
				k += 20;
				break;
			case OGREATSWORD:
			case OSTUDLEATHER:
				k += 15;
				break;
			case OLEATHER:
			case OSPEAR:
				k += 8;
				break;
			case OORBOFDRAGON:
			case OBELT:
				k += 4;
				break;
			case OSHIELD:
				k += 7;
				break;
			case OCHEST:
				k += 30 + ivenarg[i];
				break;
			default:
				k++;
				break;
		};
	return (k);
}
