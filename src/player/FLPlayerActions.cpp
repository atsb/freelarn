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

#include "FLPlayerActions.hpp"
#include "../config/FLIncludeData.hpp"
#include "../templates/FLTMath.hpp"
#include "../dungeon/FLDungeon.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../terminal/FLGetChar.hpp"
#include "../core/FLTermCapIO.hpp"
#include "../core/FLScores.hpp"
#include "../core/FLSystemDependant.hpp"
#include "FLRegenerate.hpp"
#include "FLHunger.hpp"
#include "../core/FLFuncs.hpp"
#include "../core/FLInventory.hpp"
#include "../templates/FLTTerm.hpp"

using std::cout;

static void fl_temple_actions(int);
static void act_prayer_heard(void);
extern int (*get_final_char_keyboard)(void);

/*
    act_remove_gems

    Remove gems from a throne.

    arg is zero if there is a gnome king associated with the throne

    Assumes that cursors() has been called previously, and that a check
    has been made that the throne actually has gems.
*/
void
fl_a_remove_the_gems(int arg) {
	int i, k;
	k = TRnd(101);
	if (k < 25) {
		for (i = 0; i < TRnd(4); i++) {
			/* gems pop ohf the throne */
			fl_create_gem();
		}
		object_identification[player_horizontal_position][player_vertical_position] = ODEADTHRONE;
		been_here_before[player_horizontal_position][player_vertical_position] = 0;
	} else if (k < 40 && arg == 0) {
		fl_create_a_monster(GNOMEKING);
		object_identification[player_horizontal_position][player_vertical_position] = OTHRONE2;
		been_here_before[player_horizontal_position][player_vertical_position] = 0;
	} else {
		fl_display_message("\nNothing happens");
	}
}



/*
    act_sit_throne

    Sit on a throne.

    arg is zero if there is a gnome king associated with the throne

    Assumes that cursors() has been called previously.
*/
void
fl_a_sit_on_throne(int arg) {
	int k;
	k = TRnd(101);
	if (k < 30 && arg == 0) {
		fl_create_a_monster(GNOMEKING);
		object_identification[player_horizontal_position][player_vertical_position] = OTHRONE2;
		been_here_before[player_horizontal_position][player_vertical_position] = 0;
	} else if (k < 35) {
		fl_display_message("\nZaaaappp!  You've been teleported!\n");
		fl_teleport(0);
	} else {
		fl_display_message("\nNothing happens");
	}
}



/*
    assumes that cursors() has been called and that a check has been made that
    the user is actually standing at a set of up stairs.
*/
void
fl_a_travel_up_stairs(void) {
	if (level >= 2 && level != 11) {
		fl_generate_a_new_dungeon_level(level - 1);
		draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);
		bot_linex();
		refresh();
	} else {
		fl_display_message("\nThe stairs lead to a dead end!");
	}
}



/*
    assumes that cursors() has been called and that a check has been made that
    the user is actually standing at a set of down stairs.
*/
void
fl_a_travel_down_stairs(void) {
	if (level != 0 && level != 10 && level != 13) {
		fl_generate_a_new_dungeon_level(level + 1);
		draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);
		bot_linex();
		refresh();
	} else {
		fl_display_message("\nThe stairs lead to a dead end!");
	}
}



/*
    Code to perform the action of drinking at a fountian.  Assumes that
    cursors() has already been called, and that a check has been made that
    the player is actually standing at a live fountain.
*/
void
fl_a_drink_from_fountain(void) {
	int x;
	if (TRnd(1501) < 2) {
		fl_display_message("\nOops!  You seem to have caught the dreadful sleep!");
		fl_output_buffer_flush();
		fl_wait(FL_WAIT_DURATION);
		fl_player_death(280);
		return;
	}
	x = TRnd(100);
	if (x < 7) {
		cdesc[FL_HALFDAM] += 200 + TRnd(200);
		fl_display_message("\nYou feel a sickness coming on");
	} else if (x < 13)
		fl_drink_potion(23,
		                0);	/* see invisible,but don't been_here_before the potion */
	else if (x < 45) {
		fl_display_message("\nnothing seems to have happened");
	} else if (TRnd(3) != 2) {
		fl_change_char_level(1);
	} else {
		fl_change_char_level(-1);
	}
	if (TRnd(12) < 3) {
		fl_display_message("\nThe fountains bubbling slowly quiets");
		object_identification[player_horizontal_position][player_vertical_position] = ODEADFOUNTAIN;	/* dead fountain */
		been_here_before[player_horizontal_position][player_vertical_position] = 0;
	}
	if (TRnd(10) < (6)) {
		if (cdesc[FL_HUNGER] < 100) {
			fl_display_message("\nThe refreshing water makes you a little less hungry");
			cdesc[FL_HUNGER] += 5;
		} else {
			fl_display_message("\nThe refreshing water gives you strength!");
			cdesc[FL_STRENGTH] += 1;
		}
	}
	return;
}

