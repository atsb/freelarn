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
#include "../dungeon/FLDungeon.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../core/FLFuncs.hpp"
#include "../core/FLTermCapIO.hpp"
#include "FLBotSubstitution.hpp"

using std::cout;

#define nlprc(_ch) fl_output_single_byte_to_output_buffer(_ch)

static void bot_hpx(void);
static void bot_spellx(void);
static void botside(void);
static void seepage(void);
static int minx, maxx, miny, maxy;
static int bot1f = 0, bot2f = 0, bot3f = 0;
static int always = 0;
int regen_bottom = 0;
extern char logname[];

/*
    bottomline()
    now for the bottom line of the display
*/
void
bottomline(void) {
	fl_recalculate_armor_class();
	bot1f = 1;
}

void
bottomhp(void) {
	bot2f = 1;
}

void
bottomspell(void) {
	bot3f = 1;
}

void
bottomdo(void) {
	if (bot1f) {
		bot3f = bot1f = bot2f = 0;
		bot_linex();
		return;
	}
	if (bot2f) {
		bot2f = 0;
		bot_hpx();
	}
	if (bot3f) {
		bot3f = 0;
		bot_spellx();
	}
}

void
bot_linex(void) {
	FLDisplay Display;
	int i;
	if (regen_bottom || (always)) {
		regen_bottom = 1;
		fl_termcap_cursor_position(1, 18);
		lprintf("%s", logname);
		if (cdesc[FL_SPELLMAX] > 99) {
			fl_termcap_cursor_position(69, 10);
			lprintf("SPL:");
			fl_termcap_cursor_position(73, 10);
			lprintf("%3d/%3d", cdesc[FL_SPELLS],
			        cdesc[FL_SPELLMAX]);
		} else {
			fl_termcap_cursor_position(69, 10);
			lprintf("SPL:");
			fl_termcap_cursor_position(73, 10);
			lprintf("%3d/%2d", cdesc[FL_SPELLS],
			        cdesc[FL_SPELLMAX]);
		}
		fl_termcap_cursor_position(1, 19);
		lprintf("LVL:");
		if (cdesc[FL_LEVEL] > 99) {
			fl_termcap_cursor_position(5, 19);
			lprintf("%3d", cdesc[FL_LEVEL]);
		} else {
			fl_termcap_cursor_position(5, 19);
			lprintf("%-2d", cdesc[FL_LEVEL]);
		}
		fl_termcap_cursor_position(69, 1);
		lprintf("HP:");
		fl_termcap_cursor_position(72, 1);
		lprintf("%3d/%3d", cdesc[FL_HP], cdesc[FL_HPMAX]);
		fl_termcap_cursor_position(69, 2);
		lprintf("STR:");
		fl_termcap_cursor_position(73, 2);
		lprintf("%-2d", cdesc[FL_STRENGTH] + cdesc[FL_EXTRA_STRENGTH]);
		fl_termcap_cursor_position(69, 3);
		lprintf("INT:");
		fl_termcap_cursor_position(73, 3);
		lprintf("%-2d", cdesc[FL_INTELLIGENCE]);
		fl_termcap_cursor_position(69, 4);
		lprintf("WIS:");
		fl_termcap_cursor_position(73, 4);
		lprintf("%-2d", cdesc[FL_WISDOM]);
		fl_termcap_cursor_position(69, 5);
		lprintf("CON:");
		fl_termcap_cursor_position(73, 5);
		lprintf("%-2d", cdesc[FL_CONSTITUTION]);
		fl_termcap_cursor_position(69, 6);
		lprintf("DEX:");
		fl_termcap_cursor_position(73, 6);
		lprintf("%-2d", cdesc[FL_DEXTERITY]);
		fl_termcap_cursor_position(69, 7);
		lprintf("CHA:");
		fl_termcap_cursor_position(73, 7);
		lprintf("%-2d", cdesc[FL_CHARISMA]);
		fl_termcap_cursor_position(69, 8);
		lprintf("ACC:");
		fl_termcap_cursor_position(73, 8);
		lprintf("%-3d", cdesc[FL_AC]);
		fl_termcap_cursor_position(69, 9);
		lprintf("WCL:");
		fl_termcap_cursor_position(73, 9);
		lprintf("%-3d", cdesc[FL_WCLASS]);
		fl_termcap_cursor_position(69, 11);
		lprintf("HGR:");
		fl_termcap_cursor_position(73, 11);
		lprintf("%3d/%3d", cdesc[FL_HUNGER], cdesc[FL_HUNGERMAX]);
		fl_termcap_cursor_position(24, 18);
		lprintf("Cave Level:");
		if ((level == 0) || (wizard)) {
			cdesc[FL_TELEPORT_FLAG] = 0;
		}
		if (cdesc[FL_TELEPORT_FLAG]) {
			lprintf(" ?");
		} else {
			lprintf(levelname[level]);
		}
		fl_termcap_cursor_position(24, 19);
		lprintf("EXP:");
		fl_termcap_cursor_position(28, 19);
		lprintf("%-9d", cdesc[FL_EXPERIENCE]);
		fl_termcap_cursor_position(52, 18);
		lprintf("%s", classname[cdesc[FL_LEVEL] - 1]);
		always = 1;
		botside();
		cdesc[FL_SCRATCH_SPACE] = cdesc[FL_STRENGTH] + cdesc[FL_EXTRA_STRENGTH];
		for (i = 0; i < 100; i++) {
			cbak[i] = cdesc[i];
		}
		return;
	}
	Display.BotSubstitution(FL_SPELLS, 73, 10, "%3d");
	if (cdesc[FL_SPELLMAX] > 99) {
		Display.BotSubstitution(FL_SPELLMAX, 73, 10, "%3d)");
	} else {
		Display.BotSubstitution(FL_SPELLMAX, 73, 10, "%2d) ");
	}
	Display.BotSubstitution(FL_HP, 69, 1, "%3d");
	Display.BotSubstitution(FL_HPMAX, 69, 1, "%3d");
	Display.BotSubstitution(FL_HUNGER, 69, 11, "%3d");
	Display.BotSubstitution(FL_HUNGERMAX, 73, 11, "%3d");
	Display.BotSubstitution(FL_AC, 73, 8, "%-3d");
	Display.BotSubstitution(FL_WCLASS, 73, 9, "%-3d");
	Display.BotSubstitution(FL_EXPERIENCE, 28, 19, "%-9d");
	if (cdesc[FL_LEVEL] != cbak[FL_LEVEL]) {
		fl_termcap_cursor_position(50, 18);
		lprintf(classname[cdesc[FL_LEVEL] - 1]);
	}
	if (cdesc[FL_LEVEL] > 99) {
		Display.BotSubstitution(FL_LEVEL, 5, 19, "%3d");
	} else {
		Display.BotSubstitution(FL_LEVEL, 5, 19, " %-2d");
	}
	cdesc[FL_SCRATCH_SPACE] = cdesc[FL_STRENGTH] + cdesc[FL_EXTRA_STRENGTH];
	Display.BotSubstitution(FL_SCRATCH_SPACE, 73, 2, "%-2d");
	Display.BotSubstitution(FL_INTELLIGENCE, 73, 3, "%-2d");
	Display.BotSubstitution(FL_WISDOM, 73, 4, "%-2d");
	Display.BotSubstitution(FL_CONSTITUTION, 73, 5, "%-2d");
	Display.BotSubstitution(FL_DEXTERITY, 73, 6, "%-2d");
	Display.BotSubstitution(FL_CHARISMA, 73, 7, "%-2d");
	if ((level != cbak[FL_CAVE_LEVEL])
	        || (cdesc[FL_TELEPORT_FLAG] != cbak[FL_TELEPORT_FLAG])) {
		if ((level == 0) || (wizard)) {
			cdesc[FL_TELEPORT_FLAG] = 0;
		}
		cbak[FL_TELEPORT_FLAG] = cdesc[FL_TELEPORT_FLAG];
		cbak[FL_CAVE_LEVEL] = level;
		fl_termcap_cursor_position(70, 18);
		if (cdesc[FL_TELEPORT_FLAG]) {
			lprintf(" ?");
		} else {
			lprintf(levelname[level]);
		}
	}
	botside();
}

