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

#include <time.h>
#include "FLIncludeData.hpp"
#include "../templates/FLTMath.hpp"
#include "../core/FLTermCapIO.hpp"
#include "../dungeon/FLDungeon.hpp"

const char *classname[] = {
	"Novice Explorer",     "Apprentice Explorer", "Practiced Explorer",  /*   -3 */
	"Expert Explorer",     "Novice Adventurer",   "Adventurer",          /*   -6 */
	"Apprentice Conjurer", "Conjurer",            "Master Conjurer",     /*   -9 */
	"Apprentice Mage",     "Mage",                "Experienced Mage",    /*  -12 */
	"Master Mage",         "Apprentice Warlord",  "Novice Warlord",      /*  -15 */
	"Expert Warlord",      "Master Warlord",      "Apprentice Gorgon",   /*  -18 */
	"Gorgon",              "Practiced Gorgon",    "Master Gorgon",       /*  -21 */
	"Demi-gorgon",         "Evil Master",         "Great Evil Master",   /*  -24 */
	"Mighty Evil Master",  "Mighty Evil Master",  "Mighty Evil Master",  /*  -27 */
	"Mighty Evil Master",  "Mighty Evil Master",  "Mighty Evil Master",  /*  -30 */
	"Mighty Evil Master",  "Mighty Evil Master",  "Mighty Evil Master",  /*  -33 */
	"Mighty Evil Master",  "Mighty Evil Master",  "Mighty Evil Master",  /*  -36 */
	"Mighty Evil Master",  "Mighty Evil Master",  "Mighty Evil Master",  /*  -39 */
	"Apprentice Demi-god", "Apprentice Demi-god", "Apprentice Demi-god", /*  -42 */
	"Apprentice Demi-god", "Apprentice Demi-god", "Apprentice Demi-god", /*  -45 */
	"Apprentice Demi-god", "Apprentice Demi-god", "Apprentice Demi-god", /*  -48 */
	"Minor Demi-god",      "Minor Demi-god",      "Minor Demi-god",      /*  -51 */
	"Minor Demi-god",      "Minor Demi-god",      "Minor Demi-god",      /*  -54 */
	"Minor Demi-god",      "Minor Demi-god",      "Minor Demi-god",      /*  -57 */
	"Major Demi-god",      "Major Demi-god",      "Major Demi-god",      /*  -60 */
	"Major Demi-god",      "Major Demi-god",      "Major Demi-god",      /*  -63 */
	"Major Demi-god",      "Major Demi-god",      "Major Demi-god",      /*  -66 */
	"Minor Deity",         "Minor Deity",         "Minor Deity",         /*  -69 */
	"Minor Deity",         "Minor Deity",         "Minor Deity",         /*  -72 */
	"Minor Deity",         "Minor Deity",         "Minor Deity",         /*  -75 */
	"Major Deity",         "Major Deity",         "Major Deity",         /*  -78 */
	"Major Deity",         "Major Deity",         "Major Deity",         /*  -81 */
	"Major Deity",         "Major Deity",         "Major Deity",         /*  -84 */
	"Novice Guardian",     "Novice Guardian",     "Novice Guardian",     /*  -87 */
	"Apprentice Guardian", "Apprentice Guardian", "Apprentice Guardian", /*  -90 */
	"Apprentice Guardian", "Apprentice Guardian", "Apprentice Guardian", /*  -93 */
	"Earth Guardian",      "Air Guardian",        "Fire Guardian",       /*  -96 */
	"Water Guardian",      "Time Guardian",       "Ethereal Guardian",   /*  -99 */
	"The Creator",         "The Creator",         "The Creator"          /* -102 */
};

