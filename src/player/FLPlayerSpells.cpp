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

#include "../dungeon/FLDungeon.hpp"
#include "../config/FLIncludeData.hpp"
#include "../templates/FLTMath.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../core/FLTermCapIO.hpp"
#include "../player/FLPlayerActions.hpp"
#include "../core/FLScores.hpp"
#include "../core/FLSystemDependant.hpp"
#include "../core/FLFuncs.hpp"

using std::cout;

/* used for altar reality */
struct isave {

	int type;			/* 0=object,  1=monster */
	int id;			/* object number or monster number */

	int arg;			/* the type of object or hitpoints of monster */
};

static void speldamage(int);
static void create_guardian(int, int, int);
static void loseint(void);
static int isconfuse(void);
static int nospell(int, int);
static void direct(int, int, const char*, int);
static void tdirect(int);
static void omnidirect(int, int, const char*);
static void dirpoly(int);
static void genmonst(void);

/*
    fl_cast_a_spell()      Subroutine called by parse to cast a spell for the user

    No arguments and no return value.
*/
static char eys[] = "\nEnter your spell: ";

void
fl_cast_a_spell(void) {
	int i, j, a, b, d;
	fl_termcap_cursor_position(1, 24);
	if (cdesc[FL_SPELLS] <= 0) {
		move(10, 19);
		fl_set_text_to_bold();
		fl_display_message("\nYou don't have any spells!");
		fl_reset_text_bold();
		refresh();
		return;
	}
	fl_display_message(eys);
	--cdesc[FL_SPELLS];
	while ((a = ttgetch()) == 'I') {
		seemagic(-1);
		fl_termcap_cursor_position(1, 24);
		fl_display_message(eys);
	}
	if (a == '\33') {
		fl_display_message(aborted);
		cdesc[FL_SPELLS]++;
		return;
	}
	if ((b = ttgetch()) == '\33') {
		fl_display_message(aborted);
		cdesc[FL_SPELLS]++;
		return;
	}
	if ((d = ttgetch()) == '\33') {
		fl_display_message(aborted);
		cdesc[FL_SPELLS]++;
		return;
	}
	for (fl_output_single_byte_to_output_buffer('\n'), j = -1, i = 0; i < FL_MAX_SPELLS_IN_GAME;
	        i++)	/*seq search for his spell, hash? */
		if ((spelcode[i][0] == a) && (spelcode[i][1] == b)
		        && (spelcode[i][2] == d))
			if (spelknow[i]) {
				speldamage(i);
				j = 1;
				i = FL_MAX_SPELLS_IN_GAME;
			}
	if (j == -1) {
		fl_display_message("Nothing Happened");
	}
	bottomline();
}



