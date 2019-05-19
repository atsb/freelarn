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
#include "../terminal/FLTerminal.hpp"
#include "../core/FLInventory.hpp"
#include "../core/FLTermCapIO.hpp"
#include "FLDungeon.hpp"
#include "../player/FLRegenerate.hpp"
#include "../core/FLScores.hpp"
#include "../core/FLSystemDependant.hpp"
#include "../player/FLHunger.hpp"
#include "../core/FLFuncs.hpp"
#include "../core/FLStore.hpp"
#include "../templates/FLTTerm.hpp"

using std::cout;

static void fl_stairs(int);
static void fl_potion(int);
static void fl_scroll(int);
static void fl_non_bottomless_pit(void);
static void fl_bottomless_pit(void);
static void fl_statue(void);
static void fl_mirror(void);
static void fl_book(void);
static void fl_prayer_book(void);
static void fl_fortune_cookie(void);
static void fl_gold(int);
static void fl_prompt_for_entrance(void);
static void fl_prompt_for_temple_entrance(int);
static void fl_open_door(void);
static void fl_closed_door(void);
static void fl_candle(void);

void
fl_look_for_an_object_and_give_options(
    char do_ident,
    char do_pickup,
    char do_action
) {
	FLCoreFuncs CoreFuncs;
	int i, j;
	/* can't find objects if time is stopped    */
	if (cdesc[FL_TIMESTOP]) {
		return;
	}
	i = object_identification[player_horizontal_position][player_vertical_position];
	if (i == 0) {
		return;
	}
	j = object_argument[player_horizontal_position][player_vertical_position];
	showcell(player_horizontal_position, player_vertical_position);
	fl_termcap_cursor_position(1, 24);
	y_larn_rep = 0;
	switch (i) {
		case OGOLDPILE:
		case OMAXGOLD:
		case OKGOLD:
		case ODGOLD:
			fl_display_message("\nYou have found some gold!");
			fl_gold(i);
			break;
		case OPOTION:
			if (do_ident) {
				fl_display_message("\nYou have found a magic potion ");
				if (potionname[j][0]) {
					lprintf("%s", &potionname[j][1]);
				}
			}
			if (do_pickup)
				if (take(OPOTION, j) == 0) {
					TForgetData(0);
				}
			if (do_action) {
				fl_potion(j);
			}
			break;
		case OSCROLL:
			if (do_ident) {
				fl_display_message("\nYou have found a magic scroll ");
				if (scrollname[j][0]) {
					lprintf("%s", &scrollname[j][1]);
				}
			}
			if (do_pickup)
				if (take(OSCROLL, j) == 0) {
					TForgetData(0);
				}
			if (do_action) {
				fl_scroll(j);
			}
			break;
		case OALTAR:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				fl_display_message("\nThere is a Holy Altar here!");
				fl_display_message("\nDo you (p) pray  (d) desecrate");
				fl_display_message(", or (i) ignore it? ");
				for (;;) {
					switch (ttgetch()) {
						case 'p':
							fl_display_message(" pray\nDo you (m) give money, (t) give thanks or (j) just pray? ");
							for (;;)
								switch (ttgetch()) {
									case 'j':
										fl_display_message("\n");
										fl_a_just_pray();
										return;
									case 't':
										fl_display_message("\n");
										fl_a_give_thanks();
										fl_display_message("\nThe Altar descends into the floor and vanishes before your eyes!");
										TForgetData(0);
										return;
									case 'm':
										fl_a_give_donation();
										return;
									case '\33':
										return;
								};
						case 'd':
							fl_display_message(" desecrate");
							fl_a_desecrate_an_altar();
							return;
						case 'i':
						case '\33':
							fl_display_message("ignore\n");
							fl_a_ignore_an_altar();
							return;
					};
				}
			}
			break;
		case OBOOK:
			if (do_ident) {
				fl_display_message("\nYou have found a Book.");
			}
			if (do_pickup)
				if (take(OBOOK, j) == 0) {
					TForgetData(0);
				}
			if (do_action) {
				fl_book();
			}
			break;
		case OPRAYERBOOK:
			if (do_ident) {
				fl_display_message("\nYou have found a Prayer Book.");
			}
			if (do_pickup)
				if (take(OPRAYERBOOK, j) == 0) {
					TForgetData(0);
				}
			if (do_action) {
				fl_prayer_book();
			}
			break;
		case OCANDLE:
			if (do_ident) {
				fl_display_message("\nYou have found a Candle.");
			}
			if (do_pickup)
				if (take(OCANDLE, j) == 0) {
					TForgetData(0);
				}
			if (do_action) {
			fl_candle();
			}
			break;
		case OCOOKIE:
			if (do_ident) {
				fl_display_message("\nYou have found a fortune cookie.");
			}
			if (do_pickup)
				if (take(OCOOKIE, 0) == 0) {
					TForgetData(0);
				}
			if (do_action) {
				fl_fortune_cookie();
			}
			break;
		case OTHRONE:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				lprintf("\nThere is %s here!", objectname[i]);
				fl_extra_object_func_throne(0);
			}
			break;
		case OTHRONE2:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				lprintf("\nThere is %s here!", objectname[i]);
				fl_extra_object_func_throne(1);
			}
			break;
		case ODEADTHRONE:
			if (do_ident) {
				lprintf("\nThere is %s here!", objectname[i]);
				fl_display_message("\nDo you (s) sit down");
				fl_display_message(", or (i) ignore it? ");
				for (;;) {
					for (;;) {
						switch (ttgetch()) {
							case 's':
								fl_display_message(" sit down");
								fl_a_sit_on_throne(1);
								return;
							case 'i':
							case '\33':
								fl_display_message("ignore\n");
								return;
						};
					}
				}
			}
			break;
		case OPIT:
			/* always perform these actions. */
			fl_display_message("\nYou're standing at the top of a pit.");
			fl_non_bottomless_pit();
			break;
		case OSTAIRSUP:		/* up */
			if (do_ident) {
				fl_display_message("\nThere is a circular staircase here");
			}
			if (do_action) {
				fl_stairs(1);
			}
			refresh();
			break;
		case OFOUNTAIN:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				fl_display_message("\nThere is a fountain here");
				fl_termcap_cursor_position(1, 24);
				fl_display_message("\nDo you (d) drink, (w) wash yourself");
				fl_display_message(", or (i) ignore it? ");
				for (;;) {
					switch (ttgetch()) {
						case 'd':
							fl_a_drink_from_fountain();
							return;
						case '\33':
						case 'i':
							fl_display_message("ignore\n");
							return;
						case 'w':
							fl_a_wash_in_fountain();
							return;
					}
				}
			}
			break;
		case OSTATUE:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				fl_display_message("\nYou are standing in front of a statue");
				fl_statue();
			}
			break;
		case OCHEST:
			if (do_ident) {
				fl_display_message("\nThere is a chest here");
			}
			if (do_pickup)
				if (take(OCHEST, j) == 0) {
					TForgetData(0);
				}
			if (do_action) {
				fl_display_message("\nDo you (t) take it, (o) try to open it");
				fl_display_message(", or (i) ignore it? ");
				for (;;) {
					switch (ttgetch()) {
						case 'o':
							fl_display_message(" open it");
							fl_a_open_chest(player_horizontal_position, player_vertical_position);
							return;
						case 't':
							fl_display_message(" take");
							if (take(OCHEST, object_argument[player_horizontal_position][player_vertical_position]) == 0) {
								object_identification[player_horizontal_position][player_vertical_position] = been_here_before[player_horizontal_position][player_vertical_position] = 0;
							}
							return;
						case 'i':
						case '\33':
							fl_display_message("ignore\n");
							return;
					};
				}
			}
			break;
		case OMIRROR:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				fl_display_message("\nThere is a mirror here");
				fl_mirror();
			}
			break;
		case OBANK2:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				fl_display_message("\nYou have found a branch office of the bank of Larn.");
			}
			if (do_action) {
				fl_prompt_for_entrance();
			}
			break;
		case OBANK:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				fl_display_message("\nYou have found the bank of Larn.");
			}
			if (do_action) {
				fl_prompt_for_entrance();
			}
			break;
		case ODEADFOUNTAIN:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				fl_display_message("\nThere is a dead fountain here");
			}
			break;
		case ODNDSTORE:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				fl_display_message("\nThere is a DND store here.");
			}
			if (do_action) {
				fl_prompt_for_entrance();
			}
			break;
		case OSTAIRSDOWN:		/* down */
			if (do_ident) {
				fl_display_message("\nThere is a circular staircase here");
			}
			if (do_action) {
				fl_stairs(-1);
			}
			refresh();
			break;
		case OOPENDOOR:
			if (do_ident) {
				lprintf("\nYou have found %s", objectname[i]);
			}
			if (do_action) {
				fl_open_door();
			}
			break;
		case OCLOSEDDOOR:
			if (do_ident) {
				lprintf("\nYou have found %s", objectname[i]);
			}
			if (do_action) {
				fl_closed_door();
			}
			break;
		case OENTRANCE:
			if (do_ident) {
				fl_display_message("\nYou have found ");
			}
			fl_display_message(objectname[i]);
			if (do_action) {
				fl_prompt_for_entrance();
			}
			break;
		case FL_OBJECT_TEMPLE_IN:
			if (do_ident) {
				fl_display_message("\nYou have found ");
			}
			fl_display_message(objectname[i]);
			if (do_action) {
				fl_prompt_for_temple_entrance(-1);
			}
			break;
		case FL_OBJECT_TEMPLE_OUT:
			if (do_ident) {
				fl_display_message("\nYou have found ");
			}
			fl_display_message(objectname[i]);
			if (do_action) {
				fl_prompt_for_temple_entrance(1);
			}
			break;
		case OIVTELETRAP:
			if (TRnd(11) < 6) {
				return;
			}
			object_identification[player_horizontal_position][player_vertical_position] = OTELEPORTER;
			been_here_before[player_horizontal_position][player_vertical_position] = KNOWALL;
			/* fall through to OTELEPORTER case below!!! */
			__attribute__((fallthrough));
		case OTELEPORTER:
				cdesc[FL_TELEPORT_FLAG] = 1;
				fl_display_message("\n\nYou have been teleported!");
				fl_wait(FL_WAIT_DURATION);
				fl_teleport(0);
				refresh();
			break;
		case OTRAPARROWIV:		/* for an arrow trap */
			if (TRnd(17) < 13) {
				return;
			}
			object_identification[player_horizontal_position][player_vertical_position] = OTRAPARROW;
			been_here_before[player_horizontal_position][player_vertical_position] = 0;
			/* fall through to OTRAPARROW case below!!! */
			__attribute__((fallthrough));
		case OTRAPARROW:
			fl_display_message("\nYou are hit by an arrow");
			lastnum = 259;
			CoreFuncs.DecreasePHealth(TRnd(10) + level);
			bottomhp();
			return;
		case OIVDARTRAP:		/* for a dart trap */
			if (TRnd(17) < 13) {
				return;
			}
			object_identification[player_horizontal_position][player_vertical_position] = ODARTRAP;
			been_here_before[player_horizontal_position][player_vertical_position] = 0;
			/* fall through to ODARTTRAP case below!!! */
			__attribute__((fallthrough));
		case ODARTRAP:
			fl_display_message("\nYou are hit by a dart");
			lastnum = 260;
			CoreFuncs.DecreasePHealth(TRnd(5));
			if ((--cdesc[FL_STRENGTH]) < 3) {
				cdesc[FL_STRENGTH] = 3;
			}
			bottomline();
			return;
		case OIVTRAPDOOR:		/* for a trap door */
			if (TRnd(17) < 13) {
				return;
			}
			object_identification[player_horizontal_position][player_vertical_position] = OTRAPDOOR;
			been_here_before[player_horizontal_position][player_vertical_position] = KNOWALL;
			/* fall through to OTRAPDOOR case below!!! */
			__attribute__((fallthrough));
		case OTRAPDOOR:
			lastnum = 272;		/* a trap door */
			if ((level == FL_MAX_CAVE_OF_LARN_LEVEL - 1)
			        || (level == FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL - 1)) {
				fl_display_message("\nYou fell through a bottomless trap door!");
				fl_wait(FL_WAIT_DURATION);
				fl_player_death(271);
			}
			i = TRnd(5 + level);
			lprintf("\nYou fall through a trap door!  You lose %d hit points.",
			        (long) i);
			CoreFuncs.DecreasePHealth(i);
			fl_wait(FL_WAIT_DURATION);
			fl_generate_a_new_dungeon_level(level + 1);
			draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);
			bot_linex();
			return;
		case OTRADEPOST:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				fl_display_message("\nYou have found the Larn trading Post.");
			}
			if (do_action) {
				fl_prompt_for_entrance();
			}
			return;
		case OHOME:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				fl_display_message("\nYou have found your way home.");
			}
			if (do_action) {
				fl_prompt_for_entrance();
			}
			return;
		case OWALL:
			break;
		case FL_OBJECT_SPHERE_OF_ANNIHILATION:
			fl_player_death(283);		/* annihilated by sphere of annihilation */
			return;
		case OLRS:
			if (fl_monster_closeby()) {
				return;
			}
			if (do_ident) {
				fl_display_message("\nThere is an LRS office here.");
			}
			if (do_action) {
				fl_prompt_for_entrance();
			}
			break;
		default:
			if (do_ident) {
				lprintf("\nYou have found %s ", objectname[i]);
				switch (i) {
					case ODIAMOND:
					case ORUBY:
					case OEMERALD:
					case OSAPPHIRE:
					case OSPIRITSCARAB:
					case OORBOFDRAGON:
					case OCUBEofUNDEAD:
					case ONOTHEFT:
						break;
					default:
						if (j > 0) {
							lprintf("+ %d", (int) j);
						} else if (j < 0) {
							lprintf(" %d", (int) j);
						}
						break;
				}
			}
			if (do_pickup)
				if (take(i, j) == 0) {
					TForgetData(0);
				}
			if (do_action) {
				char tempc = 0;
				fl_display_message("\nDo you want to (t) take it");
				fl_display_message(", or (i) ignore it? ");
				while (tempc != 't' && tempc != 'i' && tempc != '\33') {
					tempc = ttgetch();
				}
				if (tempc == 't') {
					fl_display_message("take");
					if (take(i, j) == 0) {
						TForgetData(0);
					}
					return;
				}
				fl_display_message("ignore\n");
			}
			break;
	};
}