/*
    special routine to update hp and level fields
*/
static void
bot_hpx(void) {
	FLDisplay Display;
	fl_termcap_cursor_position(69, 1);
	lprintf("HP:");
	fl_termcap_cursor_position(72, 1);
	lprintf("%3d", cdesc[FL_HP]);
	if (cdesc[FL_EXPERIENCE] != cbak[FL_EXPERIENCE]) {
		fl_recalculate_armor_class();
		bot_linex();
	} else {
		Display.BotSubstitution(FL_HP, 72, 1, "%3d");
	}
}

/*
    special routine to update number of spells called from fl_regen_hp_and_spells()
*/
static void
bot_spellx(void) {
	FLDisplay Display;
	Display.BotSubstitution(FL_SPELLS, 73, 10, "%2d");
}

/*
    common subroutine for a more economical bottomline()
*/
struct bot_side_def {

	int typ;
	const char *spell_text;

};

static struct bot_side_def bot_data[] = {
	{FL_STEALTH, "stealth"},
	{FL_UNDEADPRO, "undead pro"},
	{FL_SPIRITPRO, "spirit pro"},
	{FL_CHARMCOUNT, "Charm"},
	{FL_TIMESTOP, "Time Stop"},
	{FL_HOLDMONST, "Hold Monst"},
	{FL_GIANTSTR, "Giant Str"},
	{FL_FIRERESISTANCE, "Fire Resit"},
	{FL_DEXCOUNT, "Dexterity"},
	{FL_STRCOUNT, "Strength"},
	{FL_SCAREMONST, "Scare"},
	{FL_HASTESELF, "Haste Self"},
	{FL_CANCELLATION, "Cancel"},
	{FL_INVISIBILITY, "Invisible"},
	{FL_ALTPRO, "Protect 3"},
	{FL_PROTECTIONTIME, "Protect 2"},
	{FL_WALK_THROUGH_WALLS, "Wall-Walk"}
};