/*
    speldamage(x)       Function to perform spell functions cast by the player
       int x;

    Enter with the spell number, returns no value.
    Please insure that there are 2 spaces before all messages here
*/
static void
speldamage(int x) {
	int i, j, clev;
	int xl, xh, yl, yh;
	int *kn, *pm, *p;
	HitMonster hitpoints;
	FLCoreFuncs CoreFuncs;
	if (x >= FL_MAX_SPELLS_IN_GAME) {
		return;  /* no such spell */
	}
	if (cdesc[FL_TIMESTOP]) {
		fl_display_message("  It didn't seem to work");
		return;
	}				/* not if time stopped */
	clev = cdesc[FL_LEVEL];
	if ((TRnd(23) == 7) || (TRnd(18) > cdesc[FL_INTELLIGENCE])) {
		fl_display_message("  It didn't work!");
		return;
	}
	if (clev * 3 + 2 < x) {
		fl_display_message("  Nothing happens.  You seem inexperienced at this");
		return;
	}
	switch (x) {
		/* ----- FL_LEVEL 1 FL_SPELLS ----- */
		case 0:
			if (cdesc[FL_PROTECTIONTIME] == 0) {
				cdesc[FL_MOREDEFENSES] += 2;  /* protection field +2 */
			}
			cdesc[FL_PROTECTIONTIME] += 250;
			return;
		case 1:
			i = TRnd(((clev + 1) << 1)) + clev + 3;
			fl_go_direction(x, i, (clev >= 2) ? "  Your missiles hit the %s" :
			                "  Your missile hit the %s", 100,
			                '+');	/* magic missile */
			return;
		case 2:
			if (cdesc[FL_DEXCOUNT] == 0) {
				cdesc[FL_DEXTERITY] += 3;  /* dexterity   */
			}
			cdesc[FL_DEXCOUNT] += 400;
			return;
		/*  Further fixes below for issue #36.  Removed crusty old 'C' and replaced with
			direct function calls. -Gibbon*/
		case 3:
			i = TRnd(3) + 1;
			/*  Fix for bug #24 added newlines to the 'msg' for web and sleep spells.
			    Removed the msg and used fl_display_message instead plus color. ~Gibbon*/
			direct(x, fl_is_fullhit(i),
			       "\nwhile the %s slept, you hit %d times ",
			       i);	/*    sleep   */
			return;
		case 4:			/*  charm monster   */
			cdesc[FL_CHARMCOUNT] += cdesc[FL_CHARISMA] << 1;
			return;
		case 5:
			fl_go_direction(x, TRnd(10) + 15 + clev,
			                "  The sound damages the %s", 70,
			                '@');	/* sonic spear */
			return;
		/* ----- FL_LEVEL 2 FL_SPELLS ----- */
		case 6:
			i = TRnd(3) + 2;
			direct(x, fl_is_fullhit(i),
			       "\nYou damage the %s and hit %d times ",
			       i);			/* web */
			return;
		case 7:
			if (cdesc[FL_STRCOUNT] == 0) {
				cdesc[FL_EXTRA_STRENGTH] += 3;  /*  strength    */
			}
			cdesc[FL_STRCOUNT] += 150 + TRnd(100);
			return;
		case 8:
			yl = player_vertical_position - 5;		/* enlightenment */
			yh = player_vertical_position + 6;
			xl = player_horizontal_position - 15;
			xh = player_horizontal_position + 16;
			fl_verify_bound_coordinates(&xl, &yl);
			fl_verify_bound_coordinates(&xh, &yh);
			for (i = yl; i <= yh; i++)	/* enlightenment */
				for (j = xl; j <= xh; j++) {
					been_here_before[j][i] = KNOWALL;
				}
			draws(xl, xh + 1, yl, yh + 1);
			return;
		case 9:
			CoreFuncs.IncreasePHealth(20 + (clev << 1));
			return;			/* healing */
		case 10:
			cdesc[FL_BLINDCOUNT] = 0;
			return;			/* cure blindness   */
		case 11:
			fl_create_a_monster(fl_create_monster(level + 1) + 8);
			return;
		case 12:
			if (TRnd(11) + 7 <= cdesc[FL_WISDOM]) {
				direct(x, TRnd(20) + 20 + clev, "\nThe %s believed!", 0);
			} else {
				fl_display_message("\n It didn't believe the illusions!");
			}
			return;
		case 13:			/* if he has the amulet of invisibility then add more time */
			for (j = i = 0; i < 26; i++)
				if (iven[i] == OAMULET) {
					j += 1 + ivenarg[i];
				}
			cdesc[FL_INVISIBILITY] += (j << 7) + 12;
			return;
		/* ----- FL_LEVEL 3 FL_SPELLS ----- */
		case 14:
			fl_go_direction(x, TRnd(25 + clev) + 25 + clev,
			                "\nThe fireball hits the %s",
			                40, '*');
			return;			/*    fireball */
		case 15:
			fl_go_direction(x, TRnd(25) + 20 + clev,
			                "\nYour cone of cold strikes the %s", 60, 'O');	/*  cold */
			return;
		case 16:
			dirpoly(x);
			return;			/*  polymorph */
		case 17:
			cdesc[FL_CANCELLATION] += 5 + clev;
			return;			/*  cancellation    */
		case 18:
			cdesc[FL_HASTESELF] += 7 + clev;
			return;			/* haste self  */
		case 19:
			omnidirect(x, 30 + TRnd(10),
			           "\nThe %s gasps for air");	/* cloud kill */
			return;
		case 20:
			xh = TMathMin(player_horizontal_position + 1, FL_MAX_HORIZONTAL_POSITION - 2);
			yh = TMathMin(player_vertical_position + 1, FL_MAX_VERTICAL_POSITION - 2);
			for (i = TMathMax(player_horizontal_position - 1, 1); i <= xh;
			        i++)	/* vaporize rock */
				for (j = TMathMax(player_vertical_position - 1, 1); j <= yh; j++) {
					kn = &been_here_before[i][j];
					pm = &monster_identification[i][j];
					switch (* (p = &object_identification[i][j])) {
						case OWALL:
							if (level < FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL - 1) {
								*p = *kn = 0;
							}
							break;
						case OSTATUE:
							*p = OBOOK;
							object_argument[i][j] = level;
							*kn = 0;
							break;
						case OTHRONE:
							*p = OTHRONE2;
							create_guardian(GNOMEKING, i, j);
							break;
						case OALTAR:
							create_guardian(DEMONPRINCE, i, j);
							break;
						case OFOUNTAIN:
							create_guardian(WATERLORD, i, j);
							break;
					};
					switch (*pm) {
						case XORN:
							fl_player_is_blind(i, j);
							hitpoints.hitm(i, j, 200);
							break;		/* Xorn takes damage from vpr */
					}
				}
			return;
		/* ----- FL_LEVEL 4 FL_SPELLS ----- */
		case 21:
			direct(x, 100 + clev, "\nThe %s shrivels up",
			       0);	/* dehydration */
			return;
		case 22:
			fl_go_direction(x, TRnd(25) + 20 + (clev << 1),
			                "\nA lightning bolt hits the %s", 1,
			                '~');	/*  lightning */
			return;
		case 23:
			i = TMathMin(cdesc[FL_HP] - 1, cdesc[FL_HPMAX] / 2);	/* drain life */
			direct(x, i + i, "", 0);
			cdesc[FL_HP] -= i;
			return;
		case 24:
			if (cdesc[FL_GLOBE] == 0) {
				cdesc[FL_MOREDEFENSES] += 10;
			}
			cdesc[FL_GLOBE] += 200;
			loseint();		/* globe of invulnerability */
			return;
		case 25:
			omnidirect(x, 32 + clev,
			           "\nThe %s struggles for air in your flood!");	/* flood */
			return;
		case 26:
			if (TRnd(151) == 63) {
				fl_display_message("\nYour heart stopped!\n");
				fl_wait(FL_WAIT_DURATION);
				fl_player_death(270);
				return;
			}
			if (cdesc[FL_WISDOM] > TRnd(10) + 10)
				direct(x, 2000, " \nThe %s's heart stopped",
				       0);	/* finger of death */
			else {
				fl_display_message(" It didn't work");
			}
			return;
		/* ----- FL_LEVEL 5 FL_SPELLS ----- */
		case 27:
			cdesc[FL_SCAREMONST] += TRnd(10) + clev;
			return;			/* scare monster */
		case 28:
			cdesc[FL_HOLDMONST] += TRnd(10) + clev;
			return;			/* hold monster */
		case 29:
			cdesc[FL_TIMESTOP] += TRnd(20) + (clev << 1);
			return;			/* time stop */
		case 30:
			tdirect(x);
			return;			/* teleport away */
		case 31:
			omnidirect(x, 35 + TRnd(10) + clev,
			           "\nThe %s cringes from the flame");	/* magic fire */
			return;
		/* ----- FL_LEVEL 6 FL_SPELLS ----- */
		case 32:
			if ((TRnd(23) == 5)
			        && (wizard == 0)) {	/* sphere of annihilation */
				fl_display_message("\n You have been enveloped by the zone of nothingness!\n");
				fl_wait(FL_WAIT_DURATION);
				fl_player_death(258);
				return;
			}
			xl = player_horizontal_position;
			yl = player_vertical_position;
			loseint();
			i = fl_direction(&xl, &yl);	/* get direction of sphere */
			fl_a_new_sphere_of_annihilation(xl, yl, i, TRnd(20) + 11);	/* make a sphere */
			return;
		case 33:
			genmonst();
			spelknow[33] = 0;		/* genocide */
			loseint();
			return;
		case 34:			/* summon demon */
			if (TRnd(100) > 30) {
				direct(x, 150, "\n The demon strikes at the %s", 0);
				return;
			}
			if (TRnd(100) > 15) {
				fl_display_message(" Nothing seems to have happened");
				return;
			}
			fl_display_message(" The");
			fl_display_message(" demon ");
			fl_display_message("turned on you and vanished!");
			i = TRnd(40) + 30;
			lastnum = 277;
			CoreFuncs.DecreasePHealth(i);		/* must say killed by a demon */
			return;
		case 35:			/* walk through walls */
			cdesc[FL_WALK_THROUGH_WALLS] += TRnd(10) + 5;
			return;
		case 36: {		/* alter reality */
				struct isave *save;	/* pointer to object save structure */
				int sc;
				sc = 0;			/* # items saved */
				save =
				    (struct isave *) operator new (sizeof(struct isave) * FL_MAX_HORIZONTAL_POSITION * FL_MAX_VERTICAL_POSITION
				                                   * 2);
				if (save == NULL) {
					fl_display_message("\n Polinneaus won't let you mess with his dungeon!");
					return;
				}
				for (j = 0; j < FL_MAX_VERTICAL_POSITION; j++)
					for (i = 0; i < FL_MAX_HORIZONTAL_POSITION;
					        i++) {	/* save all items and monsters */
						xl = object_identification[i][j];
						if (xl && xl != OWALL && xl != FL_OBJECT_SPHERE_OF_ANNIHILATION) {
							save[sc].type = 0;
							save[sc].id = object_identification[i][j];
							save[sc++].arg = object_argument[i][j];
						}
						if (monster_identification[i][j]) {
							save[sc].type = 1;
							save[sc].id = monster_identification[i][j];
							save[sc++].arg = monster_hit_points[i][j];
						}
						object_identification[i][j] = OWALL;
						monster_identification[i][j] = 0;
						if (wizard) {
							been_here_before[i][j] = KNOWALL;
						} else {
							been_here_before[i][j] = 0;
						}
					}
				fl_carve_out_the_dungeon(1, 1);
				if (level == 1) {
					object_identification[33][FL_MAX_VERTICAL_POSITION - 1] = OENTRANCE;
				}
				for (j = TRnd(FL_MAX_VERTICAL_POSITION - 2), i = 1; i < FL_MAX_HORIZONTAL_POSITION - 1; i++) {
					object_identification[i][j] = 0;
				}
				while (sc > 0) {	/* put objects back in level */
					--sc;
					if (save[sc].type == 0) {
						int trys;
						for (trys = 100, i = j = 1; --trys > 0 && object_identification[i][j];
						        i = TRnd(FL_MAX_HORIZONTAL_POSITION - 1), j = TRnd(FL_MAX_VERTICAL_POSITION - 1));
						if (trys) {
							object_identification[i][j] = save[sc].id;
							object_argument[i][j] = save[sc].arg;
						}
					} else {
						/* put monsters back in */
						int trys;
						for (trys = 100, i = j = 1;
						        --trys > 0 && (object_identification[i][j] == OWALL || monster_identification[i][j]);
						        i = TRnd(FL_MAX_HORIZONTAL_POSITION - 1), j = TRnd(FL_MAX_VERTICAL_POSITION - 1));
						if (trys) {
							monster_identification[i][j] = save[sc].id;
							monster_hit_points[i][j] = save[sc].arg;
						}
					}
				}
				loseint();
				draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);
				if (wizard == 0) {
					spelknow[36] = 0;
				}
				free((char *) save);
				fl_place_the_player();
				return;
			}
		case 37:			/* permanence */
			fl_adjust_time(-99999L);
			spelknow[37] = 0;		/* forget */
			loseint();
			return;
		default:
			lprintf("spell %d not available!", (int) x);
			return;
	};
}