/*
    Code to perform the action of washing at a fountain.  Assumes that
    cursors() has already been called and that a check has been made that
    the player is actually standing at a live fountain.
*/
void
fl_a_wash_in_fountain(void) {
	FLCoreFuncs CoreFuncs;
	int x;
	if (TRnd(100) < 11) {
		x = TRnd((level << 2) + 2);
		lprintf("\nOh no!  The water was foul!  You suffer %d hit points!",
		        (int) x);
		lastnum = 273;
		CoreFuncs.DecreasePHealth(x);
		bottomline();
		fl_termcap_cursor_position(1, 24);
	} else if (TRnd(100) < 29) {
		fl_display_message("\nYou got the dirt off!");
	} else if (TRnd(100) < 31)
		fl_display_message
		("\nThis water seems to be hard water!  The dirt didn't come off!");
	else if (TRnd(100) < 34) {
		fl_create_a_monster(WATERLORD);     /*    make water lord     */
	} else {
		fl_display_message("\nnothing seems to have happened");
	}
	return;
}

void fl_a_enter_the_temple(void) {
	FLCoreFuncs CoreFuncs;
	
	if (cdesc[FL_WEAR] >= 0)
		switch (iven[cdesc[FL_WEAR]]) {
			case OFIRETUNIC:
				fl_display_message("\nThe temple doors open and your tunic protects you!");
			break;
			case OSHIELD:
			case OLEATHER:
			case OSTUDLEATHER:
			case ORING:
			case OCHAIN:
			case OSPLINT:
			case OPLATE:
			case OPLATEARMOR:
			case OSSPLATE:
				fl_display_message("\nThe temple doors begin to burn you!");
				lastnum = 275;
				CoreFuncs.DecreasePHealth(50 + TRnd(26));
				bottomhp();
		}
	fl_generate_a_new_dungeon_level(FL_MAX_CAVE_OF_LARN_LEVEL);
	draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);
	bot_linex();
	return;
}

int
readnum(int mx) {
	int i;
	int amt = 0;
	get_final_char_keyboard = ansiterm_getche;
	/*  allow him to say * for all gold
	*/
	if ((i = ttgetch()) == '*') {
		amt = mx;
	} else
		/* read chars into buffer, deleting when requested */
		while (i != '\n') {
			if (i == '\033') {
				get_final_char_keyboard = ansiterm_getch;
				fl_display_message(" aborted");
				return (0);
			}
			if ((i <= '9') && (i >= '0') && (amt < 999999999)) {
				amt = amt * 10 + i - '0';
			}
			if ((i == '\010') || (i == '\177')) {
				amt = (int)(amt / 10);
			}
			i = ttgetch();
		}
	get_final_char_keyboard = ansiterm_getch;
	return (amt);
}

void
fl_a_exit_the_temple(void) {
	FLCoreFuncs CoreFuncs;
	if (level != 11) {
		fl_display_message
		("\nThese doors cannot be opened.");
		return;
	}
	if (cdesc[FL_STRENGTH] < 20 + TRnd(2)) {
		fl_display_message("\nYou suddenly feel weaker.");
		lastnum = 275;
		CoreFuncs.DecreasePHealth(15 + TRnd(20));
		bottomhp();
		return;
	}
	fl_output_buffer_flush();
	fl_generate_a_new_dungeon_level(0);
	fl_temple_actions(FL_OBJECT_TEMPLE_IN);
	return;
}

static void
fl_temple_actions(int object) {
	int i, j;
	/* place player near the volcanic shaft */
	for (i = 0; i < FL_MAX_VERTICAL_POSITION; i++)
		for (j = 0; j < FL_MAX_HORIZONTAL_POSITION; j++)
			if (object_identification[j][i] == object) {
				player_horizontal_position = j;
				player_vertical_position = i;
				fl_place_the_player();
				i = FL_MAX_VERTICAL_POSITION;
				break;
			}
	draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);
	bot_linex();
	return;
}