/*
    table of experience needed to be a certain level of player
    skill[cdesc[FL_LEVEL]] is the experience required to attain the next level
*/
int skill[] = {
	0,      10,      20,      40,      80,     160,     320,     640,    1280,    2560,
	5120,   10240,   20480,   40960,  100000,  200000,  400000,  700000,   1 * MEG,   2 * MEG,
	3 * MEG,   4 * MEG,   5 * MEG,   6 * MEG,   8 * MEG,  10 * MEG,  12 * MEG,  14 * MEG,  16 * MEG,  18 * MEG,
	20 * MEG,  22 * MEG,  24 * MEG,  26 * MEG,  28 * MEG,  30 * MEG,  32 * MEG,  34 * MEG,  36 * MEG,  38 * MEG,
	40 * MEG,  42 * MEG,  44 * MEG,  46 * MEG,  48 * MEG,  50 * MEG,  52 * MEG,  54 * MEG,  56 * MEG,  58 * MEG,
	60 * MEG,  62 * MEG,  64 * MEG,  66 * MEG,  68 * MEG,  70 * MEG,  72 * MEG,  74 * MEG,  76 * MEG,  78 * MEG,
	80 * MEG,  82 * MEG,  84 * MEG,  86 * MEG,  88 * MEG,  90 * MEG,  92 * MEG,  94 * MEG,  96 * MEG,  98 * MEG,
	100 * MEG, 105 * MEG, 110 * MEG, 115 * MEG, 120 * MEG, 125 * MEG, 130 * MEG, 135 * MEG, 140 * MEG, 145 * MEG,
	150 * MEG, 155 * MEG, 160 * MEG, 165 * MEG, 170 * MEG, 175 * MEG, 180 * MEG, 185 * MEG, 190 * MEG, 195 * MEG,
	200 * MEG, 210 * MEG, 220 * MEG, 230 * MEG, 240 * MEG, 250 * MEG, 260 * MEG, 270 * MEG, 280 * MEG, 290 * MEG,
	300 * MEG
};

#undef MEG

char viewflag = 0;
char *lpbuf, *fl_buffer_pointer, *inbuffer, *fl_buffer_pending;	/* input/output pointers to the buffers */

struct cel *cell[(FL_MAX_HORIZONTAL_POSITION * FL_MAX_VERTICAL_POSITION) * (FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL)]; /* dungeon storage */
int monster_hit_points[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION];		/*  monster hp on level     */
int object_argument[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION];		/*  arg for the object_identification array  */
int object_identification[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION];		/*  objects in maze if any  */
int been_here_before[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION];		/*  1 or 0 if here before   */
int monster_identification[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION];		/*  monster object_identification array      */
int stealth[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION];	/*  0=sleeping 1=awake monst */
char lastmonst[40];		/*  this has the name of the current monster    */
int beenhere[FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL];	/*  1 if have been on this level */
int predostuff = 0;		/*  2 means that the trap handling routines must do a
				   showplayer() after a trap.  0 means don't showplayer()
				   0 - we are in create player screen
				   1 - we are in welcome screen
				   2 - we are in the normal game   */

char logname[FL_MAX_PLAYER_NAME_LENGTH];	/* the player's name */

int cheat = 0;			/*  1 if the player has fudged save file            */
int level = 0;			/*  cavelevel player is on = cdesc[FL_CAVE_LEVEL]           */
int wizard = 0;			/*  the wizard mode flag                            */
int lastnum = 0;		/* the number of the monster last hitting player    */
int hitflag = 0;		/*  flag for if player has been hit when running    */
int hit2flag = 0;		/*  flag for if player has been hit when running    */
int hit3flag = 0;		/*  flag for if player has been hit flush input     */
int player_horizontal_position, player_vertical_position;		/*  the room on the present level of the player     */
int lastpx, lastpy;		/*  0 --- FL_MAX_HORIZONTAL_POSITION-1  or  0 --- FL_MAX_VERTICAL_POSITION-1                  */
int oldx, oldy;
int prayed = 1;			/* did player pray at an altar (command mode)? needs
				   to be saved, but I don't want to add incompatibility
				   right now.  KBR 1/11/90 */
int lasthx = 0, lasthy = 0;	/* location of monster last hit by player       */
unsigned long lrandx = 33601;	/*  the random number seed                      */
time_t initialtime = 0;		/* time playing began                           */
long gtime = 0;			/*  the clock for the game                      */
long outstanding_taxes = 0;	/* present tax bill from score file             */
long cdesc[100], cbak[100];	/*  the character description arrays            */
int enable_scroll = 0;		/* constant for enabled/disabled scrolling regn */
char aborted[] = " aborted";
struct sphere *spheres = 0;	/*pointer to linked list for spheres of annihilation */

const char *levelname[14] = {
	"H", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
	"T1", "T2", "T3"
};

/*  This char array was too short.  MAXOBJECT is defined as 92 and having this
    as +2 (making it 94) was too short to hold the entire array. +36 fixes this as it
    is the minimum required to hold all objects. -Gibbon
*/