static void
botside(void) {
	int i, idx;
	for (i = 0; i < 17; i++) {
		idx = bot_data[i].typ;
		if ((always) || (cdesc[idx] != cbak[idx])) {
			if ((always) || (cbak[idx] == 0)) {
				if (cdesc[idx]) {
					fl_termcap_cursor_position(53, 19);
					lprintf(bot_data[i].spell_text);
					/*Reset cursor position. ~Gibbon */
					fl_termcap_cursor_position(1, 24);
				}
			} else if (cdesc[idx] == 0) {
				fl_termcap_cursor_position(53, 19);
				lprintf("          ");
				/*Reset cursor position. ~Gibbon */
				fl_termcap_cursor_position(1, 24);
			}
			cbak[idx] = cdesc[idx];
		}
	}
	always = 0;
}

/*
    subroutine to draw only a section of the screen
    only the top section of the screen is updated.  If entire lines are being
    drawn, then they will be cleared first.
*/
static int d_xmin = 0, d_xmax = FL_MAX_HORIZONTAL_POSITION, d_ymin = 0,
           d_ymax = FL_MAX_VERTICAL_POSITION;	/* for limited screen drawing */
void
draws(int xmin, int xmax, int ymin, int ymax) {
	int i, idx;
	if (xmin == 0
	        && xmax == FL_MAX_HORIZONTAL_POSITION) {	/* clear section of screen as needed */
		if (ymin == 0) {
			fl_clear_screen_from_to(79, ymax);
		} else
			for (i = ymin; i < ymin; i++) {
				fl_clear_line(1, i + 1);
			}
		xmin = -1;
	}
	d_xmin = xmin;
	d_xmax = xmax;
	d_ymin = ymin;
	d_ymax = ymax;		/* for limited screen drawing */
	drawscreen();
	if (xmin <= 0
	        && xmax ==
	        FL_MAX_HORIZONTAL_POSITION) {	/* draw stuff on right side of screen as needed */
		for (i = ymin; i < ymax; i++) {
			idx = bot_data[i].typ;
			if (cdesc[idx]) {
				fl_termcap_cursor_position(53, 19);
				lprintf(bot_data[i].spell_text);
			}
			cbak[idx] = cdesc[idx];
		}
	}
}