/*
    Create a guardian for a throne/altar/fountain, as a result of the player
    using a VPR spell or pulverization scroll on it.

    monst = monster code for guardian
    x, y = coords of the object being guarded

*/
static void
create_guardian(int monst, int x, int y) {
	int k;
	/* prevent the guardian from being created on top of the player */
	if (x == player_horizontal_position && y == player_vertical_position) {
		k = TRnd(8);
		x += diroffx[k];
		y += diroffy[k];
	}
	been_here_before[x][y] = 0;
	monster_identification[x][y] = monst;
	monster_hit_points[x][y] = monster[monst].hitpoints;
}



/*
    loseint()       Routine to subtract 1 from your int (intelligence) if > 3

    No arguments and no return value
*/
static void
loseint(void) {
	if (--cdesc[FL_INTELLIGENCE] < 3) {
		cdesc[FL_INTELLIGENCE] = 3;
	}
}



/*
    isconfuse()         Routine to check to see if player is confused

    This routine prints out a message saying "You can't aim your magic!"
    returns 0 if not confused, non-zero (time remaining confused) if confused
*/
static int
isconfuse(void) {
	if (cdesc[FL_CONFUSE]) {
		fl_display_message(" You can't aim your magic!");
	}
	return (cdesc[FL_CONFUSE]);
}