int objnamelist[88] = {
	'.',  //
	':',  //a holy altar
	'\\', //a handsome jewel encrusted throne
	'_',  //the orb
	'^',  //a pit
	'<',  //a staircase leading upwards
	'_',  //an elevator going up
	'{',  //a bubbling fountain
	'%',  //a great marble statue
	'^',  //a teleport trap
	'@',	//a Skeleton
	'|',  //a mirror
	'2',  //the DND store
	'>',  //a staircase going down
	'_',  //an elevator going down
	'5',  //the 5th branch of the bank of Larn
	'5',  //the bank of Larn
	'}',  //a dead fountain
	'$',  //gold
	'\'', //an open door
	'+',  //a closed door
	'#',  //a wall
	'~',  //the Eye of Larn
	'[',  //plate mail
	'[',  //chain mail
	'[',  //leather armor
	')',  //a sword of slashing
	')',  //Bessman's flailing hammer
	')',  //a sunsword
	')',  //a two handed sword
	')',  //a spear
	')',  //a dagger
	'=',  //ring of extra regeneration
	'=',  //a ring of regeneration
	'=',  //a ring of protection
	'=',  //an energy ring
	'=',  //a ring of dexterity
	'=',  //a ring of strength
	'=',  //a ring of cleverness
	'=',  //a ring of increase damage
	'-',  //a belt of striking
	'\?', //a magic scroll
	'!',  //a magic potion
	'\?', //a book
	'&',  //a chest
	'~',  //an amulet of invisibility
	'~',  //an orb of dragon slaying
	'~',  //a scarab of negate spirit
	'~',  //a cube of undead control
	'~',  //device of theft prevention
	'*',  //a brilliant diamond
	'*',  //a ruby
	'*',  //an enchanting emerald
	'*',  //a sparkling sapphire
	'8',  //the dungeon entrance
	'9',  //the Temple of Larn
	'9',  //the base of the Temple of Larn
	')',  //a battle axe
	')',  //a longsword
	')',  //a flail
	'[',  //ring mail
	'[',  //studded leather armor
	'[',  //splint mail
	'[',  //plate armor
	'[',  //stainless plate armor
	')',  //a lance of death
	'^',  //an arrow trap
	'.',  //an arrow trap
	'[',  //a shield
	'1',  //your home
	'$',  //gold
	'$',  //gold
	'$',  //gold
	'.',  //a dart trap
	'^',  //a dart trap
	'^',  //a trapdoor
	'.',  //a trapdoor
	'3',  //the local trading post
	'.',  //a teleport trap
	'/',  //a massive throne
	'0',  //a sphere of annihilation
	'\\', //a handsome jewel encrusted throne
	'4',  //the Larn Revenue Service
	',',   //a fortune cookie
	'?', // A Prayer Book
	'|', // A Candle
	';', //Silver Ghani Statue
	'[' // A Fire Tunic
};

int monstnamelist[65] = {
	'.', //
	'B', //bat
	'G', //gnome
	'H', //hobgoblin
	'J', //jackal
	'K', //kobold
	'O', //orc
	'S', //snake
	'c', //giant centipede
	'j', //jaculi
	't', //troglodyte
	'A', //giant ant
	'E', //floating eye
	'L', //leprechaun
	'N', //nymph
	'Q', //quasit
	'R', //rust monster
	'Z', //zombie
	'a', //assassin bug
	'b', //bugbear
	'h', //hell hound
	'i', //ice lizard
	'C', //centaur
	'T', //troll
	'Y', //yeti
	'd', //white dragon
	'e', //elf
	'g', //gelatinous cube
	'm', //metamorph
	'v', //vortex
	'z', //ziller
	'F', //violet fungi
	'W', //wraith
	'f', //forvalaka
	'l', //lama nobe
	'o', //osequip
	'r', //rothe
	'X', //xorn
	'V', //vampire
	'I', //invisible stalker
	'p', //poltergeist
	'q', //disenchantress
	's', //shambling mound
	'y', //yellow mold
	'U', //umber hulk
	'k', //gnome king
	'M', //mimic
	'w', //water lord
	'D', //bronze dragon
	'D', //green dragon
	'P', //purple worm
	'x', //xvart
	'n', //spirit naga
	'D', //silver dragon
	'D', //platinum dragon
	'u', //green urchin
	'D', //red dragon
	'.', //type I demon lord
	'.', //type II demon lord
	'.', //type III demon lord
	'.', //type IV demon lord
	'.', //type V demon lord
	'.', //type VI demon lord
	'.', //type VII demon lord
	'.'  //demon prince
};

char floorc = '.';
char wallc = '#';

/* Adding water for implementation in future builds. -Gibbon */
char waterc = '~';