/*
    subroutine to process the stair cases if sphere_direction > 0 the up else down
*/
static void
fl_stairs(int sphere_direction) {
	fl_display_message("\nDo you (s) stay here ");
	if (sphere_direction > 0) {
		fl_display_message("or (u) go up? ");
	} else {
		fl_display_message("or (d) go down? ");
	}
	for (;;) {
		switch (ttgetch()) {
			case '\33':
			case 's':
			case 'i':
				fl_display_message("stay here");
				return;
			case 'u':
				fl_display_message("go up");
				fl_a_travel_up_stairs();
				return;
			case 'd':
				fl_display_message("go down");
				fl_a_travel_down_stairs();
				return;
		};
	}
}



/*
    subroutine to handle a teleport trap +/- 1 level maximum
*/
void
fl_teleport(int err) {
	int tmp;
	if (err)
		if (TRnd(151) < 3) {
			/* Fix for bug #10 ~Gibbon*/
			fl_termcap_cursor_position(1, 19);
			fl_display_message("\nYou were killed by teleporting into solid rock.");
			fl_wait(4000);
			fl_player_death(264);	/* stuck in a rock */
		}
	cdesc[FL_TELEPORT_FLAG] =
	    1;		/* show ?? on bottomline if been teleported    */
	if (level == 0) {
		tmp = 0;
	} else if (level < FL_MAX_CAVE_OF_LARN_LEVEL) {
		tmp = TRnd(5) + level - 3;
		if (tmp >= FL_MAX_CAVE_OF_LARN_LEVEL) {
			tmp = FL_MAX_CAVE_OF_LARN_LEVEL - 1;
		}
		if (tmp < 1) {
			tmp = 1;
		}
	} else {
		tmp = TRnd(3) + level - 2;
		if (tmp >= FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL) {
			tmp = FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL - 1;
		}
		if (tmp < FL_MAX_CAVE_OF_LARN_LEVEL) {
			tmp = FL_MAX_CAVE_OF_LARN_LEVEL;
		}
	}
	player_horizontal_position = TRnd(FL_MAX_HORIZONTAL_POSITION - 2);
	player_vertical_position = TRnd(FL_MAX_VERTICAL_POSITION - 2);
	if (level != tmp) {
		fl_generate_a_new_dungeon_level(tmp);
		fl_place_the_player();
		draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);
		bot_linex();
		refresh();
	}
}