/*
    nospell(x,monst)    Routine to return 1 if a spell doesn't affect a monster
       int x,monst;

    Subroutine to return 1 if the spell can't affect the monster
     otherwise returns 0
    Enter with the spell number in x, and the monster number in monst.
*/
static int
nospell(int x, int monst) {
	int tmp;
	if (x >= FL_MAX_SPELLS_IN_GAME || monst >= FL_MAX_MONSTERS_IN_GAME + 8 || monst < 0
	        || x < 0) {
		return (0);    /* bad spell or monst */
	}
	if ((tmp = spelweird[monst - 1][x]) == 0) {
		return (0);
	}
	fl_termcap_cursor_position(1, 24);
	fl_output_single_byte_to_output_buffer('\n');
	lprintf(spelmes[tmp], monster[monst].name);
	return (1);
}




/*
    fullhit(xx)     Function to return full damage against a monster (aka web)
       int xx;

    Function to return hp damage to monster due to a number of full hits
    Enter with the number of full hits being done
*/
int
fl_is_fullhit(int xx) {
	int i;
	if (xx < 0 || xx > 20) {
		return (0);    /* fullhits are out of range */
	}
	if (cdesc[FL_GREAT_SWORD_OF_DEATH]) {
		return (10000);    /* great sword of death */
	}
	i = xx * ((cdesc[FL_WCLASS] >> 1) + cdesc[FL_STRENGTH] +
	          cdesc[FL_EXTRA_STRENGTH] + cdesc[FL_MORE_DAMAGE]);
	return ((i >= 1) ? i : xx);
}




