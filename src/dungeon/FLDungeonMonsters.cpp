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

#if defined WINDOWS
	#include <io.h>
	#include <conio.h>
#endif

#include "../terminal/FLTerminal.hpp"
#include "../terminal/FLGetChar.hpp"
#include "../config/FLIncludeData.hpp"
#include "../templates/FLTMath.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../core/FLInventory.hpp"
#include "../core/FLTermCapIO.hpp"
#include "FLDungeon.hpp"
#include "../player/FLPlayerActions.hpp"
#include "../core/FLSystemDependant.hpp"
#include "../core/FLFuncs.hpp"

static int cgood(int, int, int, int);
static void dropsomething(int);
static int spattack(int, int, int);

/*
    fl_create_a_monster(monstno)      Function to create a monster next to the player
       int monstno;

    Enter with the monster number (1 to FL_MAX_MONSTERS_IN_GAME+8)
    Returns no value.
*/
void
fl_create_a_monster(int mon) {
	int x, y, k, i;
	if (mon < 1 || mon > FL_MAX_MONSTERS_IN_GAME + 8) {
		/* check for monster number out of bounds */
		lprintf("\ncan't createmonst(%d)\n", mon);
		fl_wait(3000);
		return;
	}
	while (monster[mon].genocided && mon < FL_MAX_MONSTERS_IN_GAME) {
		mon++;  /* skip genocided */
	}
	for (k = TRnd(8), i = -8; i < 0; i++, k++) {
		/* choose TRnd direction, then try all */
		if (k > 8) {
			k = 1;  /* wraparound the diroff arrays */
		}
		x = player_horizontal_position + diroffx[k];
		y = player_vertical_position + diroffy[k];
		if (cgood(x, y, 0, 1)) {
			/* if we can create here */
			monster_identification[x][y] = mon;
			monster_hit_points[x][y] = monster[mon].hitpoints;
			stealth[x][y] = 0;
			been_here_before[x][y] &= ~KNOWHERE;
			switch (mon) {
				case ROTHE:
				case POLTERGEIST:
				case VAMPIRE:
					stealth[x][y] = 1;
			};
			return;
		}
	}
}



/*
    int cgood(x,y,itm,monst)      Function to check location for emptiness
       int x,y,itm,monst;

    Routine to return TRUE if a location does not have itm or monst there
    returns 0 (0) otherwise
    Enter with itm or monst TRUE or 0 if checking it
    Example:  if itm==TRUE check for no object at this location
             if monst==TRUE check for no monster at this location
    This routine will return 0 if at a wall,door or the dungeon exit
    on level 1
*/
static int
cgood(int x, int y, int itm, int monst) {
	/*
	    cannot create either monster or object if:
	    - out of bounds
	    - wall
	    - closed door
	    - dungeon entrance
	*/
	if (((y < 0) || (y > FL_MAX_VERTICAL_POSITION - 1) || (x < 0)
	        || (x > FL_MAX_HORIZONTAL_POSITION - 1)) ||
	        (object_identification[x][y] == OWALL) ||
	        (object_identification[x][y] == OCLOSEDDOOR) ||
	        ((level == 1) && (x == 33) && (y == FL_MAX_VERTICAL_POSITION - 1))) {
		return 0;
	}
	/* if checking for an object_identification, return False if one there already */
	if (itm && object_identification[x][y]) {
		return 0;
	}
	/*
	    if checking for a monster, return False if one there already _or_
	    there is a pit/trap there.
	*/
	if (monst) {
		if (monster_identification[x][y]) {
			return 0;
		}
		/*
		    note: not invisible traps, since monsters are
		    not affected by them.
		*/
		switch (object_identification[x][y]) {
			case OPIT:
			case FL_OBJECT_SPHERE_OF_ANNIHILATION:
			case OTELEPORTER:
			case OTRAPARROW:
			case ODARTRAP:
			case OTRAPDOOR:
				return 0;
		}
	}
	/* cgood! */
	return 1;
}

void
fl_create_an_item(int it, int arg) {
	int x, y, k, i;
	if (it >= MAXOBJ) {
		return;  /* no such object */
	}
	for (k = TRnd(8), i = -8; i < 0; i++, k++) {
		/* choose TRnd direction, then try all */
		if (k > 8) {
			k = 1;  /* wraparound the diroff arrays */
		}
		x = player_horizontal_position + diroffx[k];
		y = player_vertical_position + diroffy[k];
		if (cgood(x, y, 1, 0)) {
			/* if we can create here */
			object_identification[x][y] = it;
			been_here_before[x][y] = 0;
			object_argument[x][y] = arg;
			return;
		}
	}
}