/*
    drawscreen()

    subroutine to redraw the whole screen as the player knows it
*/
static int d_flag;
void
drawscreen(void) {
	int i, j, k, ileft, iright;
	if (d_xmin == 0 && d_xmax == FL_MAX_HORIZONTAL_POSITION && d_ymin == 0
	        && d_ymax == FL_MAX_VERTICAL_POSITION) {
		/* clear the screen */
		d_flag = 1;
		fl_clear_and_reset_screen();
	} else {
		d_flag = 0;
		fl_termcap_cursor_position(1, 1);
	}
	if (d_xmin < 0) {
		/* d_xmin=-1 means display all without bottomline */
		d_xmin = 0;
	}
	/* display lines of the screen */
	for (j = d_ymin; j < d_ymax; j++) {
		/*  When we show a spot of the dungeon, we have 4 cases:
		    squares we been_here_before nothing about
		    - been_here_before == 0
		    squares we've been at and still been_here_before whats there
		    - been_here_before == KNOWALL (== KNOWHERE | HAVESEEN)
		    squares we've been at, but don't still recall because
		    something else happened there.
		    - been_here_before == HAVESEEN
		    squares we recall, but haven't been at (an error condition)
		    - been_here_before == KNOWHERE

		    to minimize printing of spaces, scan from left of line until
		    we reach a location that the user knows.
		*/
		ileft = d_xmin - 1;
		while (++ileft < d_xmax) {
			if (been_here_before[ileft][j]) {
				break;
			}
		}
		/* if blank line ... */
		if (ileft >= d_xmax) {
			continue;
		}
		/*  scan from right of line until we reach a location that the
		    user knows.
		*/
		iright = d_xmax;
		while (--iright > ileft) {
			if (been_here_before[iright][j]) {
				break;
			}
		}
		/*
		    now print the line, after positioning the cursor.
		    print the line with bold objects in a different
		    loop for effeciency
		*/
		fl_termcap_cursor_position(ileft + 1, j + 1);
		for (i = ileft; i <= iright; i++) {
			/*  we still need to check for the location being known,
			    for we might have an unknown spot in the middle of
			    an otherwise known line.
			*/
			if (been_here_before[i][j] == 0) {
				nlprc(' ');
			} else if (been_here_before[i][j] & HAVESEEN) {
				/*
				    if monster there and the user still knows the place,
				    then show the monster.  Otherwise, show what was
				    there before.
				*/
				if (i == player_horizontal_position && j == player_vertical_position) {
					nlprc('@');
					continue;
				}
				k = monster_identification[i][j];
				if (k && been_here_before[i][j] & KNOWHERE) {
					nlprc(monstnamelist[k]);
				} else {
					nlprc(objnamelist[object_identification[i][j]]);
				}
			} else {
				/*
				    error condition.  recover by resetting location
				    to an 'unknown' state.
				*/
				nlprc(' ');
				monster_identification[i][j] = object_identification[i][j] = 0;
			}
		}
	}
	fl_reset_text_bold();
	if (d_flag) {
		always = 1;
		botside();
		always = 1;
		bot_linex();
	}
	/* for limited screen drawing */
	d_xmin = d_ymin = 0;
	d_xmax = FL_MAX_HORIZONTAL_POSITION;
	d_ymax = FL_MAX_VERTICAL_POSITION;
}

/*
    showcell(x,y)

    subroutine to display a cell location on the screen
*/
void
showcell(int x, int y) {
	int i, j, k, m;
	if (cdesc[FL_BLINDCOUNT]) {
		return;  /* see nothing if blind     */
	}
	if (cdesc[FL_AWARENESS]) {
		minx = x - 3;
		maxx = x + 3;
		miny = y - 3;
		maxy = y + 3;
	} else {
		minx = x - 1;
		maxx = x + 1;
		miny = y - 1;
		maxy = y + 1;
	}
	if (minx < 0) {
		minx = 0;
	}
	if (maxx > FL_MAX_HORIZONTAL_POSITION - 1) {
		maxx = FL_MAX_HORIZONTAL_POSITION - 1;
	}
	if (miny < 0) {
		miny = 0;
	}
	if (maxy > FL_MAX_VERTICAL_POSITION - 1) {
		maxy = FL_MAX_VERTICAL_POSITION - 1;
	}
	for (j = miny; j <= maxy; j++)
		for (m = minx; m <= maxx; m++)
			if ((been_here_before[m][j] & KNOWHERE) == 0) {
				fl_termcap_cursor_position(m + 1, j + 1);
				x = maxx;
				while (been_here_before[x][j] & KNOWHERE) {
					--x;
				}
				for (i = m; i <= x; i++) {
					if ((k = monster_identification[i][j]) != 0) {
						fl_output_single_byte_to_output_buffer(monstnamelist[k]);
					} else
						switch (k = object_identification[i][j]) {
							case OWALL:
							case 0:
							case OIVTELETRAP:
							case OTRAPARROWIV:
							case OIVDARTRAP:
							case OIVTRAPDOOR:
								fl_output_single_byte_to_output_buffer(objnamelist[k]);
								break;
							default:
								fl_output_single_byte_to_output_buffer(objnamelist[k]);
								break;
						};
					been_here_before[i][j] = KNOWALL;
				}
				m = maxx;
			}
}