/*
    direct(spnum,dam,str,arg)   Routine to direct spell damage 1 square in 1 sphere_direction
       int spnum,dam,arg;
       char *str;

    Routine to ask for a direction to a spell and then hit the monster
    Enter with the spell number in spnum, the damage to be done in dam,
     lprintf format string in str, and lprintf's argument in arg.
    Returns no value.
*/
static void
direct(int spnum, int dam, const char* str, int arg) {
	int x, y;
	int m;
	HitMonster hitpoints;
	FLCoreFuncs CoreFuncs;
	/* bad arguments */
	if (spnum < 0 || spnum >= FL_MAX_SPELLS_IN_GAME || str == NULL) {
		return;
	}
	if (isconfuse()) {
		return;
	}
	fl_direction(&x, &y);
	m = monster_identification[x][y];
	if (object_identification[x][y] == OMIRROR) {
		if (spnum == 3) {	/* sleep */
			fl_display_message("You fall asleep! ");
		} else if (spnum == 6) {	/* web */
			fl_display_message("You get stuck in your own web! ");
			arg += 2;
			while (arg-- > 0) {
				parse2();
				fl_wait(FL_WAIT_DURATION);
			}
			return;
		} else {
			lastnum = 278;
			lprintf(str, "spell caster (thats you)", (int) arg);
			CoreFuncs.DecreasePHealth(dam);
			return;
		}
	}
	if (m == 0) {
		fl_display_message("  There wasn't anything there!");
		return;
	}
	fl_player_is_blind(x, y);
	if (nospell(spnum, m)) {
		lasthx = x;
		lasthy = y;
		return;
	}
	lprintf(str, lastmonst, (int) arg);
	hitpoints.hitm(x, y, dam);
}



