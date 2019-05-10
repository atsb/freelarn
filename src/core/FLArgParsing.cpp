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

#include "FLHelp.hpp"
#include "FLInventory.hpp"
#include "FLTermCapIO.hpp"
#include "FLScores.hpp"
#include "FLFuncs.hpp"
#include "FLStore.hpp"
#include "../config/FLIncludeData.hpp"
#include "../lexical/FLTok.hpp"
#include "../dungeon/FLDungeon.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../player/FLPlayerActions.hpp"
#include "../player/FLRegenerate.hpp"
#include "../save/FLSave.hpp"
#include "../player/FLHunger.hpp"
#include "../terminal/FLGetChar.hpp"

extern char viewflag;

void
parse(void) {
	FLCoreFuncs CoreFuncs;
	FLStore Store;
	int i, j, k, flag;
	Save save;
	for (;;) {
		k = yylex();
		switch (k) {	/*  get the token from the input and switch on it   */
			case 'h':
				moveplayer(4);
				return;		/*  west        */
			case 'l':
				moveplayer(2);
				return;		/*  east        */
			case 'j':
				moveplayer(1);
				return;		/*  south       */
			case 'k':
				moveplayer(3);
				return;		/*  north       */
			case 'u':
				moveplayer(5);
				return;		/*  northeast   */
			case 'y':
				moveplayer(6);
				return;		/*  northwest   */
			case 'n':
				moveplayer(7);
				return;		/*  southeast   */
			case 'b':
				moveplayer(8);
				return;		/*  southwest   */
			case '.':		/*  stay here       */
				if (y_larn_rep) {
					viewflag = 1;
				}
				return;
			case 'c':
				y_larn_rep = 0;
				fl_cast_a_spell();
				return;		/*  cast a spell    */
			case 'd':
				y_larn_rep = 0;
				if (cdesc[FL_TIMESTOP] == 0) {
					dropobj();
				}
				return;		/*  to drop an object   */
			case 'e':
				y_larn_rep = 0;
				if (cdesc[FL_TIMESTOP] == 0)
					if (!floor_consume(OCOOKIE, "eat")) {
						consume(OCOOKIE, "eat");
						if (cdesc[FL_HUNGER] < cdesc[FL_HUNGERMAX]) {
							cdesc[FL_HUNGER] += 10;
						} else {
							fl_display_message("\nIt has no affect on your appetite.");
						}
					}
				return;
			case 'g':
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				lprintf("\nThe stuff you are carrying presently weighs %d pounds",
				        (int) packweight());
				break;
			case 'i':		/* inventory */
				y_larn_rep = 0;
				nomove = 1;
				showstr(0);
				return;
			case 'p':		/* pray at an altar */
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] != OALTAR) {
					fl_display_message("\nI see no altar to pray at here!");
				} else {
					fl_a_give_donation();
				}
				prayed = 1;
				return;
			case '~':
				fl_clear_and_reset_screen();
				enable_scroll = 0;
				showscores(0);     /* if we updated the scoreboard */
				fl_termcap_cursor_position(1, 24);
				fl_display_message("\nPress any key to exit. ");
				get_final_char_keyboard = ansiterm_getch;
				ttgetch();
				enable_scroll = 1;
				drawscreen();
				return;
			case 'q':		/* quaff a potion */
				y_larn_rep = 0;
				if (cdesc[FL_TIMESTOP] == 0)
					if (!floor_consume(OPOTION, "quaff")) {
						consume(OPOTION, "quaff");
					}
				return;
			case 'r':
				y_larn_rep = 0;
				if (cdesc[FL_BLINDCOUNT]) {
					fl_termcap_cursor_position(1, 24);
					fl_display_message("\nYou can't read anything when you're blind!");
				} else if (cdesc[FL_TIMESTOP] == 0)
					if (!floor_consume(OSCROLL, "read"))
						if (!floor_consume(OBOOK, "read"))
							if (!floor_consume(OPRAYERBOOK, "read")) {
								consume(OSCROLL, "read");
							}
				return;		/*  to read a scroll    */
			case 's':
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == OTHRONE) {
					fl_a_sit_on_throne(0);
				} else if ((object_identification[player_horizontal_position][player_vertical_position] == OTHRONE2) ||
				           (object_identification[player_horizontal_position][player_vertical_position] == ODEADTHRONE)) {
					fl_a_sit_on_throne(1);
				} else {
					fl_display_message("\nI see no throne to sit on here!");
				}
				return;
			case 't':		/* Tidy up at fountain */
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == ODEADFOUNTAIN) {
					fl_display_message("\nThere is no water to wash in!");
				} else if (object_identification[player_horizontal_position][player_vertical_position] != OFOUNTAIN) {
					fl_display_message("\nI see no fountain to wash at here!");
				} else {
					fl_a_wash_in_fountain();
				}
				return;
			case 'v':
				y_larn_rep = 0;
				nomove = 1;
				fl_termcap_cursor_position(1, 24);
				lprintf("\nVersion: %s", FL_BUILD_VERSION);
				lprintf("\n%s", FL_APACHE2_NOTICE);
				if (wizard) {
					fl_display_message(" Wizard");
				}
				if (cheat) {
					fl_display_message(" Cheater");
				}
				return;
			case 'w':		/*  wield a weapon */
				y_larn_rep = 0;
				wield();
				return;
			case 'A':
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == OALTAR) {
					fl_a_desecrate_an_altar();
				} else {
					fl_display_message("\nI see no altar to desecrate here!");
				}
				return;
			case 'C':		/* Close something */
				y_larn_rep = 0;
				fl_close_an_object();
				return;
			case 'D':		/* Drink at fountain */
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == ODEADFOUNTAIN) {
					fl_display_message("\nThere is no water to drink!");
				} else if (object_identification[player_horizontal_position][player_vertical_position] != OFOUNTAIN) {
					fl_display_message("\nI see no fountain to drink from here!");
				} else {
					fl_a_drink_from_fountain();
				}
				return;
			case '?':
				y_larn_rep = 0;
				display_help_text();
				nomove = 1;
				return;	/*give the help screen*/
			case 'E':		/* Enter a building */
				y_larn_rep = 0;
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
				break;
			case 'I':		/*  list spells and scrolls */
				y_larn_rep = 0;
				seemagic(0);
				nomove = 1;
				return;
			case 'O':		/* Open something */
				y_larn_rep = 0;
				fl_open_an_object();
				return;
			case 'P':
				fl_termcap_cursor_position(1, 24);
				y_larn_rep = 0;
				nomove = 1;
				if (outstanding_taxes > 0)
					lprintf("\nYou presently owe %d gp in taxes.",
					        (int) outstanding_taxes);
				else {
					fl_display_message("\nYou do not owe any taxes.");
				}
				return;
			case 'Q':		/*  quit        */
				y_larn_rep = 0;
				fl_exit_the_game();
				nomove = 1;
				return;
			case 'R':		/* remove gems from a throne */
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == ODEADTHRONE) {
					fl_display_message("\nThere are no gems to remove!");
				} else if (object_identification[player_horizontal_position][player_vertical_position] == OTHRONE) {
					fl_a_remove_the_gems(0);
				} else if (object_identification[player_horizontal_position][player_vertical_position] == OTHRONE2) {
					fl_a_remove_the_gems(1);
				} else {
					fl_display_message("\nI see no throne here to remove gems from!");
				}
				return;
			case 'S':
				/*
				    Added the save stuff
				    It's much better now!
				    ~Gibbon
				*/
				fl_termcap_cursor_position(1, 24);
				fl_output_buffer_flush();
				save_mode = 1;
				save.savegame(savefilename);
				fl_clear_terminal();
				get_final_char_keyboard = ansiterm_getch;
				exit(EXIT_SUCCESS);
				break;
			case 'T':
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (cdesc[FL_SHIELD] != -1) {
					cdesc[FL_SHIELD] = -1;
					fl_display_message("\nYour shield is off");
					bottomline();
				} else if (cdesc[FL_WEAR] != -1) {
					cdesc[FL_WEAR] = -1;
					fl_display_message("\nYour armor is off");
					bottomline();
				} else {
					fl_display_message("\nYou aren't wearing anything");
				}
				return;
			case 'W':
				y_larn_rep = 0;
				wear();
				return;		/*  wear armor  */
			case 'Z':
				y_larn_rep = 0;
				if (cdesc[FL_LEVEL] > 9) {
					fl_teleport(1);
					return;
				}
				fl_termcap_cursor_position(1, 24);
				fl_display_message
				("\nAs yet, you don't have enough experience to use teleportation");
				return;		/*  teleport yourself   */
			case ' ':
				y_larn_rep = 0;
				nomove = 1;
				return;
			case 'L' - 64:
				y_larn_rep = 0;
				drawscreen();
				nomove = 1;
				return;		/*  look        */
			case '<':		/* Go up stairs or vol shaft */
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == OSTAIRSDOWN) {
					fl_display_message("\nThe stairs don't go up!");
				} else if (object_identification[player_horizontal_position][player_vertical_position] != OSTAIRSUP) {
					fl_display_message("\nI see no way to go up here!");
				} else {
					fl_a_travel_up_stairs();
				}
				return;
			case '>':		/* Go down stairs or vol shaft */
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == OSTAIRSUP) {
					fl_display_message("\nThe stairs don't go down!");
				} else if (object_identification[player_horizontal_position][player_vertical_position] != OSTAIRSDOWN) {
					fl_display_message("\nI see no way to go down here!");
				} else {
					fl_a_travel_down_stairs();
				}
				return;
			case ',':		/* pick up an object_identification */
				y_larn_rep = 0;
				/* pickup, don't identify or prompt for action */
				fl_look_for_an_object_and_give_options(0, 1, 0);
				return;
			case ':':		/* look at object */
				y_larn_rep = 0;
				/* identify, don't pick up or prompt for action */
				fl_look_for_an_object_and_give_options(1, 0, 0);
				nomove = 1;		/* assumes look takes no time */
				return;
			case '/':		/* identify object/monster */
				fl_termcap_cursor_position(1, 24);
				fl_display_message("\nIdentify unknown object by cursor [ynq]?");
				for (;;) {
					switch (ttgetch()) {
						case '\33':
						case 'q':
							return;
						case 'y':
						case 'Y':
							specify_obj_cursor();
							return;
						case 'n':
						case 'N':
							specify_obj_nocurs();
							return;
					}
				}
				nomove = 1;
				y_larn_rep = 0;
				return;
			case '^':		/* identify traps */
				flag = y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				fl_output_single_byte_to_output_buffer('\n');
				for (j = player_vertical_position - 1; j < player_vertical_position + 2; j++) {
					if (j < 0) {
						j = 0;
					}
					if (j >= FL_MAX_VERTICAL_POSITION) {
						break;
					}
					for (i = player_horizontal_position - 1; i < player_horizontal_position + 2; i++) {
						if (i < 0) {
							i = 0;
						}
						if (i >= FL_MAX_HORIZONTAL_POSITION) {
							break;
						}
						switch (object_identification[i][j]) {
							case OTRAPDOOR:
							case ODARTRAP:
							case OTRAPARROW:
							case OTELEPORTER:
							case OPIT:
								fl_display_message("\nIts ");
								fl_display_message(objectname[object_identification[i][j]]);
								flag++;
						};
					}
				}
				if (flag == 0) {
					fl_display_message("\nNo traps are visible");
				}
				return;
				if (FL_WIZARD_MODE_ENABLED == true) {
				case '_':		/*  this is the fudge player password for wizard mode */
					y_larn_rep = 0;
					fl_termcap_cursor_position(1, 24);
					nomove = 1;
					wizard = 1;		/* disable to easily test win condition */
					get_final_char_keyboard = ansiterm_getch;		/* system("stty -echo cbreak"); */
					for (i = 0; i < 6; i++) {
						cdesc[i] = 70;
					}
					iven[0] = iven[1] = 0;
					/* give the ring a little boost. ~Gibbon */
					take(OPROTRING, 51);
					/*  lets nerf it a little bit.
					    ~Gibbon */
					take(OGREATSWORD, 24);
					cdesc[FL_WIELD] = 1;
					cdesc[FL_GREAT_SWORD_OF_DEATH] = 1;
					CoreFuncs.IncreaseExperience(6000000L);
					cdesc[FL_AWARENESS] += 25000;
					{
						for (i = 0; i < FL_MAX_VERTICAL_POSITION; i++)
							for (j = 0; j < FL_MAX_HORIZONTAL_POSITION; j++) {
								been_here_before[j][i] = KNOWALL;
							}
						for (i = 0; i < FL_MAX_SPELLS_IN_GAME; i++) {
							spelknow[i] = 1;
						}
						for (i = 0; i < MAXSCROLL; i++) {
							scrollname[i][0] = ' ';
						}
						for (i = 0; i < MAXPOTION; i++) {
							potionname[i][0] = ' ';
						}
					}
					for (i = 0; i < MAXSCROLL; i++)
						/* no null items */
						if (strlen(scrollname[i]) > 2) {
							object_identification[i][0] = OSCROLL;
							object_argument[i][0] = i;
						}
					for (i = FL_MAX_HORIZONTAL_POSITION - 1; i > FL_MAX_HORIZONTAL_POSITION - 1 - MAXPOTION; i--)
						/* no null items */
						if (strlen(potionname[i - FL_MAX_HORIZONTAL_POSITION + MAXPOTION]) > 2) {
							object_identification[i][0] = OPOTION;
							object_argument[i][0] = i - FL_MAX_HORIZONTAL_POSITION + MAXPOTION;
						}
					for (i = 1; i < FL_MAX_VERTICAL_POSITION; i++) {
						object_identification[0][i] = i;
						object_argument[0][i] = 0;
					}
					for (i = FL_MAX_VERTICAL_POSITION; i < FL_MAX_VERTICAL_POSITION + FL_MAX_HORIZONTAL_POSITION; i++) {
						object_identification[i - FL_MAX_VERTICAL_POSITION][FL_MAX_VERTICAL_POSITION - 1] = i;
						object_argument[i - FL_MAX_VERTICAL_POSITION][FL_MAX_VERTICAL_POSITION - 1] = 0;
					}
					for (i = FL_MAX_HORIZONTAL_POSITION + FL_MAX_VERTICAL_POSITION; i < MAXOBJECT; i++) {
						object_identification[FL_MAX_HORIZONTAL_POSITION - 1][i - FL_MAX_HORIZONTAL_POSITION - FL_MAX_VERTICAL_POSITION] = i;
						object_argument[FL_MAX_HORIZONTAL_POSITION - 1][i - FL_MAX_HORIZONTAL_POSITION - FL_MAX_VERTICAL_POSITION] = 0;
					}
					cdesc[FL_GOLD] += 250000;
					drawscreen();
					return;
				}
		};
	}
}

void
parse2(void) {
	FLHunger FLHunger;
	/* move the monsters */
	if (cdesc[FL_HASTEMONST]) {
		fl_move_a_monster();
	}
	fl_move_a_monster();
	if (cdesc[FL_TIMESTOP]) {
		return;
	}
	if (--random_monster_creation <= 0) {
		random_monster_creation = 120 - (level << 2);
		fl_fill_dungeon_with_monsters(fl_create_monster(level));
	}
	fl_regen_hp_and_spells();
	FLHunger.HungerLose();
}