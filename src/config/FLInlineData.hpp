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

#define HAVESEEN  0x1
#define KNOWHERE  0x2
#define KNOWALL   (HAVESEEN | KNOWHERE)
#define LARNHOME ""
/* tax rate for the LRS */
#define TAXRATE 1/20

/*  Inline functions and variables.
    Much better, safer and cleaner!
    ~Gibbon
*/
inline const int FL_MAX_HORIZONTAL_POSITION = 67;
inline const int FL_MAX_VERTICAL_POSITION = 17;
inline const int ST_START = 1;
inline const int ST_END = 2;
inline const int FL_CLEAR = 5;
inline const int CL_LINE = 6;
inline const int T_INIT = 7;
inline const int T_END = 8;
inline const int CL_DOWN = 14;
inline const int FL_CURSOR = 15;

#if defined NIX
	inline const char* FL_SCORE_FILE_NAME =	"/var/tmp/fl_scorefile";
	inline const char* FL_LOG_FILE_NAME = 	"/var/tmp/fl_logfile";
	inline const char* FL_PLAYER_ID = 	"/var/tmp/fl_playerid";
	inline const char* FL_SAVE_FILE_NAME = 	"/var/tmp/fl_savefile";
	inline const char* FL_MESSAGE_LOG_NAME = "/var/tmp/fl_messages";
	inline const char* FL_CONFIG_FILE_NAME = "/usr/share/freelarn/fl_config.json";
#endif

#if defined WINDOWS || NIX_LOCAL
	inline const char* FL_SCORE_FILE_NAME =	"fl_scorefile";
	inline const char* FL_LOG_FILE_NAME = 	"fl_logfile";
	inline const char* FL_PLAYER_ID = 	"fl_playerid";
	inline const char* FL_SAVE_FILE_NAME = 	"fl_savefile";
	inline const char* FL_MESSAGE_LOG_NAME = "fl_messages";
	inline const char* FL_CONFIG_FILE_NAME = "fl_config.json";
#endif

/* Destroy object at present location */
extern int object_identification[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION];
extern int player_horizontal_position;
extern int player_vertical_position;
extern int been_here_before[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION];
inline int fl_forget_data(void) {
	return object_identification[player_horizontal_position][player_vertical_position] = been_here_before[player_horizontal_position][player_vertical_position] = 0;
}

/* Turn on bold display for the terminal */
extern char *fl_buffer_pointer;
inline int fl_set_text_to_bold(void) {
	return *fl_buffer_pointer++ = ST_START;
}

/* Turn off bold display for the terminal */
inline int fl_reset_text_bold(void) {
	return *fl_buffer_pointer++ = ST_END;
}

/* Clear the screen and home the cursor */
inline int fl_clear_and_reset_screen(void) {
	int regen_bottom = 1;
	return *fl_buffer_pointer++ = FL_CLEAR, regen_bottom;
}
inline int FL_BOLD(void) {
	const int bold = 3;
	return bold;
}
inline int FL_END_BOLD(void) {
	const int end_bold = 4;
	return end_bold;
}
inline int FL_TOTAL_MOBUL_LIMIT(void) {
	const int timelimit = 70000;
	return timelimit;
}
inline const char* FL_BUILD_VERSION = "CENTAUR";
inline const char* FL_APACHE2_NOTICE = "FreeLarn is Free Software distributed under the Apache 2.0 License";

extern char *fl_buffer_pointer;
inline int FL_CLEAR_TO_END_OF_LINE(void) {
	return *fl_buffer_pointer++ = CL_LINE;
}

inline const int FL_MAX_CAVE_OF_LARN_LEVEL = 11;
inline const int FL_MAX_TEMPLE_OF_LARN_LEVEL = 3;
inline const int FL_MAX_PATH_NAME_LENGTH = 80;

inline bool FL_WIZARD_MODE_ENABLED = false;