/*
    godirect(spnum,dam,str,delay,cshow)     Function to perform missile attacks
       int spnum,dam,delay;
       char *str,cshow;

    Function to hit in a direction from a missile weapon and have it keep
    on going in that direction until its power is exhausted
    Enter with the spell number in spnum, the power of the weapon in hp,
     lprintf format string in str, the # of milliseconds to delay between
     locations in delay, and the character to represent the weapon in cshow.
    Returns no value.
*/
void
fl_go_direction(int spnum, int dam, const char *str, int delay,
                const char cshow) {
	int *p;
	int x, y, m;
	int dx, dy;
	HitMonster hitpoints;
	FLCoreFuncs CoreFuncs;
	/* bad args */
	if (spnum < 0 || spnum >= FL_MAX_SPELLS_IN_GAME || str == 0 || delay < 0) {
		return;
	}
	if (isconfuse()) {
		return;
	}
	fl_direction(&dx, &dy);
	x = dx;
	y = dy;
	dx = x - player_horizontal_position;
	dy = y - player_vertical_position;
	x = player_horizontal_position;
	y = player_vertical_position;
	while (dam > 0) {
		x += dx;
		y += dy;
		if ((x > FL_MAX_HORIZONTAL_POSITION - 1) || (y > FL_MAX_VERTICAL_POSITION - 1) || (x < 0)
		        || (y < 0)) {
			dam = 0;
			/* out of bounds */
			break;
		}
		/* if energy hits player */
		if ((x == player_horizontal_position) && (y == player_vertical_position)) {
			fl_termcap_cursor_position(1, 24);
			fl_display_message("\nYou are hit by your own magic!");
			lastnum = 278;
			CoreFuncs.DecreasePHealth(dam);
			return;
		}
		/* if not blind show effect */
		if (cdesc[FL_BLINDCOUNT] == 0) {
			fl_termcap_cursor_position(x + 1, y + 1);
			fl_output_single_byte_to_output_buffer(cshow);
			fl_wait(delay);
			fl_show_designated_cell_only(x, y);
		}
		m = monster_identification[x][y];
		/* is there a monster there? */
		if (m != 0) {
			fl_player_is_blind(x, y);
			if (nospell(spnum, m)) {
				lasthx = x;
				lasthy = y;
				return;
			}
			fl_termcap_cursor_position(1, 24);
			fl_output_single_byte_to_output_buffer('\n');
			lprintf(str, lastmonst);
			hitpoints.hitm(x, y, dam);
			dam -= hitpoints.hitm(x, y, dam);
			fl_show_designated_cell_only(x, y);
			x -= dx;
			y -= dy;
		} else
			switch (* (p = &object_identification[x][y])) {
				case OWALL:
					fl_termcap_cursor_position(1, 24);
					fl_output_single_byte_to_output_buffer('\n');
					lprintf(str, "wall");
					if (
					    /* enough damage? */
					    dam >= 50 &&
					    /* not on V3 */
					    level < FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL - 1 &&
					    x < FL_MAX_HORIZONTAL_POSITION - 1 && y < FL_MAX_VERTICAL_POSITION - 1 && x != 0 && y != 0) {
						fl_display_message("  The wall crumbles");
						*p = 0;
						been_here_before[x][y] = 0;
						fl_show_designated_cell_only(x, y);
					}
					dam = 0;
					break;
				case OCLOSEDDOOR:
					fl_termcap_cursor_position(1, 24);
					fl_output_single_byte_to_output_buffer('\n');
					lprintf(str, "door");
					if (dam >= 40) {
						fl_display_message("  The door is blasted apart");
						*p = 0;
						been_here_before[x][y] = 0;
						fl_show_designated_cell_only(x, y);
					}
					dam = 0;
					break;
				case OSTATUE:
					fl_termcap_cursor_position(1, 24);
					fl_output_single_byte_to_output_buffer('\n');
					lprintf(str, "statue");
					if (dam > 44) {
						fl_display_message("  The statue crumbles");
						*p = OPRAYERBOOK;
						object_argument[x][y] = level;
						been_here_before[x][y] = 0;
						fl_show_designated_cell_only(x, y);
					}
					dam = 0;
					break;
				case OTHRONE:
					fl_termcap_cursor_position(1, 24);
					fl_output_single_byte_to_output_buffer('\n');
					lprintf(str, "throne");
					if (dam > 39) {
						*p = OTHRONE2;
						create_guardian(GNOMEKING, x, y);
						fl_show_designated_cell_only(x, y);
					}
					dam = 0;
					break;
				case OALTAR:
					fl_termcap_cursor_position(1, 24);
					fl_output_single_byte_to_output_buffer('\n');
					lprintf(str, "altar");
					if (dam > 75) {
						create_guardian(DEMONPRINCE, x, y);
						fl_show_designated_cell_only(x, y);
					}
					dam = 0;
					break;
				case OFOUNTAIN:
					fl_termcap_cursor_position(1, 24);
					fl_output_single_byte_to_output_buffer('\n');
					lprintf(str, "fountain");
					if (dam > 55) {
						create_guardian(WATERLORD, x, y);
						fl_show_designated_cell_only(x, y);
					}
					dam = 0;
					break;
				case OMIRROR: {
						int bounce = 0, odx = dx, ody = dy;
						/* spells may bounce directly back or off at an angle */
						if (TRnd(100) < 50) {
							bounce = 1;
							dx *= -1;
						}
						if (TRnd(100) < 50) {
							bounce = 1;
							dy *= -1;
						}
						/* guarentee a bounce */
						if (!bounce || (odx == dx && ody == dy)) {
							dx = -odx;
							dy = -ody;
						}
					}
					break;
			};
		dam -= 3;
	}
}