int
fl_verify_bound_coordinates(int *x, int *y) {
	int flag = 0;
	if (*x < 0) {
		*x = 0;
		flag++;
	}
	if (*y < 0) {
		*y = 0;
		flag++;
	}
	if (*x >= FL_MAX_HORIZONTAL_POSITION) {
		*x = FL_MAX_HORIZONTAL_POSITION - 1;
		flag++;
	}
	if (*y >= FL_MAX_VERTICAL_POSITION) {
		*y = FL_MAX_VERTICAL_POSITION - 1;
		flag++;
	}
	return (flag);
}



/*
    fl_hit_a_monster(x,y)     Function to hit a monster at the designated coordinates
       int x,y;

    This routine is used for a bash & slash type attack on a monster
    Enter with the coordinates of the monster in (x,y).
    Returns no value.
*/
void
fl_hit_a_monster(int x, int y) {
	int tmp, monst, flag, damag = 0;
	HitMonster hitpoints;
	if (cdesc[FL_TIMESTOP]) {
		return;  /* not if time stopped */
	}
	fl_verify_bound_coordinates(&x, &y);
	if ((monst = monster_identification[x][y]) == 0) {
		return;
	}
	hit3flag = 1;
	fl_player_is_blind(x, y);
	tmp = monster[monst].armorclass + cdesc[FL_LEVEL] +
	      cdesc[FL_DEXTERITY] + cdesc[FL_WCLASS] / 4 - 12;
	fl_termcap_cursor_position(1, 24);
	if ((TRnd(20) < tmp)
	        || (TRnd(71) <
	            5)) {	/* need at least random chance to hit */
		fl_display_message("\nYou hit");
		flag = 1;
		damag = fl_is_fullhit(1);
		if (damag < 9999) {
			damag = TRnd(damag) + 1;
		}
	} else {
		fl_display_message("\nYou missed");
		flag = 0;
	}
	fl_display_message(" the ");
	fl_display_message(lastmonst);
	if (flag)			/* if the monster was hit */
		if ((monst == RUSTMONSTER) || (monst == DISENCHANTRESS)
		        || (monst == CUBE))
			if (cdesc[FL_WIELD] >= 0)
				if (ivenarg[cdesc[FL_WIELD]] > -10) {
					lprintf("\nYour weapon is dulled by the %s", lastmonst);
					--ivenarg[cdesc[FL_WIELD]];
					/* fix for dulled rings of strength, cleverness, dexterity bug. */
					switch (iven[cdesc[FL_WIELD]]) {
						case ODEXRING:
							cdesc[FL_DEXTERITY]--;
							break;
						case OSTRRING:
							cdesc[FL_EXTRA_STRENGTH]--;
							break;
						case OCLEVERRING:
							cdesc[FL_INTELLIGENCE]--;
							break;
					}
					/* */
				}
	if (flag) {
		hitpoints.hitm(x, y, damag);
	}
	if (monst == VAMPIRE)
		if (monster_hit_points[x][y] < 25) {
			/* vampire turns into bat */
			monster_identification[x][y] = BAT;
			been_here_before[x][y] = 0;
		}
}