/* this is the number of people on a scoreboard max */
inline const int FL_TOTAL_NAMES_ON_SCORE_BOARD = 4;
/* maximum player level allowed */
inline const int FL_MAX_PLAYER_LEVEL = 100;
/* maximum number of spells in existance */
inline const int FL_MAX_SPELLS_IN_GAME = 38;
/* size of the output buffer */
inline const int FL_OUTPUT_BUFFER = 4096;
/* size of the input buffer */
inline const int FL_INPUT_BUFFER = 4096;
/* max size of the player's name */
inline const int FL_MAX_PLAYER_NAME_LENGTH = 20;
/* max size of the savefile path */
inline const int FL_SAVE_FILE_NAME_SIZE = 128;
inline const int FL_STRING_BUFFER_SIZE = 256;

/* maximum # monsters in the dungeon */
inline const int FL_MAX_MONSTERS_IN_GAME = 56;
/* Maximum number of mazes */
inline const int FL_MAX_MAZES_IN_GAME = 24;

/*  defines for the monsters as objects  */
inline const int BAT = 1;
inline const int GNOME = 2;
inline const int HOBGOBLIN = 3;
inline const int JACKAL = 4;
inline const int KOBOLD = 5;
inline const int ORC = 6;
inline const int SNAKE = 7;
inline const int CENTIPEDE = 8;
inline const int JACULI = 9;
inline const int TROGLODYTE = 10;
inline const int ANT = 11;
inline const int EYE = 12;
inline const int LEPRECHAUN = 13;
inline const int NYMPH = 14;
inline const int QUASIT = 15;
inline const int RUSTMONSTER = 16;
inline const int ZOMBIE = 17;
inline const int ASSASSINBUG = 18;
inline const int BUGBEAR = 19;
inline const int HELLHOUND = 20;
inline const int ICELIZARD = 21;
inline const int CENTAUR = 22;
inline const int TROLL = 23;
inline const int YETI = 24;
inline const int WHITEDRAGON = 25;
inline const int ELF = 26;
inline const int CUBE = 27;
inline const int METAMORPH = 28;
inline const int VORTEX = 29;
inline const int ZILLER = 30;
inline const int VIOLETFUNGI = 31;
inline const int WRAITH = 32;
inline const int FORVALAKA = 33;
inline const int LAMANOBE = 34;
inline const int OSEQUIP = 35;
inline const int ROTHE = 36;
inline const int XORN = 37;
inline const int VAMPIRE = 38;
inline const int INVISIBLESTALKER = 39;
inline const int POLTERGEIST = 40;
inline const int DISENCHANTRESS = 41;
inline const int SHAMBLINGMOUND = 42;
inline const int YELLOWMOLD = 43;
inline const int UMBERHULK = 44;
inline const int GNOMEKING = 45;
inline const int MIMIC = 46;
inline const int WATERLORD = 47;
inline const int BRONZEDRAGON = 48;
inline const int GREENDRAGON = 49;
inline const int PURPLEWORM = 50;
inline const int XVART = 51;
inline const int SPIRITNAGA = 52;
inline const int SILVERDRAGON = 53;
inline const int PLATINUMDRAGON = 54;
inline const int GREENURCHIN = 55;
inline const int REDDRAGON = 56;
inline const int DEMONLORD = 57;
inline const int DEMONPRINCE = 64;