/*
    used in godirect() in monster.c for missile weapons display
*/
void
fl_show_designated_cell_only(int x, int y) {
	int k;
	fl_termcap_cursor_position(x + 1, y + 1);
	/* see nothing if blind, but clear previous player position */
	if (cdesc[FL_BLINDCOUNT]) {
		if (x == oldx && y == oldy) {
			fl_output_single_byte_to_output_buffer(' ');
		}
		return;
	}
	k = monster_identification[x][y];
	if (k) {
		fl_output_single_byte_to_output_buffer(monstnamelist[k]);
	} else {
		k = object_identification[x][y];
		switch (k) {
			case OWALL:
			case 0:
			case OIVTELETRAP:
			case OTRAPARROWIV:
			case OIVDARTRAP:
			case OIVTRAPDOOR:
				fl_output_single_byte_to_output_buffer(objnamelist[k]);
				break;
			default:
				fl_output_single_byte_to_output_buffer(objnamelist[k]);
		}
	}
	/* we end up knowing about it */
	been_here_before[x][y] = KNOWALL;
}

/*
    showplayer()

    subroutine to show where the player is on the screen
    cursor values start from 1 up
*/
void
showplayer(void) {
	fl_show_designated_cell_only(oldx, oldy);
	fl_termcap_cursor_position(player_horizontal_position + 1, player_vertical_position + 1);
	fl_output_single_byte_to_output_buffer('@');
	fl_termcap_cursor_position(player_horizontal_position + 1, player_vertical_position + 1);
	oldx = player_horizontal_position;
	oldy = player_vertical_position;
	refresh();
}

/*
    moveplayer(sphere_direction)

    subroutine to move the player from one room to another
    returns 0 if can't move in that direction or hit a monster or on an object
    else returns 1
    nomove is set to 1 to stop the next move (inadvertent monsters hitting
    players when walking into walls) if player walks off screen or into wall
*/
int diroffx[] = { 0, 0, 1, 0, -1, 1, -1, 1, -1 };
int diroffy[] = { 0, 1, 0, -1, 0, -1, -1, 1, 1 };
int
moveplayer(int sphere_direction)
/*  from = present room #  direction = [1-north]
    [2-east] [3-south] [4-west] [5-northeast]
    [6-northwest] [7-southeast] [8-southwest]
    if direction=0, don't move--just show where he is */
{
	int k, m, i, j;
	if (cdesc[FL_CONFUSE])
		if (cdesc[FL_LEVEL] < TRnd(30)) {
			sphere_direction = TRund(9);     /*if confused any sphere_direction */
		}
	k = player_horizontal_position + diroffx[sphere_direction];
	m = player_vertical_position + diroffy[sphere_direction];
	if (k < 0 || k >= FL_MAX_HORIZONTAL_POSITION || m < 0 || m >= FL_MAX_VERTICAL_POSITION) {
		nomove = 1;
		return (y_larn_rep = 0);
	}
	i = object_identification[k][m];
	j = monster_identification[k][m];
	/*  prevent the player from moving onto a wall, or a closed door,
	    unless the character has Walk-Through-Walls.
	*/
	if ((i == OCLOSEDDOOR || i == OWALL) && cdesc[FL_WALK_THROUGH_WALLS] == 0) {
		nomove = 1;
		return (y_larn_rep = 0);
	}
	if (k == 33 && m == FL_MAX_VERTICAL_POSITION - 1 && level == 1) {
		fl_generate_a_new_dungeon_level(0);
		for (k = 0; k < FL_MAX_HORIZONTAL_POSITION; k++)
			for (m = 0; m < FL_MAX_VERTICAL_POSITION; m++)
				if (object_identification[k][m] == OENTRANCE) {
					player_horizontal_position = k;
					player_vertical_position = m;
					fl_place_the_player();
					drawscreen();
					return (0);
				}
	}
	/*  hit a monster
	*/
	if (j > 0) {
		fl_hit_a_monster(k, m);
		return (y_larn_rep = 0);
	}
	/*  check for the player ignoring an altar
	*/
	if (object_identification[player_horizontal_position][player_vertical_position] == OALTAR && !prayed) {
		fl_termcap_cursor_position(1, 24);
		lprintf("\nYou have ignored the altar!");
		fl_a_ignore_an_altar();
	}
	prayed = 0;
	lastpx = player_horizontal_position;
	lastpy = player_vertical_position;
	player_horizontal_position = k;
	player_vertical_position = m;
	if (i && i != OTRAPARROWIV && i != OIVTELETRAP
	        && i != OIVDARTRAP
	        && i != OIVTRAPDOOR) {
		return (y_larn_rep = 0);
	} else {
		return (1);
	}
}

