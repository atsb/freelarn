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

const int HAVESEEN = 1;
const int KNOWHERE = 2;
const int KNOWALL = HAVESEEN | KNOWHERE;
const int FL_TOTAL_MOBUL_LIMIT = 70000;

//Tax rate for the LRS
const int TAXRATE = 1/20;

//Termcap stuff
const int FL_MAX_HORIZONTAL_POSITION = 67;
const int FL_MAX_VERTICAL_POSITION = 17;
const int ST_START = 1;
const int ST_END = 2;
const int FL_CLEAR = 5;
const int CL_LINE = 6;
const int T_INIT = 7;
const int T_END = 8;
const int CL_DOWN = 14;
const int FL_CURSOR = 15;

const int FL_MAX_CAVE_OF_LARN_LEVEL = 11;
const int FL_MAX_TEMPLE_OF_LARN_LEVEL = 3;
const int FL_MAX_PATH_NAME_LENGTH = 80;

/* this is the number of people on a scoreboard max */
const int FL_TOTAL_NAMES_ON_SCORE_BOARD = 4;
/* maximum player level allowed */
const int FL_MAX_PLAYER_LEVEL = 100;
/* maximum number of spells in existance */
const int FL_MAX_SPELLS_IN_GAME = 38;
/* size of the output buffer */
const int FL_OUTPUT_BUFFER = 4096;
/* size of the input buffer */
const int FL_INPUT_BUFFER = 4096;
/* max size of the player's name */
const int FL_MAX_PLAYER_NAME_LENGTH = 20;
/* max size of the savefile path */
const int FL_SAVE_FILE_NAME_SIZE = 128;
const int FL_STRING_BUFFER_SIZE = 256;

/* maximum # monsters in the dungeon */
const int FL_MAX_MONSTERS_IN_GAME = 56;
/* Maximum number of mazes */
const int FL_MAX_MAZES_IN_GAME = 2;

/*  defines for the monsters as objects  */
const int BAT = 1;
const int GNOME = 2;
const int HOBGOBLIN = 3;
const int JACKAL = 4;
const int KOBOLD = 5;
const int ORC = 6;
const int SNAKE = 7;
const int CENTIPEDE = 8;
const int JACULI = 9;
const int TROGLODYTE = 10;
const int ANT = 11;
const int EYE = 12;
const int LEPRECHAUN = 13;
const int NYMPH = 14;
const int QUASIT = 15;
const int RAGANA = 16;
const int ZOMBIE = 17;
const int ASSASSINBUG = 18;
const int BUGBEAR = 19;
const int HELLHOUND = 20;
const int ICELIZARD = 21;
const int CENTAUR = 22;
const int TROLL = 23;
const int YETI = 24;
const int WHITEDRAGON = 25;
const int ELF = 26;
const int GYTRASH = 27;
const int METAMORPH = 28;
const int VORTEX = 29;
const int ZILLER = 30;
const int VIOLETFUNGI = 31;
const int WRAITH = 32;
const int FORVALAKA = 33;
const int LAMANOBE = 34;
const int OSEQUIP = 35;
const int ROTHE = 36;
const int XORN = 37;
const int VAMPIRE = 38;
const int INVISIBLEDIWATA = 39;
const int POLTERGEIST = 40;
const int DISENCHANTRESS = 41;
const int SHAMBLINGMOUND = 42;
const int YELLOWMOLD = 43;
const int UNKTEHI = 44;
const int GNOMEKING = 45;
const int MIMIC = 46;
const int WATERLORD = 47;
const int BRONZEDRAGON = 48;
const int GREENDRAGON = 49;
const int PURPLEWORM = 50;
const int XVART = 51;
const int SPIRITNAGA = 52;
const int SILVERDRAGON = 53;
const int PLATINUMDRAGON = 54;
const int GREENURCHIN = 55;
const int REDDRAGON = 56;
const int DEMONLORD = 57;
const int DEMONPRINCE = 64;