int
HitMonster::hitm(int x, int y, int amt) {
	FLCoreFuncs CoreFuncs;
	int monst;
	fl_verify_bound_coordinates(&x, &y);			/* verify coordinates are within range */
	amt2 = amt;			/* save initial damage so we can return it */
	monst = monster_identification[x][y];
	if (cdesc[FL_HALFDAM]) {
		amt >>= 1;  /* if half damage curse adjust damage points */
	}
	if (amt <= 0) {
		amt2 = amt = 1;
	}
	lasthx = x;
	lasthy = y;
	stealth[x][y] =
	    1;		/* make sure hitting monst breaks stealth condition */
	cdesc[FL_HOLDMONST] =
	    0;		/* hit a monster breaks hold monster spell  */
	switch (monst) {
		/* if a dragon and orb(s) of dragon slaying   */
		case WHITEDRAGON:
		case REDDRAGON:
		case GREENDRAGON:
		case BRONZEDRAGON:
		case PLATINUMDRAGON:
		case SILVERDRAGON:
			amt *= 1 + (cdesc[FL_SLAYING] << 1);
			break;
	}
	/* invincible monster fix is here */
	if (monster_hit_points[x][y] > monster[monst].hitpoints) {
		monster_hit_points[x][y] = monster[monst].hitpoints;
	}
	if ((hpoints = monster_hit_points[x][y]) <= amt) {
		cdesc[FL_MONSTER_KILLED]++;
		lprintf("\nThe");
		lprintf(" %s ", lastmonst);
		lprintf("was killed\n");
		CoreFuncs.IncreaseExperience(monster[monst].experience);
		amt = monster[monst].gold;
		if (amt > 0) {
			fl_drop_rand_gold_around_player_location(TRnd(amt) + amt);
		}
		dropsomething(monst);
		disappear(x, y);
		bottomline();
		return 0;
	}
	monster_hit_points[x][y] = hpoints - amt;
	return 0;
}

void
fl_hit_the_player(int x, int y) {
	FLCoreFuncs CoreFuncs;
	int dam, tmp, mster, bias;
	fl_verify_bound_coordinates(&x, &y);
	lastnum = mster = monster_identification[x][y];
	/*  spirit naga's and poltergeist's do nothing if scarab of negate spirit   */
	if (cdesc[FL_NEGATE_SPIRIT] || cdesc[FL_SPIRITPRO])
		if ((mster == POLTERGEIST) || (mster == SPIRITNAGA)) {
			return;
		}
	/*  if undead and cube of undead control    */
	if (cdesc[FL_CUBE_OF_UNDEAD] || cdesc[FL_UNDEADPRO])
		if ((mster == VAMPIRE) || (mster == WRAITH)
		        || (mster == ZOMBIE)) {
			return;
		}
	if ((been_here_before[x][y] & KNOWHERE) == 0) {
		fl_show_designated_cell_only(x, y);
	}
	bias = + 1;
	hitflag = hit2flag = hit3flag = 1;
	y_larn_rep = 0;
	fl_termcap_cursor_position(1, 24);
	fl_player_is_blind(x, y);
	if (cdesc[FL_INVISIBILITY])
		if (TRnd(33) < 20) {
			lprintf("\nThe");
			lprintf(" %s ", lastmonst);
			lprintf("misses wildly\n");
			return;
		}
	if (cdesc[FL_CHARMCOUNT])
		if (TRnd(30) + 5 * monster[mster].level - cdesc[FL_CHARISMA] <
		        30) {
			lprintf("\nThe");
			lprintf(" %s ", lastmonst);
			lprintf("is awestruck at your magnificence!\n");
			return;
		}
	if (mster == BAT) {
		dam = 1;
	} else {
		dam = monster[mster].damage;
		dam += TRnd((int)((dam < 1) ? 1 : dam)) +
		       monster[mster].level;
	}
	tmp = 0;
	if (monster[mster].attack > 0)
		if (((dam + bias + 8) > cdesc[FL_AC])
		        || (TRnd((int)((cdesc[FL_AC] > 0) ? cdesc[FL_AC] : 1)) == 1)) {
			if (spattack(monster[mster].attack, x, y)) {
				#if defined WINDOWS
				while (kbhit()) {
					ansiterm_getch();
				}
				#endif
				#if defined NIX || NIX_LOCAL
				fflush(NULL);
				#endif
				return;
			}
			tmp = 1;
			bias -= 2;
			fl_termcap_cursor_position(1, 24);
		}
	if (((dam + bias) > cdesc[FL_AC])
	        || (TRnd((int)((cdesc[FL_AC] > 0) ? cdesc[FL_AC] : 1)) == 1)) {
		lprintf("\nThe");
		lprintf(" %s ", lastmonst);
		lprintf("hit you");
		tmp = 1;
		if ((dam -= cdesc[FL_AC]) < 0) {
			dam = 0;
		}
		if (dam > 0) {
			CoreFuncs.DecreasePHealth(dam);
			bottomhp();
			#if defined WINDOWS
			while (kbhit()) {
				ansiterm_getch();
			}
			#endif
			#if defined NIX || NIX_LOCAL
			fflush(NULL);
			#endif
		}
	}
	if (tmp == 0) {
		lprintf("\nThe");
		lprintf(" %s ", lastmonst);
		lprintf("missed");
	}
}