/*
    Perform the actions associated with Altar desecration.
*/
void
fl_a_desecrate_an_altar(void) {
	FLCoreFuncs CoreFuncs;
	if (TRnd(100) < 60) {
		fl_create_a_monster(fl_create_monster(level + 2) + 8);
		fl_display_message("\nThe altar explodes violently and spawns a monster!");
		CoreFuncs.DecreasePHealth(5);
		cdesc[FL_AGGRAVATE] += 2500;
		TForgetData(0);
	} else if (TRnd(101) < 30) {
		fl_display_message("\nThe altar crumbles into a pile of dust before your eyes");
		TForgetData(0);		/*  remember to destroy the altar   */
	} else {
		fl_display_message("\nThe altar was desecrated and ruined!");
		TForgetData(0);
	}
	return;
}



/*
    Perform the actions associated with praying at an altar and giving a
    donation.
*/
void
fl_a_give_donation(void) {
	int k, temp;
	for (;;) {
		fl_display_message("\n\n");
		fl_termcap_cursor_position(1, 24);
		TClearToEOL(CL_LINE);
		fl_termcap_cursor_position(1, 23);
		TClearToEOL(CL_LINE);
		fl_display_message("how much do you donate? ");
		k = readnum((int) cdesc[FL_GOLD]);
		fl_display_message("\n");
		/*  make giving zero gold equivalent to 'just pray'ing.  Allows player to
		    'just pray' in command mode, without having to add yet another command.
		*/
		if (k == 0) {
			fl_a_just_pray();
			return;
		}
		if (cdesc[FL_GOLD] >= k) {
			temp = cdesc[FL_GOLD] / 10;
			cdesc[FL_GOLD] -= k;
			bottomline();
			/*  if player gave less than 10% of _original_ gold, make a monster
			*/
			if (k < temp || k < TRnd(50)) {
				/* added by ~Gibbon */
				fl_display_message("You have offended the Gods.");
				fl_create_a_monster(fl_create_monster(level + 1));
				cdesc[FL_AGGRAVATE] += 200;
				return;
			}
			if (TRnd(101) > 50) {
				act_prayer_heard();
				return;
			}
			if (TRnd(43) == 5) {
				if (cdesc[FL_WEAR]) {
					fl_display_message("You feel your armor vibrate for a moment");
				}
				fl_enchant_armor();
				return;
			}
			if (TRnd(43) == 8) {
				if (cdesc[FL_WIELD]) {
					fl_display_message("You feel your weapon vibrate for a moment");
				}
				fl_enchant_weapon();
				return;
			}
			fl_display_message("The Gods thank you for your prayers.");
			return;
		}
		/*  Player donates more gold than they have.  Loop back around so
		    player can't escape the altar for free.
		*/
		fl_display_message("You don't have that much!");
	}
	TForgetData(0);
}



/*
    Performs the actions associated with 'just praying' at the altar.  Called
    when the user responds 'just pray' when in prompt mode, or enters 0 to
    the money prompt when praying.

    Assumes cursors(), and that any leading \n have been printed
*/
void
fl_a_just_pray(void) {
	if (TRnd(100) < 75) {
		fl_display_message("The altar ascends!");
	} else if (TRnd(43) == 10) {
		if (cdesc[FL_WEAR]) {
			fl_display_message("You feel your armor vibrate for a moment");
		}
		fl_enchant_armor();
		return;
	} else if (TRnd(43) == 10) {
		if (cdesc[FL_WIELD]) {
			fl_display_message("You feel your weapon vibrate for a moment");
		}
		fl_enchant_weapon();
		return;
	} else {
		fl_create_a_monster(fl_create_monster(level + 1));
	}
	return;
	TForgetData(0);
}

/*
    Performs the actions associated with 'give thanks' at the altar.  Called
    when the user responds 'give thanks (t)' when in prompt mode.

    Assumes cursors(), and that any leading \n have been printed

    ~Gibbon
*/
void
fl_a_give_thanks(void) {
	FLCoreFuncs CoreFuncs;
	int i;
	//The below math is to identify if the player has the ghani statue in their inventory or not. ~Gibbon
	for (i = 0; i < MAXOBJECT; i++)
		/*
		    If the player is holding the Ghani statue, when praying at an altar,
		    they will get some goodies.

		    ~Gibbon
		*/
		if (iven[i] == OGHANISTATUE) {
			fl_display_message("You place the statues on the altar and close your eyes reverently..");
			fl_display_message("\nWe hear your prayers.\nBehold your increased strength and experience!");
			cdesc[FL_STRENGTH] += 4;
			CoreFuncs.IncreaseExperience(600);
			//Then we remove the statue from the inventory.  If the user has more than 1 all will be removed. ~Gibbon
			iven[i] = 0;
			TForgetData(0);
		}
	return;
}