/*
    defines for the character attribute array   cdesc[]
*/
inline const int FL_STRENGTH = 0;		/* characters physical strength not due to objects */
inline const int FL_INTELLIGENCE = 1;
inline const int FL_WISDOM = 2;
inline const int FL_CONSTITUTION = 3;
inline const int FL_DEXTERITY = 4;
inline const int FL_CHARISMA = 5;
inline const int FL_HPMAX = 6;
inline const int FL_HP = 7;
inline const int FL_GOLD = 8;
inline const int FL_EXPERIENCE = 9;
inline const int FL_LEVEL = 10;
inline const int FL_REGEN = 11;
inline const int FL_WCLASS = 12;
inline const int FL_AC = 13;
inline const int FL_BANK_ACCOUNT = 14;
inline const int FL_SPELLMAX = 15;
inline const int FL_SPELLS = 16;
inline const int FL_ENERGY = 17;
inline const int FL_ECOUNTER = 18;
inline const int FL_MOREDEFENSES = 19;
inline const int FL_WEAR = 20;
inline const int FL_PROTECTIONTIME = 21;
inline const int FL_WIELD = 22;
inline const int FL_AMULET = 23;
inline const int FL_REGENCOUNTER = 24;
inline const int FL_MORE_DAMAGE = 25;
inline const int FL_DEXCOUNT = 26;
inline const int FL_STRCOUNT = 27;
inline const int FL_BLINDCOUNT = 28;
inline const int FL_CAVE_LEVEL = 29;
inline const int FL_CONFUSE = 30;
inline const int FL_ALTPRO = 31;
inline const int FL_HERO = 32;
inline const int FL_CHARMCOUNT = 33;
inline const int FL_INVISIBILITY = 34;
inline const int FL_CANCELLATION = 35;
inline const int FL_HASTESELF = 36;
inline const int FL_EYE_OF_LARN = 37;
inline const int FL_AGGRAVATE = 38;
inline const int FL_GLOBE = 39;
inline const int FL_TELEPORT_FLAG = 40;
inline const int FL_SLAYING = 41;
inline const int FL_NEGATE_SPIRIT = 42;
inline const int FL_SCAREMONST = 43;
inline const int FL_AWARENESS = 44;
inline const int FL_HOLDMONST = 45;
inline const int FL_TIMESTOP = 46;
inline const int FL_HASTEMONST = 47;
inline const int FL_CUBE_OF_UNDEAD = 48;
inline const int FL_GIANTSTR = 49;
inline const int FL_FIRERESISTANCE = 50;
inline const int FL_BESS_MANN = 51;
inline const int FL_NO_THEFT = 52;
inline const int FL_CPU_TIME = 54;
inline const int FL_BYTES_IN = 55;
inline const int FL_BYTES_OUT = 56;
inline const int FL_MOVESMADE = 57;
inline const int FL_MONSTER_KILLED = 58;
inline const int FL_SPELLS_CAST = 59;
inline const int FL_GREAT_SWORD_OF_DEATH = 60;
inline const int FL_SPIRITPRO = 61;
inline const int FL_UNDEADPRO = 62;
inline const int FL_SHIELD = 63;
inline const int FL_STEALTH = 64;
inline const int FL_ITCHING = 65;
inline const int FL_LAUGHING = 66;
inline const int FL_DRAIN_STRENGTH = 67;
inline const int FL_CLUMSINESS = 68;
inline const int FL_INFEEBLEMENT = 69;
inline const int FL_HALFDAM = 70;
inline const int FL_SEEINVISIBLE = 71;
inline const int FL_SPHERE_CAST = 74;		/* nz if an active sphere of annihilation */
inline const int FL_WALK_THROUGH_WALLS = 75;		/* walk through walls */
inline const int FL_EXTRA_STRENGTH = 76;		/* character strength due to objects or enchantments */
inline const int FL_SCRATCH_SPACE = 77;			/* misc scratch space */
inline const int FL_LIFE_PROTECTION = 78;		/* life protection counter */
inline const int FL_HUNGER = 79;
inline const int FL_HUNGERMAX = 80;
/* fl_wait related */
inline const int FL_WAIT_DURATION = 1000;