/*
    dropsomething(monst)    Function to create an object when a monster dies
       int monst;

    Function to create an object near the player when certain monsters are killed
    Enter with the monster number
    Returns nothing of value.
*/
static void
dropsomething(int monst) {
	switch (monst) {
		case ORC:
		case NYMPH:
		case ELF:
		case TROGLODYTE:
		case TROLL:
		case ROTHE:
		case VIOLETFUNGI:
		case PLATINUMDRAGON:
		case GNOMEKING:
		case REDDRAGON:
			fl_create_rand_object_around_player_location(level);
			return;
		case LEPRECHAUN:
			if (TRnd(101) >= 75) {
				fl_create_gem();
			}
			if (TRnd(5) == 1) {
				dropsomething(LEPRECHAUN);
			}
			return;
	}
}

void
fl_drop_rand_gold_around_player_location(int amount) {
	if (amount > 250) {
		fl_create_an_item(OMAXGOLD, amount / 100);
	} else {
		fl_create_an_item(OGOLDPILE, amount);
	}
}

void
fl_create_rand_object_around_player_location(int lv) {
	int j, i;
	if (lv < 0 || lv > FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL) {
		return;  /* correct level? */
	}
	if (TRnd(101) < 8) {
		fl_create_rand_object_around_player_location(lv);   /* possibly more than one object_identification */
	}
	j = fl_new_rand_object(lv, &i);
	fl_create_an_item(j, i);
}

static int no_object_tab[] = { 0, OSCROLL, OSCROLL, OSCROLL, OSCROLL, OPOTION,
                               OPOTION, OPOTION, OPOTION, OGOLDPILE, OGOLDPILE, OGOLDPILE, OGOLDPILE,
                               OBOOK, OBOOK, OBOOK, OBOOK, ODAGGER, ODAGGER, ODAGGER, OLEATHER, OLEATHER,
                               OLEATHER, OREGENRING, OPROTRING, OENERGYRING, ODEXRING, OSTRRING, OSPEAR,
                               OBELT, ORING, OSTUDLEATHER, OSHIELD, OCOOKIE, OCHAIN, OBATTLEAXE,
                               OSPLINT, O2SWORD, OHSWORD, OCLEVERRING, OPLATE, OLONGSWORD,
                               OPRAYERBOOK, OPRAYERBOOK, OPRAYERBOOK, OPRAYERBOOK
                             };


int
fl_new_rand_object(int lev, int *i) {
	int tmp = 33, j, hacktmp = 0;
	if (level < 0 || level > FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL) {
		return (0);    /* correct level? */
	}
	if (lev > 6) {
		tmp = 41;
	} else if (lev > 4) {
		tmp = 39;
	}
	j = no_object_tab[tmp = TRnd(tmp)];	/* the object type */
	switch (tmp) {
		case 1:
		case 2:
		case 3:
		case 4:			/* scroll */
			*i = scroll_probability[TRund(81)];
			break;
		case 5:
		case 6:
		case 7:
		case 8:			/* potion */
			*i = potion_probability[TRund(41)];
			break;
		case 9:
		case 10:
		case 11:
		case 12:			/* gold */
			*i = TRnd((lev + 1) * 10) + lev * 10 + 10;
			break;
		case 13:
		case 14:
		case 15:
		case 16:			/* book */
			*i = lev;
			break;
		case 17:
		case 18:
		case 19:
			/* Return + points on new daggers */
			hacktmp = (*i = ndgg[TRund(13)]);
			if (!hacktmp) {
				return (0);
			}
			break;
		case 20:
		case 21:
		case 22:
			/* Return the + points on created leather armor */
			hacktmp = (*i = nlpts[TRund(15)]);
			if (!hacktmp) {
				return (0);
			}
			break;
		case 23:
		case 32:
		case 38:			/* regenerate ring, shield, 2-hand sword */
			*i = TRund(lev / 3 + 1);
			break;
		case 24:
		case 26:			/* prot ring, dexterity ring */
			*i = TRnd(lev / 4 + 1);
			break;
		case 25:			/* energy ring */
			*i = TRund(lev / 4 + 1);
			break;
		case 27:
		case 39:			/* strength ring, cleverness ring */
			*i = TRnd(lev / 2 + 1);
			break;
		case 30:
		case 34:			/* ring mail, flail */
			*i = TRund(lev / 2 + 1);
			break;
		case 28:
		case 36:			/* spear, battleaxe */
			*i = TRund(lev / 3 + 1);
			if (*i == 0) {
				return (0);
			}
			break;
		case 29:
		case 31:
		case 37:			/* belt, studded leather, splint */
			*i = TRund(lev / 2 + 1);
			if (*i == 0) {
				return (0);
			}
			break;
		case 33:			/* fortune cookie */
			*i = 0;
			break;
		case 35:
			/* Return the + points on chain armor */
			*i = nch[TRund(10)];
			break;
		case 40:
			/* Return + points on plate armor */
			*i = nplt[TRund(12)];
			break;
		case 41:
			/* Return + points on new swords */
			*i = nsw[TRund(13)];
			break;
	}
	return (j);
}