/*
    function to process a potion
*/
static void
fl_potion(int pot) {
	fl_display_message("\nDo you (d) drink it, (t) take it");
	fl_display_message(", or (i) ignore it? ");
	for (;;) {
		switch (ttgetch()) {
			case '\33':
			case 'i':
				fl_display_message("ignore\n");
				return;
			case 'd':
				fl_display_message("drink\n");
				TForgetData(0);		/* destroy potion  */
				fl_drink_potion(pot, 1);
				return;
			case 't':
				fl_display_message("take\n");
				if (take(OPOTION, pot) == 0) {
					TForgetData(0);
				}
				return;
		};
	}
}



/*
    function to drink a potion

    Also used to perform the action of a potion without quaffing a potion (see
    invisible capability when drinking from a fountain).
*/
void
fl_drink_potion(int pot, int set_known) {
	FLCoreFuncs CoreFuncs;
	int i, j, k;
	/* check for within bounds */
	if (pot < 0 || pot >= MAXPOTION) {
		return;
	}
	/*
	    if player is to been_here_before this potion (really quaffing one), make it
	    known
	*/
	if (set_known) {
		potionname[pot][0] = ' ';
	}
	switch (pot) {
		case 0:
			fl_display_message("\nYou fall asleep. . .");
			i = TRnd(11) - (cdesc[FL_CONSTITUTION] >> 2) + 2;
			while (--i > 0) {
				parse2();
				fl_wait(FL_WAIT_DURATION);
			}
			fl_termcap_cursor_position(1, 24);
			fl_display_message("\nYou woke up!");
			return;
		case 1:
			fl_display_message("\nYou feel better");
			if (cdesc[FL_HP] == cdesc[FL_HPMAX]) {
				TRaiseMaxHealth(1);
			} else if ((cdesc[FL_HP] += TRnd(20) + 20 + cdesc[FL_LEVEL]) >
			           cdesc[FL_HPMAX]) {
				cdesc[FL_HP] = cdesc[FL_HPMAX];
			}
			break;
		case 2:
			fl_display_message("\nSuddenly, you feel much more skillful!");
			CoreFuncs.IncreasePlayerLevel();
			TRaiseMaxHealth(1);
			return;
		case 3:
			fl_display_message("\nYou feel strange for a moment");
			cdesc[TRund(6)]++;
			break;
		case 4:
			fl_display_message("\nYou feel more self confident!");
			cdesc[FL_WISDOM] += TRnd(2);
			break;
		case 5:
			fl_display_message("\nWow!  You feel great!");
			if (cdesc[FL_STRENGTH] < 12) {
				cdesc[FL_STRENGTH] = 12;
			} else {
				cdesc[FL_STRENGTH]++;
			}
			break;
		case 6:
			fl_display_message("\nYour charm went up by one!");
			cdesc[FL_CHARISMA]++;
			break;
		case 7:
			fl_display_message("\nYou become dizzy!");
			if (--cdesc[FL_STRENGTH] < 3) {
				cdesc[FL_STRENGTH] = 3;
			}
			break;
		case 8:
			fl_display_message("\nYour intelligence went up by one!");
			cdesc[FL_INTELLIGENCE]++;
			break;
		case 9:
			fl_display_message("\nYou sense the presence of objects!");
			fl_wait(FL_WAIT_DURATION);
			if (cdesc[FL_BLINDCOUNT]) {
				return;
			}
			for (i = 0; i < FL_MAX_VERTICAL_POSITION; i++)
				for (j = 0; j < FL_MAX_HORIZONTAL_POSITION; j++)
					switch (object_identification[j][i]) {
						case OPLATE:
						case OCHAIN:
						case OLEATHER:
						case ORING:
						case OSTUDLEATHER:
						case OSPLINT:
						case OPLATEARMOR:
						case OFIRETUNIC:
						case OSSPLATE:
						case OSHIELD:
						case OSWORDofSLASHING:
						case OHAMMER:
						case OSWORD:
						case O2SWORD:
						case OHSWORD:
						case OSPEAR:
						case ODAGGER:
						case OBATTLEAXE:
						case OLONGSWORD:
						case OGREATSWORD:
						case ORINGOFEXTRA:
						case OREGENRING:
						case OPROTRING:
						case OENERGYRING:
						case ODEXRING:
						case OSTRRING:
						case OCLEVERRING:
						case ODAMRING:
						case OBELT:
						case OSCROLL:
						case OPOTION:
						case OBOOK:
						case OPRAYERBOOK:
						case OCHEST:
						case OAMULET:
						case OORBOFDRAGON:
						case OSPIRITSCARAB:
						case OCUBEofUNDEAD:
						case ONOTHEFT:
						case OCOOKIE:
							been_here_before[j][i] = HAVESEEN;
							fl_show_designated_cell_only(j, i);
							break;
					}
			showplayer();
			return;
		case 10:			/* monster detection */
			fl_display_message("\nYou detect the presence of monsters!");
			fl_wait(FL_WAIT_DURATION);
			if (cdesc[FL_BLINDCOUNT]) {
				return;
			}
			for (i = 0; i < FL_MAX_VERTICAL_POSITION; i++)
				for (j = 0; j < FL_MAX_HORIZONTAL_POSITION; j++)
					if (monster_identification[j][i] && (monstnamelist[monster_identification[j][i]] != floorc)) {
						been_here_before[j][i] = HAVESEEN;
						fl_show_designated_cell_only(j, i);
					}
			return;
		case 11:
			fl_display_message("\nYou stagger for a moment . .");
			for (i = 0; i < FL_MAX_VERTICAL_POSITION; i++)
				for (j = 0; j < FL_MAX_HORIZONTAL_POSITION; j++) {
					been_here_before[j][i] = 0;
				}
			fl_wait(1000);
			draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);	/* potion of forgetfulness */
			return;
		case 12:
			fl_display_message("\nThis potion has no taste to it");
			return;
		case 13:
			fl_display_message("\nYou can't see anything!");	/* blindness */
			cdesc[FL_BLINDCOUNT] += 500;
			return;
		case 14:
			fl_display_message("\nYou feel confused");
			cdesc[FL_CONFUSE] += 20 + TRnd(9);
			return;
		case 15:
			fl_display_message("\nWOW!!!  You feel Super-fantastic!!!");
			if (cdesc[FL_HERO] == 0)
				for (i = 0; i < 6; i++) {
					cdesc[i] += 11;
				}
			cdesc[FL_HERO] += 250;
			break;
		case 16:
			fl_display_message("\nYou have a greater intestinal constitude!");
			cdesc[FL_CONSTITUTION]++;
			break;
		case 17:
			fl_display_message("\nYou now have incredibly bulging muscles!!!");
			if (cdesc[FL_GIANTSTR] == 0) {
				cdesc[FL_EXTRA_STRENGTH] += 21;
			}
			cdesc[FL_GIANTSTR] += 700;
			break;
		case 18:
			fl_display_message("\nYou feel a chill run up your spine!");
			cdesc[FL_FIRERESISTANCE] += 1000;
			break;
		case 19:
			fl_display_message("\nYou feel greedy . . .");
			fl_wait(FL_WAIT_DURATION);
			if (cdesc[FL_BLINDCOUNT]) {
				return;
			}
			for (i = 0; i < FL_MAX_VERTICAL_POSITION; i++)
				for (j = 0; j < FL_MAX_HORIZONTAL_POSITION; j++) {
					k = object_identification[j][i];
					if ((k == ODIAMOND) ||
					        (k == ORUBY) ||
					        (k == OEMERALD) ||
					        (k == OMAXGOLD) ||
					        (k == OSAPPHIRE) || (k == OLARNEYE) || (k == OGOLDPILE)) {
						been_here_before[j][i] = HAVESEEN;
						fl_show_designated_cell_only(j, i);
					}
				}
			showplayer();
			return;
		case 20:
			fl_display_message("\nYou feel all better now!");
			cdesc[FL_HP] = cdesc[FL_HPMAX];
			break;			/* instant healing */
		case 21:
			fl_display_message("\nYou don't seem to be affected");
			return;			/* cure dianthroritis */
		case 22:
			fl_display_message("\nYou feel a sickness engulf you");	/* poison */
			cdesc[FL_HALFDAM] += 200 + TRnd(200);
			return;
		case 23:
			fl_display_message("\nYou feel your vision sharpen");	/* see invisible */
			cdesc[FL_SEEINVISIBLE] += TRnd(1000) + 400;
			monstnamelist[INVISIBLEDIWATA] = 'I';
			return;
	};
	bottomline();		/* show new stats      */
	return;
}