const char *objectname[] = {
	0, "a holy altar", "a handsome jewel encrusted throne", "the orb",
	"a pit",
	"a staircase leading upwards", "an elevator going up",
	"a bubbling fountain",
	"a great marble statue", "a teleport trap", "a Skeleton",
	"a mirror", "the DND store", "a staircase going down",
	"an elevator going down",
	"the bank of Larn", "the 5th branch of the Bank of Larn",
	"a dead fountain", "gold", "an open door", "a closed door",
	"a wall", "the Eye of Larn", "plate mail", "chain mail", "leather armor",
	"a sword of slashing", "Bessman's flailing hammer", "a sunsword",
	"a two handed sword", "a spear", "a dagger",
	"ring of extra regeneration", "a ring of regeneration",
	"a ring of protection",
	"an energy ring", "a ring of dexterity", "a ring of strength",
	"a ring of cleverness", "a ring of increase damage", "a belt of striking",
	"a magic scroll", "a magic potion", "a Book", "a chest",
	"an amulet of invisibility", "an orb of dragon slaying",
	"a scarab of negate spirit", "a cube of undead control",
	"device of theft prevention", "a brilliant diamond", "a ruby",
	"an enchanting emerald", "a sparkling sapphire", "the dungeon entrance",
	"the Temple of Larn", "the exit of the Temple of Larn",
	"a battle axe", "a longsword", "a longsword of Hymie", "ring mail",
	"Studded Leather Armor",
	"Splint Mail", "Plate Armor", "Stainless Plate Armor",
	"Great Sword of Death",
	"an Arrow Trap", "an Arrow Trap", "a Shield", "your Home",
	"Gold", "Gold", "Gold", "a Dart Trap",
	"a Dart Trap", "a Trapdoor", "a Trapdoor", "the local Trading Post",
	"a Teleport Trap", "a Massive Throne",
	"a Sphere of Annihilation", "a Handsome Jewel Encrusted Throne",
	"the Larn Revenue Service", "a Fortune Cookie", "a Prayer Book",
	"a Candle", "a Silver Ghani Statue","A Fire Tunic", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""
};

/*
    for the monster data

    array to do TRnd() to create monsters <= a given level
*/
int monstlevel[] = { 5, 11, 17, 22, 27, 33, 39, 42, 46, 50, 53, 56, 59 };

struct monst monster[] = {
	/*  NAME            LV  FL_AC  DAM ATT DEF GEN INT FL_GOLD    HP  EXP
	    ----------------------------------------------------------------- */
	{"", 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},
	{"bat", 1, 0, 1, 0, 0, 0, 3, 0, 1, 1},
	{"gnome", 1, 10, 1, 0, 0, 0, 8, 30, 2, 2},
	{"hobgoblin", 1, 14, 2, 0, 0, 0, 5, 25, 3, 2},
	{"jackal", 1, 17, 1, 0, 0, 0, 4, 0, 1, 1},
	{"kobold", 1, 20, 1, 0, 0, 0, 7, 10, 1, 1},

	{"orc", 2, 12, 1, 0, 0, 0, 9, 40, 4, 2},
	{"snake", 2, 15, 1, 0, 0, 0, 3, 0, 3, 1},
	{"giant centipede", 2, 14, 0, 4, 0, 0, 3, 0, 1, 2},
	{"jaculi", 2, 20, 1, 0, 0, 0, 3, 0, 2, 1},
	{"troglodyte", 2, 10, 2, 0, 0, 0, 5, 80, 4, 3},
	{"giant ant", 2, 8, 1, 4, 0, 0, 4, 0, 5, 5},

	/*  NAME            LV  FL_AC  DAM ATT DEF GEN INT FL_GOLD    HP  EXP
	    ----------------------------------------------------------------- */

	{"floating eye", 3, 8, 1, 0, 0, 0, 3, 0, 5, 2},
	{"leprechaun", 3, 3, 0, 8, 0, 0, 3, 1500, 13, 45},
	{"nymph", 3, 3, 0, 14, 0, 0, 9, 0, 18, 45},
	{"quasit", 3, 5, 3, 0, 0, 0, 3, 0, 10, 15},
	{"rust monster", 3, 4, 0, 1, 0, 0, 3, 0, 18, 25},
	{"zombie", 3, 12, 2, 0, 0, 0, 3, 0, 6, 7},

	{"assassin bug", 4, 9, 3, 0, 0, 0, 3, 0, 20, 15},
	{"bugbear", 4, 5, 4, 15, 0, 0, 5, 40, 20, 35},
	{"hell hound", 4, 5, 2, 2, 0, 0, 6, 0, 16, 35},
	{"ice lizard", 4, 11, 2, 10, 0, 0, 6, 50, 16, 25},
	{"centaur", 4, 6, 4, 0, 0, 0, 10, 40, 24, 45},

	/*  NAME            LV  FL_AC  DAM ATT DEF GEN INT FL_GOLD    HP  EXP
	    ----------------------------------------------------------------- */

