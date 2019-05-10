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

#include <iostream>
#include <fstream>
#include <cstring>
#include <setjmp.h>
#include <time.h>
#include "FLInlineData.hpp"

inline const int MEG = 1000000;

/*  this is the structure that holds the entire dungeon specifications  */
struct cel {
	int monster_hit_points;
	int monster_identification;
	int object_identification;
	int object_argument;
	int been_here_before;
};

/*  this is the structure definition for the items in the dnd store */
struct _itm {
	int price;
	int obj;
	int arg;
	int qty;
};

/* this is the structure for maintaining & moving the spheres of annihilation */
struct sphere {
	struct sphere *p;		/* pointer to next structure */
	int x, y, lev;		/* location of the sphere */
	int sphere_direction;
	int sphere_duration;
};

/*  this is the structure definition of the monster data
*/
struct monst {
	const char *name;
	int level;
	int armorclass;
	int damage;
	int attack;
	int defense;
	int genocided;
	int intelligence;		/* monsters intelligence -- used to choose movement */
	int gold;
	int hitpoints;
	unsigned long experience;
};

extern int regen_bottom;
extern char floorc, wallc;

/* extern char for the water. -Gibbon */
extern char waterc;
extern int beenhere[], cheat;
extern int object_identification[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION], iven[], been_here_before[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION];
extern char aborted[];
extern const char *classname[];
extern char lastmonst[];
extern char *fl_buffer_pointer, *lpbuf, *fl_buffer_pending, *inbuffer;
extern int level;
extern int monster_identification[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION], monstlevel[];
extern int nch[], ndgg[], nlpts[], nomove;
extern int nplt[], nsw[];
extern int potion_probability[];
extern int monstnamelist[];
extern const char *levelname[];
extern int objnamelist[];
extern char logname[];
extern int predostuff, restorflag;
extern char savefilename[];
extern char scorefile[];
extern int scroll_probability[];
extern int screen[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION], sex;
extern int spelknow[];
extern const char *spelmes[];
extern const char *speldescript[];
extern const char *spelcode[];
extern const char *spelname[];
extern int splev[], stealth[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION], wizard;
extern int diroffx[], diroffy[], hitflag, hit2flag, hit3flag,
       monster_hit_points[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION];
extern int object_argument[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION], ivenarg[], lasthx, lasthy, lastnum, lastpx,
       lastpy;
extern int oldx, oldy, player_horizontal_position, player_vertical_position;
extern int enable_scroll, y_larn_rep, wisid, lfd, fd;
extern long outstanding_taxes, gtime;
extern int skill[];
extern long cdesc[], cbak[];
extern time_t initialtime;
extern unsigned long lrandx;
extern struct sphere *spheres;
extern struct monst monster[];
extern struct _itm dnd_item[];
extern int prayed;
extern char scrollname[MAXSCROLL + 1][MAXSCROLLNAME];
extern char potionname[MAXPOTION + 1][MAXPOTIONNAME];
extern const char *objectname[];
extern int spelweird[FL_MAX_MONSTERS_IN_GAME + 8][FL_MAX_SPELLS_IN_GAME];
extern int random_monster_creation;
extern int dropflag;
extern int save_mode;
extern int lasttime;
extern int move_no_pickup;

/* Templates*/
template<class T>
T disappear(T x, T y) {
	return (monster_identification[x][y] = been_here_before[x][y] = 0);
}