/*
    function to process a magic scroll
*/
static void
fl_scroll(int typ) {
	fl_display_message("\nDo you ");
	if (cdesc[FL_BLINDCOUNT] == 0) {
		fl_display_message("(r) read it, ");
	}
	fl_display_message("(t) take it");
	fl_display_message(", or (i) ignore it? ");
	for (;;) {
		switch (ttgetch()) {
			case '\33':
			case 'i':
				fl_display_message("ignore\n");
				return;
			case 'r':
				if (cdesc[FL_BLINDCOUNT]) {
					break;
				}
				fl_display_message("read");
				TForgetData(0);
				if (typ == 2 || typ == 15) {
					fl_show_designated_cell_only(player_horizontal_position, player_vertical_position);
					fl_termcap_cursor_position(1, 24);
				}
				/* destroy it  */ fl_read_scroll(typ);
				return;
			case 't':
				fl_display_message("take");
				if (take(OSCROLL, typ) == 0) {
					TForgetData(0);   /* destroy it  */
				}
				return;
		};
	}
}

/*
    data for the function to read a scroll
*/
static int xh, yh, yl, xl;
static int curse[] = { FL_BLINDCOUNT, FL_CONFUSE, FL_AGGRAVATE, FL_HASTEMONST, FL_ITCHING,
                       FL_LAUGHING, FL_DRAIN_STRENGTH, FL_CLUMSINESS, FL_INFEEBLEMENT,
                       FL_HALFDAM
                     };