/*
    spattack(atckno,xx,yy)  Function to process special attacks from monsters
       int atckno,xx,yy;

    Enter with the special attack number, and the coordinates (xx,yy)
       of the monster that is special attacking
    Returns 1 if must do a fl_show_designated_cell_only(xx,yy) upon return, 0 otherwise

    atckno   monster     effect
    ---------------------------------------------------
    0   none
    1   rust monster    eat armor
    2   hell hound      breathe light fire
    3   dragon          breathe fire
    4   giant centipede weakening sing
    5   white dragon    cold breath
    6   wraith          drain level
    7   waterlord       water gusher
    8   leprechaun      steal gold
    9   disenchantress  disenchant weapon or armor
    10  ice lizard      hits with barbed tail
    11  umber hulk      confusion
    12  spirit naga     cast spells taken from special attacks
    13  platinum dragon psionics
    14  nymph           steal objects
    15  bugbear         bite
    16  osequip         bite

    char rustarm[ARMORTYPES][2];
    special array for maximum rust damage to armor from rustmonster
    format is: { armor type , minimum attribute
*/

static int rustarm[ARMORTYPES][2] = {

	{OSTUDLEATHER, -2},
	{ORING, -4},
	{OCHAIN, -5},
	{OSPLINT, -6},
	{OPLATE, -8},
	{OPLATEARMOR, -9}

};



static char spsel[] = { 1, 2, 3, 5, 6, 8, 9, 11, 13, 14 };