	{"troll", 5, 4, 5, 0, 0, 0, 9, 80, 50, 300},
	{"yeti", 5, 6, 4, 0, 0, 0, 5, 50, 35, 100},
	{"white dragon", 5, 2, 4, 5, 0, 0, 16, 500, 55, 1000},
	{"elf", 5, 8, 1, 0, 0, 0, 15, 50, 22, 35},
	{"gelatinous cube", 5, 9, 1, 0, 0, 0, 3, 0, 22, 45},

	{"metamorph", 6, 7, 3, 0, 0, 0, 3, 0, 30, 40},
	{"vortex", 6, 4, 3, 0, 0, 0, 3, 0, 30, 55},
	{"ziller", 6, 15, 3, 0, 0, 0, 3, 0, 30, 35},
	{"violet fungi", 6, 12, 3, 0, 0, 0, 3, 0, 38, 100},
	{"wraith", 6, 3, 1, 6, 0, 0, 3, 0, 30, 325},
	{"forvalaka", 6, 2, 5, 0, 0, 0, 7, 0, 50, 280},

	/*  NAME            LV  FL_AC  DAM ATT DEF GEN INT FL_GOLD    HP  EXP
	    ----------------------------------------------------------------- */

	{"lama nobe", 7, 7, 3, 0, 0, 0, 6, 0, 35, 80},
	{"osequip", 7, 4, 3, 16, 0, 0, 4, 0, 35, 100},
	{"rothe", 7, 15, 5, 0, 0, 0, 3, 100, 50, 250},
	{"xorn", 7, 0, 6, 0, 0, 0, 13, 0, 60, 300},
	{"vampire", 7, 3, 4, 6, 0, 0, 17, 0, 50, 1000},
	{"invisible stalker", 7, 3, 6, 0, 0, 0, 5, 0, 50, 350},

	{"poltergeist", 8, 1, 4, 0, 0, 0, 3, 0, 50, 450},
	{"disenchantress", 8, 3, 0, 9, 0, 0, 3, 0, 50, 500},
	{"shambling mound", 8, 2, 5, 0, 0, 0, 6, 0, 45, 400},
	{"yellow mold", 8, 12, 4, 0, 0, 0, 3, 0, 35, 250},
	{"umber hulk", 8, 3, 7, 11, 0, 0, 14, 0, 65, 600},

	/*  NAME            LV  FL_AC  DAM ATT DEF GEN INT FL_GOLD    HP  EXP
	    ----------------------------------------------------------------- */

	{"gnome king", 9, -1, 10, 0, 0, 0, 18, 2000, 100, 3000},
	{"mimic", 9, 5, 6, 0, 0, 0, 8, 0, 55, 99},
	{"water lord", 9, -10, 15, 7, 0, 0, 20, 0, 150, 15000},
	{"bronze dragon", 9, 2, 9, 3, 0, 0, 16, 300, 80, 4000},
	{"green dragon", 9, 3, 8, 10, 0, 0, 15, 200, 70, 2500},
	{"purple worm", 9, -1, 11, 0, 0, 0, 3, 100, 120, 15000},
	{"xvart", 9, -2, 12, 0, 0, 0, 13, 0, 90, 1000},

	{"spirit naga", 10, -20, 12, 12, 0, 0, 23, 0, 95, 20000},
	{"silver dragon", 10, -1, 12, 3, 0, 0, 20, 700, 100, 10000},
	{"platinum dragon", 10, -5, 15, 13, 0, 0, 22, 1000, 130, 24000},
	{"green urchin", 10, -3, 12, 0, 0, 0, 3, 0, 85, 5000},
	{"red dragon", 10, -2, 13, 3, 0, 0, 19, 800, 110, 14000},

	{"type I demon lord", 12, -30, 18, 0, 0, 0, 20, 0, 140, 50000},
	{"type II demon lord", 13, -30, 18, 0, 0, 0, 21, 0, 160, 75000},
	{"type III demon lord", 14, -30, 18, 0, 0, 0, 22, 0, 180, 100000},
	{"type IV demon lord", 15, -35, 20, 0, 0, 0, 23, 0, 200, 125000},
	{"type V demon lord", 16, -40, 22, 0, 0, 0, 24, 0, 220, 150000},
	{"type VI demon lord", 17, -45, 24, 0, 0, 0, 25, 0, 240, 175000},
	{"type VII demon lord", 18, -70, 27, 6, 0, 0, 26, 0, 260, 200000},
	{"demon prince", 25, -127, 30, 6, 0, 0, 28, 0, 345, 300000}