static int exten[] = { FL_PROTECTIONTIME, FL_DEXCOUNT, FL_STRCOUNT, FL_CHARMCOUNT,
                       FL_INVISIBILITY, FL_CANCELLATION, FL_HASTESELF, FL_GLOBE,
                       FL_SCAREMONST, FL_HOLDMONST, FL_TIMESTOP
                     };

static int time_change[] = {
	FL_HASTESELF, FL_HERO, FL_ALTPRO, FL_PROTECTIONTIME, FL_DEXCOUNT,
	FL_STRCOUNT, FL_GIANTSTR, FL_CHARMCOUNT, FL_INVISIBILITY,
	FL_CANCELLATION, FL_HASTESELF, FL_AGGRAVATE, FL_SCAREMONST,
	FL_STEALTH, FL_AWARENESS, FL_HOLDMONST, FL_HASTEMONST,
	FL_FIRERESISTANCE, FL_GLOBE, FL_SPIRITPRO, FL_UNDEADPRO,
	FL_HALFDAM, FL_SEEINVISIBLE, FL_ITCHING, FL_CLUMSINESS, FL_WALK_THROUGH_WALLS
};



/*
    function to adjust time when time warping and taking courses in school
*/
void
fl_adjust_time(int tim) {
	FLHunger FLHunger;
	int j;
	for (j = 0; j < 26;
	        j++)	/* adjust time related parameters */
		if (cdesc[time_change[j]])
			if ((cdesc[time_change[j]] -= tim) < 1) {
				cdesc[time_change[j]] = 1;
			}
	fl_regen_hp_and_spells();
	FLHunger.HungerLose();
}