/*
    function to cast a +3 protection on the player
*/
static void
act_prayer_heard(void) {
	fl_display_message("You have been heard!");
	if (cdesc[FL_ALTPRO] == 0) {
		cdesc[FL_MOREDEFENSES] += 3;
	}
	/* protection field */
	cdesc[FL_ALTPRO] += 500;
	bottomline();
	TForgetData(0);
}

/*
    Performs the act of ignoring an altar.

    Assumptions:  cursors() has been called.
*/
void
fl_a_ignore_an_altar(void) {
	if (TRnd(100) < 30) {
		fl_create_a_monster(fl_create_monster(level + 1));
		fl_display_message("The altar turns into a monster!");
		cdesc[FL_AGGRAVATE] += TRnd(450);
		TForgetData(0);
	} else {
		fl_display_message("\nThe altar crumbles into stone!");
		TForgetData(0);
	}
	return;
}



/*
    Performs the act of opening a chest.

    Parameters:   x,y location of the chest to open.
    Assumptions:  cursors() has been called previously
*/
void
fl_a_open_chest(int x, int y) {
	int i, k;
	k = TRnd(101);
	FLCoreFuncs CoreFuncs;
	if (k < 40) {
		fl_display_message("\nThe chest explodes as you open it");
		i = TRnd(10);
		lastnum = 281;		/* in case he dies */
		lprintf("\nYou suffer %d hit points damage!", (int) i);
		if (i > 0) {
			CoreFuncs.DecreasePHealth(i);
			bottomhp();
		}
		switch (TRnd(10)) {	/* see if he gets a curse */
			case 1:
				cdesc[FL_ITCHING] += TRnd(1000) + 100;
				fl_display_message("\nYou feel an irritation spread over your skin!");
				break;
			case 2:
				cdesc[FL_CLUMSINESS] += TRnd(1600) + 200;
				fl_display_message("\nYou begin to lose hand to eye coordination!");
				break;
			case 3:
				cdesc[FL_HALFDAM] += TRnd(1600) + 200;
				fl_display_message("\nA sickness engulfs you!");
				break;
		};
		object_identification[x][y] = been_here_before[x][y] = 0;	/* destroy the chest */
		if (TRnd(100) < 69) {
			fl_create_gem();   /* gems from the chest */
		}
		fl_drop_rand_gold_around_player_location(TRnd(110 * object_argument[player_horizontal_position][player_vertical_position] + 200));
		for (i = 0; i < TRnd(4); i++) {
			fl_create_rand_object_around_player_location(object_argument[player_horizontal_position][player_vertical_position] + 2);
		}
	} else {
		fl_display_message("\nNothing happens");
	}
	return;
}

/*
    Perform the actions common to command and prompt mode when opening a
    door.  Assumes cursors().

    Parameters:     the X,Y location of the door to open.
    Return value:   TRUE if successful in opening the door, false if not.
*/
int
fl_a_open_dungeon_door(int x, int y) {
	FLCoreFuncs CoreFuncs;
	if (TRnd(11) < 7) {
		switch (object_argument[x][y]) {
			case 6:
				fl_display_message("\n\nThe door makes an awful groan, but remains stuck");
				cdesc[FL_AGGRAVATE] += TRnd(400);
				break;
			case 7:
				fl_display_message("\n\nYou are jolted by an electric shock");
				lastnum = 274;
				CoreFuncs.DecreasePHealth(TRnd(20));
				bottomline();
				break;
			case 8:
				fl_display_message("\n\nYou feel drained");
				CoreFuncs.DecreasePlayerLevel();
				break;
			case 9:
				fl_display_message("\n\nYou suddenly feel weaker");
				if (cdesc[FL_STRENGTH] > 3) {
					cdesc[FL_STRENGTH]--;
				}
				bottomline();
				break;
			default:
				fl_display_message("\n\nThe door makes an awful groan, but remains stuck");
				break;
		}
		return (0);
	} else {
		fl_display_message("\n\nThe door opens");
		been_here_before[x][y] = 0;
		object_identification[x][y] = OOPENDOOR;
		return (1);
	}
}