/*
    defines for the character attribute array   cdesc[]
*/
const int FL_STRENGTH = 0;		/* characters physical strength not due to objects */
const int FL_INTELLIGENCE = 1;
const int FL_WISDOM = 2;
const int FL_CONSTITUTION = 3;
const int FL_DEXTERITY = 4;
const int FL_CHARISMA = 5;
const int FL_HPMAX = 6;
const int FL_HP = 7;
const int FL_GOLD = 8;
const int FL_EXPERIENCE = 9;
const int FL_LEVEL = 10;
const int FL_REGEN = 11;
const int FL_WCLASS = 12;
const int FL_AC = 13;
const int FL_BANK_ACCOUNT = 14;
const int FL_SPELLMAX = 15;
const int FL_SPELLS = 16;
const int FL_ENERGY = 17;
const int FL_ECOUNTER = 18;
const int FL_MOREDEFENSES = 19;
const int FL_WEAR = 20;
const int FL_PROTECTIONTIME = 21;
const int FL_WIELD = 22;
const int FL_AMULET = 23;
const int FL_REGENCOUNTER = 24;
const int FL_MORE_DAMAGE = 25;
const int FL_DEXCOUNT = 26;
const int FL_STRCOUNT = 27;
const int FL_BLINDCOUNT = 28;
const int FL_CAVE_LEVEL = 29;
const int FL_CONFUSE = 30;
const int FL_ALTPRO = 31;
const int FL_HERO = 32;
const int FL_CHARMCOUNT = 33;
const int FL_INVISIBILITY = 34;
const int FL_CANCELLATION = 35;
const int FL_HASTESELF = 36;
const int FL_EYE_OF_LARN = 37;
const int FL_AGGRAVATE = 38;
const int FL_GLOBE = 39;
const int FL_TELEPORT_FLAG = 40;
const int FL_SLAYING = 41;
const int FL_NEGATE_SPIRIT = 42;
const int FL_SCAREMONST = 43;
const int FL_AWARENESS = 44;
const int FL_HOLDMONST = 45;
const int FL_TIMESTOP = 46;
const int FL_HASTEMONST = 47;
const int FL_CUBE_OF_UNDEAD = 48;
const int FL_GIANTSTR = 49;
const int FL_FIRERESISTANCE = 50;
const int FL_BESS_MANN = 51;
const int FL_NO_THEFT = 52;
const int FL_CPU_TIME = 54;
const int FL_BYTES_IN = 55;
const int FL_BYTES_OUT = 56;
const int FL_MOVESMADE = 57;
const int FL_MONSTER_KILLED = 58;
const int FL_SPELLS_CAST = 59;
const int FL_GREAT_SWORD_OF_DEATH = 60;
const int FL_SPIRITPRO = 61;
const int FL_UNDEADPRO = 62;
const int FL_SHIELD = 63;
const int FL_STEALTH = 64;
const int FL_ITCHING = 65;
const int FL_LAUGHING = 66;
const int FL_DRAIN_STRENGTH = 67;
const int FL_CLUMSINESS = 68;
const int FL_INFEEBLEMENT = 69;
const int FL_HALFDAM = 70;
const int FL_SEEINVISIBLE = 71;
const int FL_SPHERE_CAST = 74;		/* nz if an active sphere of annihilation */
const int FL_WALK_THROUGH_WALLS = 75;		/* walk through walls */
const int FL_EXTRA_STRENGTH = 76;		/* character strength due to objects or enchantments */
const int FL_SCRATCH_SPACE = 77;			/* misc scratch space */
const int FL_LIFE_PROTECTION = 78;		/* life protection counter */
const int FL_HUNGER = 79;
const int FL_HUNGERMAX = 80;
/* fl_wait related */
const int FL_WAIT_DURATION = 1000;