/*
    function to read a scroll
*/
void
fl_read_scroll(int typ) {
	int i, j;
	if (typ < 0 || typ >= MAXSCROLL) {
		return;  /* be sure we are within bounds */
	}
	scrollname[typ][0] = ' ';
	switch (typ) {
		case 0:
			fl_display_message("\nYour armor glows for a moment");
			fl_enchant_armor();
			return;
		case 1:
			fl_display_message("\nYour weapon glows for a moment");
			fl_enchant_weapon();
			return;			/* enchant weapon */
		case 2:
			fl_display_message("\nYou have been granted enlightenment!");
			yh = TMathMin(player_vertical_position + 7, FL_MAX_VERTICAL_POSITION);
			xh = TMathMin(player_horizontal_position + 25, FL_MAX_HORIZONTAL_POSITION);
			yl = TMathMax(player_vertical_position - 7, 0);
			xl = TMathMax(player_horizontal_position - 25, 0);
			for (i = yl; i < yh; i++)
				for (j = xl; j < xh; j++) {
					been_here_before[j][i] = KNOWALL;
				}
			draws(xl, xh, yl, yh);
			refresh();
			return;
		case 3:
			fl_display_message("\nThis scroll seems to be blank");
			return;
		case 4:
			fl_create_a_monster(fl_create_monster(level + 1));
			return;			/* this one creates a monster  */
		case 5:
			fl_create_rand_object_around_player_location(level);	/* create artifact     */
			return;
		case 6:
			fl_display_message("\nSomething isn't right...");
			cdesc[FL_AGGRAVATE] += 800;
			return;			/* aggravate monsters */
		case 7:
			gtime += (i = TRnd(1000) - 850);	/* time warp */
			if (i >= 0)
				lprintf("\nYou went forward in time by %d mobuls",
				        (int)((i + 99) / 100));
			else
				lprintf("\nYou went backward in time by %d mobuls",
				        (int)(- (i + 99) / 100));
			fl_adjust_time((int) i);	/* adjust time for time warping */
			return;
		case 8:
			fl_display_message("\nYour surroundings change");
			fl_teleport(0);
			refresh();
			return;			/* teleportation */
		case 9:
			fl_display_message("\nYou feel extra alert");
			cdesc[FL_AWARENESS] += 1800;
			return;			/* expanded awareness   */
		case 10:
			fl_display_message("\nSomething isn't right...");
			cdesc[FL_HASTEMONST] += TRnd(55) + 12;
			return;			/* haste monster */
		case 11:
			fl_display_message("\nSomething isn't right...");
			for (i = 0; i < FL_MAX_VERTICAL_POSITION; i++)
				for (j = 0; j < FL_MAX_HORIZONTAL_POSITION; j++)
					if (monster_identification[j][i]) {
						monster_hit_points[j][i] = monster[monster_identification[j][i]].hitpoints;
					}
			return;			/* monster healing */
		case 12:
			cdesc[FL_SPIRITPRO] += 300 + TRnd(200);
			bottomline();
			return;			/* spirit protection */
		case 13:
			cdesc[FL_UNDEADPRO] += 300 + TRnd(200);
			bottomline();
			return;			/* undead protection */
		case 14:
			cdesc[FL_STEALTH] += 250 + TRnd(250);
			bottomline();
			return;			/* stealth */
		case 15:
			fl_display_message("\nYou have been granted enlightenment!");	/* magic mapping */
			for (i = 0; i < FL_MAX_VERTICAL_POSITION; i++)
				for (j = 0; j < FL_MAX_HORIZONTAL_POSITION; j++) {
					been_here_before[j][i] = KNOWALL;
				}
			draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);
			refresh();
			return;
		case 16:
			cdesc[FL_HOLDMONST] += 30;
			bottomline();
			return;			/* hold monster */
		case 17:
			fl_display_message("\nYou feel someone eyeing your belongings");
			for (i = 0; i < 26; i++)	/* gem perfection */
				switch (iven[i]) {
					case ODIAMOND:
					case ORUBY:
					case OEMERALD:
					case OSAPPHIRE:
						j = ivenarg[i];
						j &= 255;
						j <<= 1;
						if (j > 255) {
							j = 255;  /* double value */
						}
						ivenarg[i] = j;
						break;
				}
			break;
		case 18:
			fl_display_message("\nYou feel a twitch at the base of your skull");
			for (i = 0; i < 11; i++) {
				cdesc[exten[i]] <<= 1;  /* spell extension */
			}
			break;
		case 19:
			fl_display_message("\nYou feel someone eyeing your belongings");
			for (i = 0; i < 26; i++) {	/* identify */
				if (iven[i] == OPOTION) {
					potionname[ivenarg[i]][0] = ' ';
				}
				if (iven[i] == OSCROLL) {
					scrollname[ivenarg[i]][0] = ' ';
				}
			}
			break;
		case 20:
			fl_display_message("\nYou sense a benign presence");
			for (i = 0; i < 10; i++)	/* remove curse */
				if (cdesc[curse[i]]) {
					cdesc[curse[i]] = 1;
				}
			break;
		case 21:
			fl_do_annihilation();
			break;			/* scroll of annihilation */
		case 22:
			fl_go_direction(22, 150, "The ray hits the %s", 0,
			                ' ');	/* pulverization */
			break;
		case 23:
			fl_display_message("\nYou sense a benign presence");
			cdesc[FL_LIFE_PROTECTION]++;
			break;			/* life protection */
	};
}

/* FIXES for bool bug */
static void
fl_non_bottomless_pit(void) {
	FLCoreFuncs CoreFuncs;
	int i;
	if (TRnd(101) < 81) {
		if (TRnd(70) > 9 * cdesc[FL_DEXTERITY] - packweight()
		        || TRnd(101) < 5) {
			if (level == FL_MAX_CAVE_OF_LARN_LEVEL - 1) {
				fl_bottomless_pit();
			} else if (level == FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL - 1) {
				fl_bottomless_pit();
			} else {
				if (TRnd(101) < 20) {
					i = 0;
					fl_display_message
					("\nYou fell into a pit!  Your fall is cushioned by an unknown force\n");
				} else {
					i = TRnd(level * 3 + 3);
					lprintf
					("\nYou fell into a pit!  You suffer %ld hit points damage",
					 (long) i);
					lastnum = 261;	/* if he dies scoreboard * will say so */
				}
				CoreFuncs.DecreasePHealth(i);
				fl_wait(2000);
				fl_generate_a_new_dungeon_level(level + 1);
				draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);
			}
		}
	}
}

static void
fl_bottomless_pit(void) {
	fl_display_message("\nYou fell into a bottomless pit!");
	fl_wait(3000);
	fl_player_death(262);
}


static void
fl_statue(void) {
}

/* I will add something here, for now it's a placeholder -Gibbon*/
static void
fl_mirror(void) {
	fl_display_message("\nMirror mirror on the wall..");
}