/*
    function to show what magic items have been discovered thus far
    enter with -1 for just spells, anything else will give scrolls & potions
*/
static int fllinecount, flcount;
void
seemagic(int arg) {
	int i, j, k, number;
	int sort[FL_MAX_SPELLS_IN_GAME + 1];		/* OK as long as FL_MAX_SPELLS_IN_GAME > MAXSCROLL,MAXPOTION */
	flcount = fllinecount = 0;
	/*  count and sort the known spell codes
	*/
	for (j = 0; j <= FL_MAX_SPELLS_IN_GAME; j++) {
		sort[j] = FL_MAX_SPELLS_IN_GAME;
	}
	for (number = i = 0; i < FL_MAX_SPELLS_IN_GAME; i++)
		if (spelknow[i]) {
			number++;
			j = 0;
			while (strncmp(spelcode[sort[j]], spelcode[i], 3) < 0) {
				j++;
			}
			k = number - 1;
			while (k > j) {
				sort[k] = sort[k - 1], k--;
			}
			sort[j] = i;
		}
	if (arg == -1) {	/* if display spells while casting one */
		fl_clear_screen_from_to(79, ((number + 2) / 3 + 4));	/* lines needed for display */
		fl_termcap_cursor_position(1, 1);
	} else {
		enable_scroll = 0;
		fl_clear_and_reset_screen();
	}
	lprintf("The magic spells you have discovered thus far:\n");
	for (i = 0; i < number; i++) {
		lprintf("\n%s %-20s ", spelcode[sort[i]], spelname[sort[i]]);
		seepage();
	}
	if (arg == -1) {
		seepage();
		fl_more_info(0);
		draws(0, FL_MAX_HORIZONTAL_POSITION, 0, ((number + 2) / 3 + 4));
		return;
	}
	fllinecount += 3;
	if (flcount != 0) {
		flcount = 2;
		seepage();
	}
	/*  count and sort the known scrolls
	*/
	for (j = 0; j <= MAXSCROLL; j++) {
		sort[j] = MAXSCROLL;
	}
	for (number = i = 0; i < MAXSCROLL; i++)
		if (scrollname[i][0]) {
			number++;
			j = 0;
			while (strcmp(&scrollname[sort[j]][1],
			              &scrollname[i][1]) < 0) {
				j++;
			}
			k = number - 1;
			while (k > j) {
				sort[k] = sort[k - 1], k--;
			}
			sort[j] = i;
		}
	lprintf("\nThe magic scrolls you have found to date are:\n");
	flcount = 0;
	for (i = 0; i < number; i++) {
		lprintf("\na magic scroll %-26s", &scrollname[sort[i]][1]);
		seepage();
	}
	fllinecount += 3;
	if (flcount != 0) {
		flcount = 2;
		seepage();
	}
	/*  count and sort the known potions
	*/
	for (j = 0; j <= MAXPOTION; j++) {
		sort[j] = MAXPOTION;
	}
	for (number = i = 0; i < MAXPOTION; i++)
		if (potionname[i][0]) {
			number++;
			j = 0;
			while (strcmp(&potionname[sort[j]][1],
			              &potionname[i][1]) < 0) {
				j++;
			}
			k = number - 1;
			while (k > j) {
				sort[k] = sort[k - 1], k--;
			}
			sort[j] = i;
		}
	lprintf("\nThe magic potions you have found to date are:\n");
	flcount = 0;
	for (i = 0; i < number; i++) {
		lprintf("\na magic potion %-26s", &potionname[sort[i]][1]);
		seepage();
	}
	if (fllinecount != 0) {
		fl_more_info(0);
	}
	enable_scroll = 1;
	drawscreen();
}

/*
    subroutine to paginate the seemagic function
*/
static void
seepage(void) {
	if (++flcount == 3) {
		fllinecount++;
		flcount = 0;
		if (fllinecount > 5) {
			fllinecount = 0;
			fl_more_info(0);
			fl_clear_and_reset_screen();
		}
	}
}