	/*  NAME                LV  FL_AC  DAM ATT DEF GEN INT FL_GOLD    HP  EXP
	    --------------------------------------------------------------------- */
};

/*  name array for scrolls      */

char scrollname[MAXSCROLL + 1][MAXSCROLLNAME] = {
	"\0of enchant armor",
	"\0of enchant weapon",
	"\0of enlightenment",
	"\0of blank paper",
	"\0of create monster",
	"\0of create artifact",
	"\0of aggravate monsters",
	"\0of time warp",
	"\0of teleportation",
	"\0of expanded awareness",
	"\0of haste monsters",
	"\0of monster healing",
	"\0of spirit protection",
	"\0of undead protection",
	"\0of stealth",
	"\0of magic mapping",
	"\0of hold monsters",
	"\0of gem perfection",
	"\0of spell extension",
	"\0of identify",
	"\0of remove curse",
	"\0of annihilation",
	"\0of pulverization",
	"\0of life protection",
	"\0 ",
	"\0 ",
	"\0 ",
	"\0 ",
	"\0zzzzzzzzzzzzzz"		/* sentinel, for the sorted known objects inventory */
};

/*  name array for magic potions    */
char potionname[MAXPOTION + 1][MAXPOTIONNAME] = {
	"\0of sleep",
	"\0of healing",
	"\0of raise level",
	"\0of increase ability",
	"\0of wisdom",
	"\0of strength",
	"\0of raise charisma",
	"\0of dizziness",
	"\0of learning",
	"\0of object detection",
	"\0of monster detection",
	"\0of forgetfulness",
	"\0of water",
	"\0of blindness",
	"\0of confusion",
	"\0of heroism",
	"\0of sturdiness",
	"\0of giant strength",
	"\0of fire resistance",
	"\0of treasure finding",
	"\0of instant healing",
	" of cure dianthroritis",
	"\0of poison",
	"\0of see invisible",
	"\0 ",
	"\0 ",
	"\0 ",
	"\0 ",
	"\0 ",
	"\0 ",
	"\0 ",
	"\0 ",
	"\0 ",
	"\0 ",
	"\0 ",
	"\0zzzzzzzzzzzzzz"		/* sentinel, for the sorted known objects inventory */
};


/*
    spell data
*/
int spelknow[FL_MAX_SPELLS_IN_GAME];
int splev[] = { 1, 4, 9, 14, 18, 22, 26, 29, 32, 35, 37, 37, 37, 37, 37 };

const char *spelcode[FL_MAX_SPELLS_IN_GAME + 1] = {
	"pro", "mle", "dex", "sle", "chm", "ssp",
	"web", "str", "enl", "hel", "cbl", "cre", "pha", "inv",
	"bal", "cld", "ply", "can", "has", "ckl", "vpr",
	"dry", "lit", "drl", "glo", "flo", "fgr",
	"sca", "hld", "stp", "tel", "mfi",	/* 31 */
	"sph", "gen", "sum", "wtw", "alt", "per", "zzz"
};

const char *spelname[] = {
	"protection", "magic missile", "dexterity",
	"sleep", "charm monster", "sonic spear",

	"web", "strength", "enlightenment",
	"healing", "cure blindness", "create monster",
	"phantasmal forces", "invisibility",

	"fireball", "cold", "polymorph",
	"cancellation", "haste self", "cloud kill",
	"vaporize rock",

	"dehydration", "lightning", "drain life",
	"invulnerability", "flood", "finger of death",

	"scare monster", "hold monster", "time stop",
	"teleport away", "magic fire",

	"sphere of annihilation", "genocide", "summon demon",
	"walk through walls", "alter reality", "permanence",
	""
};