/*
    function to wield a weapon
*/
void
wield(void) {
	int i;
	for (;;) {
		i = whatitem("wield (- for nothing)");
		if (i == '\33') {
			return;
		}
		if (i != '.') {
			if (i == '-') {
				cdesc[FL_WIELD] = -1;
				bottomline();
				return;
			}
			if (!i || i == '.') {
				continue;
			}
			if (iven[i - 'a'] == 0) {
				fl_termcap_cursor_position(1, 24);
				lprintf("\nYou do not have %c!", i);
				return;
			} else if (iven[i - 'a'] == OPOTION) {
				fl_termcap_cursor_position(1, 24);
				lprintf("\nYou cannot wield %c!", i);
				return;
			} else if (iven[i - 'a'] == OSCROLL) {
				fl_termcap_cursor_position(1, 24);
				lprintf("\nYou cannot wield %c!", i);
				return;
			} else if (cdesc[FL_SHIELD] != -1
			           && iven[i - 'a'] == O2SWORD) {
				fl_display_message("\nBut one arm is busy with your shield!");
				return;
			} else if (cdesc[FL_SHIELD] != -1
			           && iven[i - 'a'] == OHSWORD) {
				fl_display_message("\nA longsword of Hymie cannot be used while a shield is equipped!");
				return;
			} else {
				cdesc[FL_WIELD] = i - 'a';
				if (iven[i - 'a'] == OGREATSWORD) {
					cdesc[FL_GREAT_SWORD_OF_DEATH] = 1;
				} else {
					cdesc[FL_GREAT_SWORD_OF_DEATH] = 0;
				}
				bottomline();
				return;
			}
		}
	}
}

/*
    function to wear armor
*/
void
wear(void) {
	int i;
	for (;;) {
		if ((i = whatitem("wear")) == '\33') {
			return;
		}
		if (i != '.' && i != '-') {
			if (i && i != '.')
				switch (iven[i - 'a']) {
					case 0:
						fl_termcap_cursor_position(1, 24);
						lprintf("\nYou do not have %c!", i);
						return;
					case OLEATHER:
					case OCHAIN:
					case OPLATE:
					case ORING:
					case OSPLINT:
					case OPLATEARMOR:
					case OFIRETUNIC:
					case OSTUDLEATHER:
					case OSSPLATE:
						if (cdesc[FL_WEAR] != -1) {
							fl_display_message("\nYou're already wearing some armor");
							return;
						}
						cdesc[FL_WEAR] = i - 'a';
						bottomline();
						return;
					case OSHIELD:
						if (cdesc[FL_SHIELD] != -1) {
							fl_display_message("\nYou are already wearing a shield");
							return;
						}
						if (iven[cdesc[FL_WIELD]] == O2SWORD) {
							fl_display_message
							("\nYour hands are busy with the two handed sword!");
							return;
						}
						if (iven[cdesc[FL_WIELD]] == OHSWORD) {
							fl_display_message("\nYou are holding a longsword of Hymie!");
							return;
						}
						cdesc[FL_SHIELD] = i - 'a';
						bottomline();
						return;
					default:
						fl_display_message("\nYou can't wear that!");
				};
		}
	}
}




/*
    function to drop an object
*/
void
dropobj(void) {
	int i;
	int *p;
	int amt;
	p = &object_identification[player_horizontal_position][player_vertical_position];
	for (;;) {
		if ((i = whatitem("drop")) == '\33') {
			return;
		}
		if (i != '-') {
			if (i == '.') {	/* drop some gold */
				if (*p) {
					lprintf("\nThere's something here already: %s",
					        objectname[object_identification[player_horizontal_position][player_vertical_position]]);
					dropflag = 1;
					return;
				}
				fl_display_message("\n\n");
				fl_clear_screen_from_to_end_of_display(1, 23);
				fl_display_message("How much gold do you drop? ");
				if ((amt = readnum((int) cdesc[FL_GOLD])) == 0) {
					return;
				}
				if (amt > cdesc[FL_GOLD]) {
					fl_display_message("\n");
					fl_display_message("You don't have that much!");
					return;
				}
				if (amt <= 32767) {
					*p = OGOLDPILE;
					i = amt;
				} else if (amt <= 327670L) {
					*p = ODGOLD;
					i = amt / 10;
					amt = 10L * i;
				} else if (amt <= 3276700L) {
					*p = OMAXGOLD;
					i = amt / 100;
					amt = 100L * i;
				} else if (amt <= 32767000L) {
					*p = OKGOLD;
					i = amt / 1000;
					amt = 1000L * i;
				} else {
					*p = OKGOLD;
					i = 32767;
					amt = 32767000L;
				}
				cdesc[FL_GOLD] -= amt;
				lprintf("\nYou drop %d gold pieces", amt);
				object_argument[player_horizontal_position][player_vertical_position] = i;
				been_here_before[player_horizontal_position][player_vertical_position] = 0;
				dropflag = 1;
				return;
			}
			if (i) {
				drop_object(i - 'a');
				return;
			}
		}
	}
}