static int
spattack(int x, int xx, int yy) {
	FLCoreFuncs CoreFuncs;
	int i, j = 0, k, m;
	const char *p = 0;
	if (cdesc[FL_CANCELLATION]) {
		return (0);
	}
	fl_verify_bound_coordinates(&xx, &yy);
	switch (x) {
		case 1:			/* rust your armor, j=1 when rusting has occurred */
			m = k = cdesc[FL_WEAR];
			i = cdesc[FL_SHIELD];
			if (i != -1) {
				if (--ivenarg[i] < -1) {
					ivenarg[i] = -1;
				} else {
					j = 1;
				}
			}
			if ((j == 0) && (k != -1)) {
				m = iven[k];
				for (i = 0; i < ARMORTYPES; i++)
					if (m == rustarm[i][0]) {	/* find his armor in table */
						if (--ivenarg[k] < rustarm[i][1]) {
							ivenarg[k] = rustarm[i][1];
						} else {
							j = 1;
						}
						break;
					}
			}
			if (j == 0)		/* if rusting did not occur */
				switch (m) {
					case OLEATHER:
						p = "\nThe %s hit you -- Your lucky you have leather on";
						break;
					case OSSPLATE:
						p =
						    "\nThe %s hit you -- Your fortunate to have stainless steel armor!";
						break;
				} else {
				p = "\nThe %s hit you -- your armor feels weaker";
			}
			break;
			[[fallthrough]];
		case 2:
			i = TRnd(15) + 8 - cdesc[FL_AC];
			[[fallthrough]];
		case 3:
			i = TRnd(20) + 25 - cdesc[FL_AC];
			p = "\nThe %s breathes fire at you!";
			if (cdesc[FL_FIRERESISTANCE]) {
				p = "\nThe %s's flame doesn't phase you!";
			}
			[[fallthrough]];
		case 4:
			if (cdesc[FL_STRENGTH] > 3) {
				p = "\nThe %s stung you!  You feel weaker";
				--cdesc[FL_STRENGTH];
			} else {
				p = "\nThe %s stung you!";
			}
			break;
		case 5:
			p = "\nThe %s blasts you with his cold breath";
			i = TRnd(15) + 18 - cdesc[FL_AC];
			if (p) {
				lprintf(p, lastmonst);
			}
			if (i > 0) {
				CoreFuncs.DecreasePHealth(i);
				bottomhp();
			}
			return (0);
		case 6:
			lprintf("\nThe ");
			lprintf("%s", lastmonst);
			lprintf(" drains you of your life energy!");
			CoreFuncs.DecreasePlayerLevel();
			return (0);
		case 7:
			p = "\nThe %s got you with a gusher!";
			i = TRnd(15) + 25 - cdesc[FL_AC];
			if (p) {
				lprintf(p, lastmonst);
			}
			if (i > 0) {
				CoreFuncs.DecreasePHealth(i);
				bottomhp();
			}
			return (0);
		case 8:
			if (cdesc[FL_NO_THEFT]) {
				return (0);    /* he has a device of no theft */
			}
			if (cdesc[FL_GOLD]) {
				p = "\nThe %s hit you -- Your purse feels lighter";
				if (cdesc[FL_GOLD] > 32767) {
					cdesc[FL_GOLD] >>= 1;
				} else {
					cdesc[FL_GOLD] -= TRnd((int)(1 + (cdesc[FL_GOLD] >> 1)));
				}
				if (cdesc[FL_GOLD] < 0) {
					cdesc[FL_GOLD] = 0;
				}
			} else {
				p = "\nThe %s couldn't find any gold to steal";
			}
			lprintf(p, lastmonst);
			disappear(xx, yy);
			return (1);
		case 9:
			for (j = 50;;) {	/* disenchant */
				i = TRund(26);
				m = iven[i];		/* randomly select object_identification */
				if (m > 0 && ivenarg[i] > 0 && m != OSCROLL
				        && m != OPOTION) {
					if ((ivenarg[i] -= 3) < 0) {
						ivenarg[i] = 0;
					}
					lprintf("\nThe ");
					lprintf("%s ", lastmonst);
					lprintf("hits you -- you feel a sense of loss");
					fl_display_inventory(i, 0);
					bottomline();
					return (0);
				}
				if (--j <= 0) {
					p = "\nThe %s nearly misses";
					break;
				}
				break;
			}
			break;
		case 10:
			p = "\nThe %s hit you with his barbed tail";
			i = TRnd(25) - cdesc[FL_AC];
			if (p) {
				lprintf(p, lastmonst);
			}
			if (i > 0) {
				CoreFuncs.DecreasePHealth(i);
				bottomhp();
			}
			return (0);
		case 11:
			p = "\nThe %s has confused you";
			cdesc[FL_CONFUSE] += 10 + TRnd(10);
			break;
		case 12:			/*  performs any number of other special attacks    */
			return (spattack(spsel[TRund(10)], xx, yy));
		case 13:
			p = "\nThe %s flattens you with his psionics!";
			i = TRnd(15) + 30 - cdesc[FL_AC];
			if (p) {
				lprintf(p, lastmonst);
			}
			if (i > 0) {
				CoreFuncs.DecreasePHealth(i);
				bottomhp();
			}
			return (0);
		case 14:
			if (cdesc[FL_NO_THEFT]) {
				return (0);    /* he has device of no theft */
			}
			if (fl_empty_handed() == 1) {
				p = "\nThe %s couldn't find anything to steal";
				break;
			}
			lprintf("\nThe");
			lprintf(" %s ", lastmonst);
			lprintf("picks your pocket and takes:");
			if (fl_stolen() == 0) {
				fl_display_message(" nothing");
			}
			disappear(xx, yy);
			bottomline();
			return (1);
			[[fallthrough]];
		case 15:
			i = TRnd(10) + 5 - cdesc[FL_AC];
			[[fallthrough]];
		case 16:
			i = TRnd(15) + 10 - cdesc[FL_AC];
			p = "\nThe %s bit you!";
			if (p) {
				lprintf(p, lastmonst);
			}
			if (i > 0) {
				CoreFuncs.DecreasePHealth(i);
				bottomhp();
			}
			return (0);
	};
	if (p) {
		lprintf(p, lastmonst);
		bottomline();
	}
	return (0);
}