const char *speldescript[] = {
	/* 1 */
	"generates a +2 protection field",
	"creates and hurls a magic missile equivalent to a + 1 magic arrow",
	"adds +2 to the casters dexterity",
	"causes some monsters to go to sleep",
	"some monsters may be awed at your magnificence",
	"causes your hands to emit a screeching sound toward what they point",
	/* 7 */
	"causes strands of sticky thread to entangle an enemy",
	"adds +2 to the casters strength for a short term",
	"the caster becomes aware of things around him",
	"restores some hp to the caster",
	"restores sight to one so unfortunate as to be blinded",
	"creates a monster near the caster appropriate for the location",
	"creates illusions, and if believed, monsters die",
	"the caster becomes invisible",
	/* 15 */
	"makes a ball of fire that burns on what it hits",
	"sends forth a cone of cold which freezes what it touches",
	"you can find out what this does for yourself",
	"negates the ability of a monster to use his special abilities",
	"speeds up the casters movements",
	"creates a fog of poisonous gas which kills all that is within it",
	"this changes rock to air",
	/* 22 */
	"dries up water in the immediate vicinity",
	"you finger will emit a lightning bolt when this spell is cast",
	"subtracts hit points from both you and a monster",
	"this globe helps to protect the player from physical attack",
	"this creates an avalanche of H2O to flood the immediate chamber",
	"this is a holy spell and calls upon your god to back you up",
	/* 28 */
	"terrifies the monster so that hopefully he wont hit the magic user",
	"the monster is frozen in his tracks if this is successful",
	"all movement in the caverns ceases for a limited duration",
	"moves a particular monster around in the dungeon (hopefully away from you)",
	"this causes a curtain of fire to appear all around you",
	/* 33 */
	"anything caught in this sphere is instantly killed.  Warning -- dangerous",
	"eliminates a species of monster from the game -- use sparingly",
	"summons a demon who hopefully helps you out",
	"allows the player to walk through walls for a short period of time",
	"god only knows what this will do",
	"makes a character spell permanent, i. e. protection, strength, etc.",
	""
};