static void
fl_book(void) {
	fl_display_message("\nDo you ");
	if (cdesc[FL_BLINDCOUNT] == 0) {
		fl_display_message("(r) read it, ");
	}
	fl_display_message("(t) take it");
	fl_display_message(", or (i) ignore it? ");
	for (;;) {
		switch (ttgetch()) {
			case '\33':
			case 'i':
				fl_display_message("ignore\n");
				return;
			case 'r':
				if (cdesc[FL_BLINDCOUNT]) {
					break;
				}
				fl_display_message("read");
				/* no more book */ fl_read_a_book(object_argument[player_horizontal_position][player_vertical_position]);
				TForgetData(0);
				return;
			case 't':
				fl_display_message("take");
				if (take(OBOOK, object_argument[player_horizontal_position][player_vertical_position]) == 0) {
					TForgetData(0);   /* no more book */
				}
				return;
		};
	}
}

static void
fl_candle(void) {
fl_display_message("\nDo you ");
fl_display_message("(p) say a protection prayer, ");
fl_display_message("(w) say a wisdom prayer?");
for (;;) {
switch (ttgetch()) {
			case '\33':
			case 'p':
			fl_display_message("\nYou feel protected by a spiritual power!");
			cdesc[FL_LIFE_PROTECTION]++;
			bottomline();
			TForgetData(0);
			return;
			case 'w':
			fl_display_message("\nYour Wisdom has increased by 10!");
			cdesc[FL_WISDOM] += 10;
			bottomline();
			TForgetData(0);
			return;
			break;
		};
	}
}

static void
fl_prayer_book(void) {
	fl_display_message("\nDo you ");
	if (cdesc[FL_BLINDCOUNT] == 0) {
		fl_display_message("(r) read it, ");
	}
	fl_display_message("(t) take it");
	fl_display_message(", or (i) ignore it? ");
	for (;;) {
		switch (ttgetch()) {
			case '\33':
			case 'i':
				fl_display_message("ignore\n");
				return;
			case 'r':
				if (cdesc[FL_BLINDCOUNT]) {
					break;
				}
				fl_display_message("read");
				/* no more book */ fl_read_a_prayer_book(object_argument[player_horizontal_position][player_vertical_position]);
				TForgetData(0);
				return;
			case 't':
				fl_display_message("take");
				if (take(OPRAYERBOOK, object_argument[player_horizontal_position][player_vertical_position]) == 0) {
					TForgetData(0);   /* no more book */
				}
				return;
		};
	}
}

/*
    function to read a book
*/
void
fl_read_a_book(int lev) {
	int i, tmp;
	if (lev <= 3) {
		tmp = splev[lev];
		if (tmp == 0) {
			tmp = 1;
		}
		i = TRund(tmp);
	} else {
		tmp = splev[lev] - 9;
		if (tmp == 0) {
			tmp = 1;
		}
		i = TRnd(tmp + 9);
	}
	spelknow[i] = 1;
	lprintf("\nSpell \"%s\":  %s\n%s", spelcode[i],
	        spelname[i],
	        speldescript[i]);
	if (TRnd(10) == 4) {
		fl_display_message("\nYour int went up by one!");
		cdesc[FL_INTELLIGENCE]++;
		bottomline();
	}
}

/*
    function to read a prayer book
*/
void
fl_read_a_prayer_book(int lev) {
	int tmp;
	if (lev <= 3) {
		tmp = splev[lev];
		if (tmp == 0) {
			tmp = 1;
		}
		TRund(tmp);
	} else {
		tmp = splev[lev] - 9;
		if (tmp == 0) {
			tmp = 1;
		}
		TRnd(tmp + 9);
	}
	if (TRnd(10) < 4) {
		fl_display_message("\nYou mutter an ancient prayer and feel stronger!");
		cdesc[FL_STRENGTH]++;
		bottomline();
		}
    if (TRnd(10) > 4) {
		fl_display_message("\nYou read ancient prayers and feel stronger!");
		cdesc[FL_INTELLIGENCE]++;
		bottomline();
		}
	if (TRnd(10) == 4) {
		fl_display_message("\nYour WISDOM has increased by one!");
		cdesc[FL_WISDOM]++;
		bottomline();
	}
}

static void
fl_fortune_cookie(void) {
	fl_display_message("\nDo you (e) eat it, (t) take it");
	fl_display_message(", or (i) ignore it? ");
	for (;;) {
		switch (ttgetch()) {
			case '\33':
			case 'i':
				fl_display_message("ignore\n");
				return;
			case 'e':
				fl_display_message("eat");
				TForgetData(0);		/* no more cookie */
				return;
			case 't':
				fl_display_message("take");
				if (take(OCOOKIE, 0) == 0) {
					TForgetData(0);   /* no more book */
				}
				return;
		};
	}
}



/*
    routine to pick up some gold -- if arg==OMAXGOLD then the pile is worth
    100* the argument
*/
static void
fl_gold(int arg) {
	int a;
	a = object_argument[player_horizontal_position][player_vertical_position];
	if (arg == OMAXGOLD) {
		a *= 100;
	} else if (arg == OKGOLD) {
		a *= 1000;
	} else if (arg == ODGOLD) {
		a *= 10;
	}
	cdesc[FL_GOLD] += a;
	fl_display_message("\nIt is worth %d. You have a total of %-6d", a, cdesc[FL_GOLD]);
	object_identification[player_horizontal_position][player_vertical_position] = been_here_before[player_horizontal_position][player_vertical_position] = 0;	/* destroy gold    */
}