/*
    fl_player_is_blind(x,y)    Routine to put "monster" or the monster name into lastmosnt
       int x,y;

    Subroutine to copy the word "monster" into lastmonst if the player is blind
    Enter with the coordinates (x,y) of the monster
    Returns no value.
*/
void
fl_player_is_blind(int x, int y) {
	const char *p;
	/* verify correct x, y coordinates */
	fl_verify_bound_coordinates(&x, &y);
	if (cdesc[FL_BLINDCOUNT]) {
		lastnum = 279;
		p = "monster";
	} else {
		lastnum = monster_identification[x][y];
		p = monster[lastnum].name;
	}
	strncpy(lastmonst, p, 50);
}



/*
    tdirect(spnum)      Routine to teleport away a monster
       int spnum;

    Routine to ask for a direction to a spell and then teleport away monster
    Enter with the spell number that wants to teleport away
    Returns no value.
*/
static void
tdirect(int spnum) {
	int x, y, m;
	/* bad args */
	if (spnum < 0 || spnum >= FL_MAX_SPELLS_IN_GAME) {
		return;
	}
	if (isconfuse()) {
		return;
	}
	fl_direction(&x, &y);
	m = monster_identification[x][y];
	if (m == 0) {
		fl_display_message("  There wasn't anything there!");
		return;
	}
	fl_player_is_blind(x, y);
	if (nospell(spnum, m)) {
		lasthx = x;
		lasthy = y;
		return;
	}
	fl_fill_dungeon_with_monsters(m);
	monster_identification[x][y] = 0;
	been_here_before[x][y] &= ~KNOWHERE;
}



/*
    omnidirect(sp,dam,str)   Routine to damage all monsters 1 square from player
       int sp,dam;
       char *str;

    Routine to cast a spell and then hit the monster in all directions
    Enter with the spell number in sp, the damage done to wach square in dam,
     and the lprintf string to identify the spell in str.
    Returns no value.
*/
static void
omnidirect(int spnum, int dam, const char *str) {
	int x, y, m;
	HitMonster hitpoints;
	/* bad args */
	if (spnum < 0 || spnum >= FL_MAX_SPELLS_IN_GAME || str == 0) {
		return;
	}
	for (x = player_horizontal_position - 1; x < player_horizontal_position + 2; x++) {
		for (y = player_vertical_position - 1; y < player_vertical_position + 2; y++) {
			m = monster_identification[x][y];
			if (m == 0) {
				continue;
			}
			if (nospell(spnum, m) == 0) {
				fl_player_is_blind(x, y);
				fl_termcap_cursor_position(1, 24);
				fl_output_single_byte_to_output_buffer('\n');
				lprintf(str, lastmonst);
				hitpoints.hitm(x, y, dam);
				fl_wait(FL_WAIT_DURATION);
			} else {
				lasthx = x;
				lasthy = y;
			}
		}
	}
}