int spelweird[FL_MAX_MONSTERS_IN_GAME + 8][FL_MAX_SPELLS_IN_GAME] = {
	/*                      p m d s c s    w s e h c c p i    b c p c h c v    d l d g f f    s h s t m    s g s w a p */
	/*                      r l e l h s    e t n e b r h n    a l l a a k p    r i r l l g    c l t e f    p e u t l e */
	/*                      o e x e m p    b r l l l e a v    l d y n s l r    y t l o o r    a d p l i    h n m w t r */


	/*            bat */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*          gnome */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*      hobgoblin */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*         jackal */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*         kobold */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},

	/*            orc */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*          snake */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*giant centipede */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*         jaculi */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*     troglodyte */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},

	/*      giant ant */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*   floating eye */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*     leprechaun */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*          nymph */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*         quasit */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},

	/*   rust monster */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*         zombie */ {
		0, 0, 0, 8, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*   assassin bug */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*        bugbear */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*     hell hound */ {
		0, 6, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},

	/*     ice lizard */ {
		0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0
	},
	/*        centaur */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*          troll */ {
		0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0,
		0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*           yeti */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*   white dragon */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 15, 0,
		0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0
	},

	/*            elf */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 5, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*gelatinous cube */ {
		0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*      metamorph */ {
		0, 13, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*         vortex */ {
		0, 13, 0, 0, 0, 10, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 4, 0, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0
	},
	/*         ziller */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},

	/*   violet fungi */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*         wraith */ {
		0, 0, 0, 8, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*      forvalaka */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*      lama nobe */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*        osequip */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},

	/*          rothe */ {
		0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*           xorn */ {
		0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0,
		0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*        vampire */ {
		0, 0, 0, 8, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*invisible staker */ {
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*    poltergeist */ {
		0, 13, 0, 8, 0, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0,
		0, 4, 0, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},

	/* disenchantress */ {
		0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*shambling mound */ {
		0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*    yellow mold */ {
		0, 0, 0, 8, 0, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0,
		0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*     umber hulk */ {
		0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*     gnome king */ {
		0, 7, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 9, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},

	/*          mimic */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*     water lord */ {
		0, 13, 0, 8, 3, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0,
		0, 4, 0, 0, 0, 0, 0, 16, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*  bronze dragon */ {
		0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*   green dragon */ {
		0, 7, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*    purple worm */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},

	/*          xvart */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*    spirit naga */ {
		0, 13, 0, 8, 3, 4, 1, 0, 0, 0, 0, 0, 0, 5, 0, 4, 9, 0,
		0, 4, 0, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*  silver dragon */ {
		0, 6, 0, 9, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*platinum dragon */ {
		0, 7, 0, 9, 0, 0, 11, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0
	},
	/*   green urchin */ {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},
	/*     red dragon */ {
		0, 6, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0
	},

	/*                      p m d s c s    w s e h c c p i    b c p c h c v    d l d g f f    s h s t m    s g s w a p */
	/*                      r l e l h s    e t n e b r h n    a l l a a k p    r i r l l g    c l t e f    p e u t l e */
	/*                      o e x e m p    b r l l l e a v    l d y n s l r    y t l o o r    a d p l i    h n m w t r */

	/*     demon lord */ {
		0, 7, 0, 4, 3, 0, 1, 0, 0, 0, 0, 0, 14, 5, 0, 0, 4, 0,
		0, 4, 0, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 9, 0, 0, 0,
		0, 0
	},
	/*     demon lord */ {
		0, 7, 0, 4, 3, 0, 1, 0, 0, 0, 0, 0, 14, 5, 0, 0, 4, 0,
		0, 4, 0, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 9, 0, 0, 0,
		0, 0
	},
	/*     demon lord */ {
		0, 7, 0, 4, 3, 0, 1, 0, 0, 0, 0, 0, 14, 5, 0, 0, 4, 0,
		0, 4, 0, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 9, 0, 0, 0,
		0, 0
	},
	/*     demon lord */ {
		0, 7, 0, 4, 3, 0, 1, 0, 0, 0, 0, 0, 14, 5, 0, 0, 4, 0,
		0, 4, 0, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 9, 0, 0, 0,
		0, 0
	},
	/*     demon lord */ {
		0, 7, 0, 4, 3, 0, 1, 0, 0, 0, 0, 0, 14, 5, 0, 0, 4, 0,
		0, 4, 0, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 9, 0, 0, 0,
		0, 0
	},
	/*     demon lord */ {
		0, 7, 0, 4, 3, 0, 1, 0, 0, 0, 0, 0, 14, 5, 0, 0, 4, 0,
		0, 4, 0, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 9, 0, 0, 0,
		0, 0
	},
	/*     demon lord */ {
		0, 7, 0, 4, 3, 0, 1, 0, 0, 0, 0, 0, 14, 5, 0, 0, 4, 0,
		0, 4, 0, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 9, 0, 0, 0,
		0, 0
	},
	/*   demon prince */ {
		0, 7, 0, 4, 3, 9, 1, 0, 0, 0, 0, 0, 14, 5, 0, 0, 4, 0,
		0, 4, 0, 4, 0, 0, 0, 4, 4, 4, 0, 0, 0, 4, 9, 0, 0, 0,
		0, 0
	}

};

const char *spelmes[] = { "",
                          /*  1 */ "the web had no effect on the %s",
                          /*  2 */ "the %s changed shape to avoid the web",
                          /*  3 */ "the %s isn't afraid of you",
                          /*  4 */ "the %s isn't affected",
                          /*  5 */ "the %s can see you with his infravision",
                          /*  6 */ "the %s vaporizes your missile",
                          /*  7 */ "your missile bounces off the %s",
                          /*  8 */ "the %s doesn't sleep",
                          /*  9 */ "the %s resists",
                          /* 10 */ "the %s can't hear the noise",
                          /* 11 */ "the %s's tail cuts it free of the web",
                          /* 12 */ "the %s burns through the web",
                          /* 13 */ "your missiles pass right through the %s",
                          /* 14 */ "the %s sees through your illusions",
                          /* 15 */ "the %s loves the cold!",
                          /* 16 */ "the %s loves the water!"
                        };

/*
    function to create scroll numbers with appropriate probability of
    occurrence

    0 - armor           1 - weapon      2 - enlightenment   3 - paper
    4 - create monster  5 - create object_identification 6 - aggravate       7 - time warp
    8 - teleportation   9 - expanded awareness              10 - haste monst
    11 - heal monster   12 - spirit protection      13 - undead protection
    14 - stealth        15 - magic mapping          16 - hold monster
    17 - gem perfection 18 - spell extension        19 - identify
    20 - remove curse   21 - annihilation           22 - pulverization
    23 - life protection
*/
int scroll_probability[] = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3,
                             3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9,
                             9, 9, 10, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14,
                             15, 15, 16, 16, 16, 17, 17, 18, 18, 19, 19, 19, 20, 20, 20, 20, 21, 22,
                             22, 22, 23
                           };

/*
    function to return a potion number created with appropriate probability
    of occurrence

    0 - sleep               1 - healing                 2 - raise level
    3 - increase ability    4 - gain wisdom             5 - gain strength
    6 - increase charisma   7 - dizziness               8 - learning
    9 - object detection    10 - monster detection      11 - forgetfulness
    12 - water              13 - blindness              14 - confusion
    15 - heroism            16 - sturdiness             17 - giant strength
    18 - fire resistance    19 - treasure finding       20 - instant healing
    21 - cure dianthroritis 22 - poison                 23 - see invisible
*/
int potion_probability[] = { 0, 0, 1, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 9, 9, 9,
                             10, 10, 10, 11, 11, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 19,
                             20, 20, 22, 22, 23, 23
                           };

int nlpts[] = { 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 6, 7 };
int nch[] = { 0, 0, 0, 1, 1, 1, 2, 2, 3, 4 };
int nplt[] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 4 };
int ndgg[] = { 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 4, 5 };
int nsw[] = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 3 };