/*
    object related
*/
inline const int MAXSCROLL = 28;		/* maximum number of scrolls that are possible */
inline const int MAXSCROLLNAME = 32;
inline const int MAXPOTION = 35;		/* maximum number of potions that are possible */
inline const int MAXPOTIONNAME = 32;
inline const int MAXOBJ = 100;		/* the maximum number of objects   n < MAXOBJ */
/*  defines for the objects in the game     */
inline const int MAXOBJECT = 88;
inline const int OALTAR = 1;
inline const int OTHRONE = 2;
inline const int OORB = 3;
inline const int OPIT = 4;
inline const int OSTAIRSUP = 5;
inline const int OELEVATORUP = 6;
inline const int OFOUNTAIN = 7;
inline const int OSTATUE = 8;
inline const int OTELEPORTER = 9;
inline const int OSKELETON = 10;
inline const int OMIRROR = 11;
inline const int ODNDSTORE = 12;
inline const int OSTAIRSDOWN = 13;
inline const int OELEVATORDOWN = 14;
inline const int OBANK2 = 15;
inline const int OBANK = 16;
inline const int ODEADFOUNTAIN = 17;
inline const int OMAXGOLD = 70;
inline const int OGOLDPILE = 18;
inline const int OOPENDOOR = 19;
inline const int OCLOSEDDOOR = 20;
inline const int OWALL = 21;
inline const int OTRAPARROW = 66;
inline const int OTRAPARROWIV = 67;
inline const int OLARNEYE = 22;
inline const int OPLATE = 23;
inline const int OCHAIN = 24;
inline const int OLEATHER = 25;
inline const int ORING = 60;
inline const int OSTUDLEATHER = 61;
inline const int OSPLINT = 62;
inline const int OPLATEARMOR = 63;
inline const int OSSPLATE = 64;
inline const int OSHIELD = 68;
inline const int OELVENCHAIN = 92;
inline const int OSWORDofSLASHING = 26;
inline const int OHAMMER = 27;
inline const int OSWORD = 28;
inline const int O2SWORD = 29;
inline const int OHSWORD = 59;
inline const int OSPEAR = 30;
inline const int ODAGGER = 31;
inline const int OBATTLEAXE = 57;
inline const int OLONGSWORD = 58;
inline const int OGREATSWORD = 65;
inline const int OVORPAL = 90;
inline const int OSLAYER = 91;
inline const int ORINGOFEXTRA = 32;
inline const int OREGENRING = 33;
inline const int OPROTRING = 34;
inline const int OENERGYRING = 35;
inline const int ODEXRING = 36;
inline const int OSTRRING = 37;
inline const int OCLEVERRING = 38;
inline const int ODAMRING = 39;
inline const int OBELT = 40;
inline const int OSCROLL = 41;
inline const int OPOTION = 42;
inline const int OBOOK = 43;
inline const int OCHEST = 44;
inline const int OAMULET = 45;
inline const int OORBOFDRAGON = 46;
inline const int OSPIRITSCARAB = 47;
inline const int OCUBEofUNDEAD = 48;
inline const int ONOTHEFT = 49;
inline const int ODIAMOND = 50;
inline const int ORUBY = 51;
inline const int OEMERALD = 52;
inline const int OSAPPHIRE = 53;
inline const int OENTRANCE = 54;
inline const int FL_OBJECT_TEMPLE_IN = 55;
inline const int FL_OBJECT_TEMPLE_OUT = 56;
inline const int OHOME = 69;
inline const int OKGOLD = 71;
inline const int ODGOLD = 72;
inline const int OIVDARTRAP = 73;
inline const int ODARTRAP = 74;
inline const int OTRAPDOOR = 75;
inline const int OIVTRAPDOOR = 76;
inline const int OTRADEPOST = 77;
inline const int OIVTELETRAP = 78;
inline const int ODEADTHRONE = 79;
inline const int FL_OBJECT_SPHERE_OF_ANNIHILATION = 80;	/* sphere of annihilation */
inline const int OTHRONE2 = 81;
inline const int OLRS = 82;		/* Larn Revenue Service */
inline const int OCOOKIE = 83;
inline const int OPRAYERBOOK = 84;
inline const int OCANDLE = 85;
inline const int OGHANISTATUE = 86;
inline const int OFIRETUNIC = 87;

inline int (*get_final_char_keyboard)(void);
inline const int ARMORTYPES = 6;

/* maximum number of spheres fl_move_spheres_of_annihilation can handle */
inline const int SPHMAX = 20;

#if 0
	inline const int IDISTNORM = 8;		/* was 17 - dgk */
	inline const int IDISTAGGR = 20;	/* was 40 - dgk */
#endif
inline const int IDISTNORM = 17;	/* was 17 - dgk */
inline const int IDISTAGGR = 40;	/* was 40 - dgk */
inline const int MAX_QUEUE = 100;

//Inline functions for raising and losing health and spells
extern long cdesc[];

inline void FL_RAISEMAXHEALTH(int y) {
	cdesc[FL_HP] += y;
	cdesc[FL_HPMAX] += y;
}
inline void FL_RAISEMAXSPELLS(int y) {
	cdesc[FL_SPELLS] += y;
	cdesc[FL_SPELLMAX] += y;
}
inline void FL_LOSEMAXHEALTH(int y) {
	cdesc[FL_HP] -= y;
	cdesc[FL_HPMAX] -= y;
}
inline void FL_LOSEMAXSPELLS(int y) {
	cdesc[FL_SPELLS] -= y;
	cdesc[FL_SPELLMAX] -= y;
}