int
floor_consume(int search_item, const char *cons_verb) {
	int i;
	char tempc;
	fl_termcap_cursor_position(1, 24);
	i = object_identification[player_horizontal_position][player_vertical_position];
	/*  object not there, quit
	*/
	if (i != search_item) {
		return (0);
	}
	/*  object there.  does the player want to consume it?
	*/
	lprintf("\nThere is %s", objectname[i]);
	if (i == OSCROLL)
		if (scrollname[object_argument[player_horizontal_position][player_vertical_position]][0]) {
			lprintf("%s", scrollname[object_argument[player_horizontal_position][player_vertical_position]]);
		}
	if (i == OPOTION)
		if (potionname[object_argument[player_horizontal_position][player_vertical_position]][0]) {
			lprintf("%s", potionname[object_argument[player_horizontal_position][player_vertical_position]]);
		}
	lprintf(" here.  Do you want to %s it?", cons_verb);
	if ((tempc = getyn()) == 'n') {
		return (0);    /* object_identification there, not consumed */
	} else if (tempc != 'y') {
		fl_display_message(" aborted");
		return (-1);		/* abort */
	}
	/*  consume the object_identification.
	*/
	switch (i) {
		case OCOOKIE:
			TForgetData(0);
			break;
		case OBOOK:
			fl_read_a_book(object_argument[player_horizontal_position][player_vertical_position]);
			TForgetData(0);
			break;
		case OPRAYERBOOK:
			fl_read_a_prayer_book(object_argument[player_horizontal_position][player_vertical_position]);
			TForgetData(0);
			break;
		case OPOTION:
			fl_drink_potion(object_argument[player_horizontal_position][player_vertical_position], 1);
			TForgetData(0);
			break;
		case OSCROLL:
			/*  scrolls are tricky because of teleport.
			*/
			i = object_argument[player_horizontal_position][player_vertical_position];
			been_here_before[player_horizontal_position][player_vertical_position] = 0;
			object_identification[player_horizontal_position][player_vertical_position] = object_argument[player_horizontal_position][player_vertical_position] = 0;
			fl_read_scroll(i);
			break;
	}
	return (1);
}

void
consume(int search_item, const char *prompt) {
	int i;
	for (;;) {
		if ((i = whatitem(prompt)) == '\33') {
			return;
		}
		if (i != '.' && i != '-') {
			if (i && i != '.') {
				switch (iven[i - 'a']) {
					case OSCROLL:
						if (search_item != OSCROLL) {
							lprintf("\nYou can't %s that.", prompt);
							return;
						}
						fl_read_scroll(ivenarg[i - 'a']);
						break;
					case OBOOK:
						if (search_item != OSCROLL) {
							lprintf("\nYou can't %s that.", prompt);
							return;
						}
						fl_read_a_book(ivenarg[i - 'a']);
						break;
					case OPRAYERBOOK:
						if (search_item != OSCROLL) {
							lprintf("\nYou can't %s that.", prompt);
							return;
						}
						fl_read_a_prayer_book(ivenarg[i - 'a']);
						break;
					case OCOOKIE:
						if (search_item != OCOOKIE) {
							lprintf("\nYou can't %s that.", prompt);
							return;
						}
						break;
					case OPOTION:
						if (search_item != OPOTION) {
							lprintf("\nYou can't %s that.", prompt);
							return;
						}
						fl_drink_potion(ivenarg[i - 'a'], 1);
						break;
					case 0:
						fl_termcap_cursor_position(1, 24);
						lprintf("\nYou do not have %c!", i);
						return;
					default:
						lprintf("\nYou can't %s that.", prompt);
						return;
				}
				iven[i - 'a'] = 0;
				return;
			}
		}
	}
}



/*
    function to ask what player wants to do
*/
int
whatitem(const char *str) {
	int i = 0;
	fl_termcap_cursor_position(1, 24);
	lprintf("\nWhat do you want to %s? ", str);
	while (i > 'z' || (i < 'a' && i != '-' && i != '*' && i != '\33' && i != '.')) {
		i = ttgetch();
	}
	if (i == '\33') {
		fl_display_message(" aborted");
	}
	return (i);
}