/*
    object related
*/
const int MAXSCROLL = 28;		/* maximum number of scrolls that are possible */
const int MAXSCROLLNAME = 32;
const int MAXPOTION = 35;		/* maximum number of potions that are possible */
const int MAXPOTIONNAME = 32;
const int MAXOBJ = 100;		/* the maximum number of objects   n < MAXOBJ */
/*  defines for the objects in the game     */
const int MAXOBJECT = 88;
const int OALTAR = 1;
const int OTHRONE = 2;
const int OORB = 3;
const int OPIT = 4;
const int OSTAIRSUP = 5;
const int OELEVATORUP = 6;
const int OFOUNTAIN = 7;
const int OSTATUE = 8;
const int OTELEPORTER = 9;
const int OSKELETON = 10;
const int OMIRROR = 11;
const int ODNDSTORE = 12;
const int OSTAIRSDOWN = 13;
const int OELEVATORDOWN = 14;
const int OBANK2 = 15;
const int OBANK = 16;
const int ODEADFOUNTAIN = 17;
const int OMAXGOLD = 70;
const int OGOLDPILE = 18;
const int OOPENDOOR = 19;
const int OCLOSEDDOOR = 20;
const int OWALL = 21;
const int OTRAPARROW = 66;
const int OTRAPARROWIV = 67;
const int OLARNEYE = 22;
const int OPLATE = 23;
const int OCHAIN = 24;
const int OLEATHER = 25;
const int ORING = 60;
const int OSTUDLEATHER = 61;
const int OSPLINT = 62;
const int OPLATEARMOR = 63;
const int OSSPLATE = 64;
const int OSHIELD = 68;
const int OELVENCHAIN = 92;
const int OSWORDofSLASHING = 26;
const int OHAMMER = 27;
const int OSWORD = 28;
const int O2SWORD = 29;
const int OHSWORD = 59;
const int OSPEAR = 30;
const int ODAGGER = 31;
const int OBATTLEAXE = 57;
const int OLONGSWORD = 58;
const int OGREATSWORD = 65;
const int OVORPAL = 90;
const int OSLAYER = 91;
const int ORINGOFEXTRA = 32;
const int OREGENRING = 33;
const int OPROTRING = 34;
const int OENERGYRING = 35;
const int ODEXRING = 36;
const int OSTRRING = 37;
const int OCLEVERRING = 38;
const int ODAMRING = 39;
const int OBELT = 40;
const int OSCROLL = 41;
const int OPOTION = 42;
const int OBOOK = 43;
const int OCHEST = 44;
const int OAMULET = 45;
const int OORBOFDRAGON = 46;
const int OSPIRITSCARAB = 47;
const int OCUBEofUNDEAD = 48;
const int ONOTHEFT = 49;
const int ODIAMOND = 50;
const int ORUBY = 51;
const int OEMERALD = 52;
const int OSAPPHIRE = 53;
const int OENTRANCE = 54;
const int FL_OBJECT_TEMPLE_IN = 55;
const int FL_OBJECT_TEMPLE_OUT = 56;
const int OHOME = 69;
const int OKGOLD = 71;
const int ODGOLD = 72;
const int OIVDARTRAP = 73;
const int ODARTRAP = 74;
const int OTRAPDOOR = 75;
const int OIVTRAPDOOR = 76;
const int OTRADEPOST = 77;
const int OIVTELETRAP = 78;
const int ODEADTHRONE = 79;
const int FL_OBJECT_SPHERE_OF_ANNIHILATION = 80;	/* sphere of annihilation */
const int OTHRONE2 = 81;
const int OLRS = 82;		/* Larn Revenue Service */
const int OCOOKIE = 83;
const int OPRAYERBOOK = 84;
const int OCANDLE = 85;
const int OGHANISTATUE = 86;
const int OFIRETUNIC = 87;

const int ARMORTYPES = 6;

/* maximum number of spheres fl_move_spheres_of_annihilation can handle */
const int SPHMAX = 20;

const int IDISTNORM = 17;	/* was 17 - dgk */
const int IDISTAGGR = 40;	/* was 40 - dgk */
const int MAX_QUEUE = 100;

//Templates for raising and losing health and spells,
//kept here for now as it uses other definitions. ~Gibbon
extern long cdesc[];
extern int object_identification[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION];
extern int player_horizontal_position;
extern int player_vertical_position;
extern int been_here_before[FL_MAX_HORIZONTAL_POSITION][FL_MAX_VERTICAL_POSITION];

template<typename T>
T TRaiseMaxHealth(const T& y) {
	return cdesc[FL_HP] = cdesc[FL_HPMAX] += y;
}
template<typename T>
T TRaiseMaxSpells(const T& y) {
	return cdesc[FL_SPELLS] = cdesc[FL_SPELLMAX] += y;
}
template<typename T>
T TLowerMaxHealth(const T& y) {
	return cdesc[FL_HP] = cdesc[FL_HPMAX] -= y;
}
template<typename T>
T TLowerMaxSpells(const T& y) {
	return cdesc[FL_SPELLS] = cdesc[FL_SPELLMAX] -= y;
}
//The call is really TForgetData(0); but in this case the '0' means to set it to 0 to forget it. ~Gibbon
template<typename T>
T TForgetData(const T& x) {
	return object_identification[player_horizontal_position][player_vertical_position]
			= been_here_before[player_horizontal_position][player_vertical_position] = x;
}