/*
    fl_direction(x,y)      Routine to ask for direction, then modify player_horizontal_position,
                    player_vertical_position for it
       int *x,*y;

    Function to ask for a direction and modify an x,y for that direction
    Enter with the coordinate destination (x,y).
    Returns index into diroffx[] (0-8).
*/
int
fl_direction(int *x, int *y) {
	int i;
	fl_display_message("\nIn What Direction? ");
	for (i = 0;;) {
		switch (ttgetch()) {
			case 'b':
				i++;
				[[fallthrough]];
			case 'n':
				i++;
				[[fallthrough]];
			case 'y':
				i++;
				[[fallthrough]];
			case 'u':
				i++;
				[[fallthrough]];
			case 'h':
				i++;
				[[fallthrough]];
			case 'k':
				i++;
				[[fallthrough]];
			case 'l':
				i++;
				[[fallthrough]];
			case 'j':
				i++;
				/* Added an ESC. -Gibbon */
				[[fallthrough]];
			case '\33':
				drawscreen();
				*x = player_horizontal_position + diroffx[i];
				*y = player_vertical_position + diroffy[i];
				fl_verify_bound_coordinates(x, y);
				return i;
		};
	}
}

/*
    dirpoly(spnum)      Routine to ask for a direction and polymorph a monst
       int spnum;

    Subroutine to polymorph a monster and ask for the direction its in
    Enter with the spell number in spmun.
    Returns no value.
*/
static void
dirpoly(int spnum) {
	int x, y, m;
	/* bad args */
	if (spnum < 0 || spnum >= FL_MAX_SPELLS_IN_GAME) {
		return;
	}
	/* if he is confused, he can't aim his magic */
	if (isconfuse()) {
		return;
	}
	fl_direction(&x, &y);
	if (monster_identification[x][y] == 0) {
		fl_display_message("  There wasn't anything there!");
		return;
	}
	fl_player_is_blind(x, y);
	if (nospell(spnum, monster_identification[x][y])) {
		lasthx = x;
		lasthy = y;
		return;
	}
	do {
		m = TRnd(FL_MAX_MONSTERS_IN_GAME + 7);
		monster_identification[x][y] = m;
	} while (monster[m].genocided);
	monster_hit_points[x][y] = monster[m].hitpoints;
	/* show the new monster */
	fl_show_designated_cell_only(x, y);
}



/*
    annihilate()    Routine to annihilate all monsters around player (player_horizontal_position,player_vertical_position)

    Gives player experience, but no dropped objects

*/
void
fl_do_annihilation(void) {
	FLCoreFuncs CoreFuncs;
	int i, j;
	int k;
	int *p;
	for (k = 0, i = player_horizontal_position - 1; i <= player_horizontal_position + 1; i++) {
		for (j = player_vertical_position - 1; j <= player_vertical_position + 1; j++) {
			/* out of bounds */
			if (fl_verify_bound_coordinates(&i, &j)) {
				continue;
			}
			p = &monster_identification[i][j];
			/* no monster here */
			if (*p == 0) {
				continue;
			}
			if (*p < DEMONLORD + 2) {
				k += monster[*p].experience;
				*p = been_here_before[i][j] &= ~KNOWHERE;
			} else {
				lprintf("\nThe %s barely escapes being annihilated!",
				        monster[*p].name);
				/* lose half hit points */
				monster_hit_points[i][j] = (monster_hit_points[i][j] >> 1) + 1;
			}
		}
	}
	if (k > 0) {
		fl_display_message("\nYou hear loud screams of agony!");
		CoreFuncs.IncreaseExperience(k);
	}
}



/*
    genmonst()      Function to ask for monster and genocide from game

    This is done by setting a flag in the monster[] structure
*/
static void
genmonst(void) {
	int i, j;
	fl_termcap_cursor_position(1, 24);
	fl_display_message("\nGenocide what monster? ");
	for (i = 0; !isalpha(i) && i != ' '; i = ttgetch()) {
		;
	}
	lprint(i);
	/* search for the monster type */
	for (j = 0; j < FL_MAX_MONSTERS_IN_GAME; j++) {
		/* have we found it? */
		if (monstnamelist[j] == i) {
			/* genocided from game */
			monster[j].genocided = 1;
			lprintf("  There will be no more %s's", monster[j].name);
			/* now wipe out monsters on this level */
			fl_generate_a_new_dungeon_level(level);
			draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);
			bot_linex();
			return;
		}
	}
	fl_display_message("  You sense failure!");
}