void
fl_player_home(void) {
	int i;
	for (i = 0; i < MAXINVEN; i++)
		if (iven[i] == OPOTION)
			if (ivenarg[i] == 21) {
				iven[i] = 0;
				TClearAndResetScreen(FL_CLEAR);
				lprintf
				("Congratulations.  You found a potion of cure dianthroritis.\n");
				lprintf
				("\nFrankly, No one thought you could do it.  Boy!  Did you surprise them!\n");
				if (gtime > FL_TOTAL_MOBUL_LIMIT) {
					lprintf
					("\nThe doctor has the sad duty to inform you that your daughter passed away..");
					lprintf
					("\nbefore your return.  There was nothing he could do without the potion.\n");
					fl_wait(FL_WAIT_DURATION);
					fl_player_death(269);
				} else {
					lprintf
					("\nThe doctor is now administering the potion, and in a few moments\n");
					lprintf
					("your daughter should be well on her way to recovery.\n");
					fl_wait(FL_WAIT_DURATION);
					lprintf("\nThe potion is");
					fl_wait(FL_WAIT_DURATION);
					lprintf(" working!  The doctor thinks that\n");
					lprintf
					("your daughter will recover in a few days.  Congratulations!\n");
					fl_wait(FL_WAIT_DURATION);
					fl_player_death(263);
				}
			}
	for (;;) {
		TClearAndResetScreen(FL_CLEAR);
		lprintf("Welcome home %s.  Latest word from the doctor is not good.\n",
		        logname);
		if (gtime > FL_TOTAL_MOBUL_LIMIT) {
			lprintf
			("\nThe doctor has the sad duty to inform you that your daughter passed away..\n");
			lprintf
			("You didn't make it in time.  There was nothing he could do without the potion.\n");
			fl_wait(FL_WAIT_DURATION);
			fl_player_death(269);
		}
		lprintf
		("\nThe diagnosis is confirmed as dianthroritis.  He guesses that\n");
		lprintf
		("your daughter has only %d mobuls left in this world.  It's up to you,\n",
		 ((FL_TOTAL_MOBUL_LIMIT - gtime + 99) / 100));
		lprintf("%s, to find the only hope for your daughter, the very rare\n",
		        logname);
		lprintf
		("potion of cure dianthroritis.  It is rumored that only deep in the\n");
		lprintf("depths of the caves can this potion be found.\n\n\n");
		lprintf("\n     ----- press ");
		fl_invert_color_space("return");
		lprintf(" to continue, ");
		fl_invert_color_space("escape");
		lprintf(" to leave ----- ");
		i = ttgetch();
		while (i != '\33' && i != '\n') {
			i = ttgetch();
		}
		drawscreen();
		return;
	}
}

/*
    For prompt mode, prompt for entering a building.
*/
static void
fl_prompt_for_entrance(void) {
	FLStore Store;
	char i;
	fl_display_message("\nDo you (g) go inside, or (i) stay here? ");
	i = 0;
	while ((i != 'g') && (i != 'i') && (i != '\33')) {
		i = ttgetch();
	}
	if (i == 'g') {
		fl_termcap_cursor_position(1, 24);
		switch (object_identification[player_horizontal_position][player_vertical_position]) {
			case OBANK:
				banktitle("    Welcome to the First National Bank of Larn.");
				break;
			case OBANK2:
				banktitle("Welcome to the 5th level branch office of "
				          "the First National Bank of Larn.");
				/*  because we state the level in the title, clear the '?' in the
				    level display at the bottom, if the user teleported.
				*/
				cdesc[FL_TELEPORT_FLAG] = 0;
				break;
			case ODNDSTORE:
				Store.DNDStore();
				break;
			case OENTRANCE:
				/*  place player in front of entrance on level 1.  fl_generate_a_new_dungeon_level()
				    prevents player from landing on a monster/object.
				*/
				player_horizontal_position = 33;
				player_vertical_position = FL_MAX_VERTICAL_POSITION - 2;
				fl_generate_a_new_dungeon_level(1);
				been_here_before[33][FL_MAX_VERTICAL_POSITION - 1] = KNOWALL;
				monster_identification[33][FL_MAX_VERTICAL_POSITION - 1] = 0;
				draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);
				showcell(player_horizontal_position, player_vertical_position);	/* to show around player */
				bot_linex();
				break;
			case OTRADEPOST:
				Store.TradingPost();
				break;
			case OLRS:
				Store.TaxOffice();
				break;
			case OHOME:
				fl_player_home();
				break;
			case FL_OBJECT_TEMPLE_IN:
				fl_a_enter_the_temple();
				break;
			case FL_OBJECT_TEMPLE_OUT:
				fl_a_exit_the_temple();
				break;
			default:
				fl_display_message("\nThere is no place to enter here!\n");
				break;
		}
	} else {
		fl_display_message(" stay here");
	}
}

static void
fl_prompt_for_temple_entrance(int sphere_direction) {
	char i;
	fl_display_message("\nDo you (e) enter ");
	if (sphere_direction > 0) {
		fl_display_message("enter");
	} else {
		fl_display_message("leave");
	}
	fl_display_message(", or (i) ignore it? ");
	i = 0;
	while ((i != 'e') && (i != 'i') && (i != '\33')) {
		i = ttgetch();
	}
	if ((i == '\33') || (i == 'i')) {
		fl_display_message("ignore\n");
		return;
	}
	if (sphere_direction > 0) {
		fl_a_exit_the_temple();
	} else {
		fl_a_enter_the_temple();
	}
}

static void
fl_open_door(void) {
	char i;
	fl_display_message("\nDo you (c) close it");
	fl_display_message(", or (i) ignore it? ");
	i = 0;
	while ((i != 'c') && (i != 'i') && (i != '\33')) {
		i = ttgetch();
	}
	if ((i == '\33') || (i == 'i')) {
		fl_display_message("ignore\n");
		return;
	}
	fl_display_message("close");
	TForgetData(0);
	object_identification[player_horizontal_position][player_vertical_position] = OCLOSEDDOOR;
	object_argument[player_horizontal_position][player_vertical_position] = 0;
	player_horizontal_position = lastpx;
	player_vertical_position = lastpy;
}



static void
fl_closed_door(void) {
	char i;
	fl_display_message("\nDo you (o) try to open it");
	fl_display_message(", or (i) ignore it? ");
	i = 0;
	while ((i != 'o') && (i != 'i') && (i != '\33')) {
		i = ttgetch();
	}
	if ((i == '\33') || (i == 'i')) {
		fl_display_message("ignore\n");
		player_horizontal_position = lastpx;
		player_vertical_position = lastpy;
		return;
	} else {
		fl_display_message("open");
		/*
		    if he failed to open the door ...
		*/
		if (!fl_a_open_dungeon_door(player_horizontal_position, player_vertical_position)) {
			player_horizontal_position = lastpx;
			player_vertical_position = lastpy;
		}
	}
}
