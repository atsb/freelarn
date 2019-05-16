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


#include "../terminal/FLGetChar.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#if defined WINDOWS
	#include <io.h>
#endif

#if defined NIX || NIX_LOCAL
	#include <unistd.h>
	#include <fcntl.h>
#endif

#include "FLBill.hpp"
#include "../config/FLIncludeData.hpp"
#include "../templates/FLTMath.hpp"
#include "../terminal/FLTerminal.hpp"
#include "FLInventory.hpp"
#include "FLScores.hpp"
#include "FLSystemDependant.hpp"
#include "FLTermCapIO.hpp"

using std::cout;

struct scofmt {		/*  This is the structure for the scoreboard        */
	int score;			/* the score of the player                          */
	int what;			/* the number of the monster that killed player     */
	int level;			/* the level player was on when he died             */
	int order;			/* the relative ordering place of this entry        */
	char who[FL_MAX_PLAYER_NAME_LENGTH];	/* the name of the character                        */
	int sciv[26][2];		/* this is the inventory list of the character      */
};

struct wscofmt {		/* This is the structure for the winning scoreboard */
	int score;			/* the score of the player                          */
	int timeused;			/* the time used in mobuls to win the game          */
	long taxes;			/* taxes he owes to LRS                             */
	int order;			/* the relative ordering place of this entry        */
	int hasmail;			/* 1 if mail is to be read, 0 otherwise */
	char who[FL_MAX_PLAYER_NAME_LENGTH];	/* the name of the character                        */
};

static int sortboard(void);
static void newscore(int, char *, int, int);
static void new1sub(int, int, char *, int);
static void new2sub(int, int, char *, int);
static int readboard(void);
static int writeboard(void);
static struct scofmt sco[FL_TOTAL_NAMES_ON_SCORE_BOARD];
static struct wscofmt winr[FL_TOTAL_NAMES_ON_SCORE_BOARD];
extern bool FL_WIZARD_MODE_ENABLED;
extern int (*get_final_char_keyboard)(void);

const char *whydead[31] = {
	"",
	"",
	"self - annihilated",
	"being shot by an arrow",
	"being hit by a dart",
	"falling into a pit",
	"falling into a bottomless pit",
	"a winner", //263
	"getting trapped in solid rock", //264
	"starving to death", //265
	"",
	"",
	"",
	"failed",
	"being erased by a wayward finger",
	"falling through a bottomless trap door",
	"falling through a trap door",
	"drinking poisonous water",
	"being fried by an electric shock",
	"burning to death in the Temple of Larn", //275
	"",
	"a revolting demon",
	"getting hit with own magic",
	"getting demolished by an unseen attacker",
	"falling into the dreadful sleep",
	"an exploding chest",
	"",
	"being annihilated in a sphere",
	"",
	"",
	""
};

/*
    readboard()     Function to read in the scoreboard into a static buffer

    returns -1 if unable to read in the scoreboard, returns 0 if all is OK
*/
static int
readboard(void) {
	FILE *pFile;
	int b;
	pFile = fopen(scorefile, "rb");
	if (pFile == NULL) {
		cout << "ERROR: scorefile is not readable \n";
		fl_output_buffer_flush();
		return (-1);
	}
	b = fread(sco, sizeof(struct scofmt) * FL_TOTAL_NAMES_ON_SCORE_BOARD, 1, pFile);
	if (b != 1) {
		cout << "ERROR: Loosers scoreboard is not readable\n";
		fclose(pFile);
		return (-1);
	}
	b = fread(winr, sizeof(struct wscofmt) * FL_TOTAL_NAMES_ON_SCORE_BOARD, 1, pFile);
	if (b != 1) {
		cout << "ERROR: Winners scoreboard is not readable\n";
		fclose(pFile);
		return (-1);
	}
	fclose(pFile);
	return 0;
}

/*
    writeboard()    Function to write the scoreboard from readboard()'s buffer

    returns -1 if unable to write the scoreboard, returns 0 if all is OK
*/
static int
writeboard(void) {
	FILE *pFile;
	int b;
	enable_scroll = -1;
	pFile = fopen(scorefile, "wb");
	if (pFile == NULL) {
		cout << "ERROR: scorefile is not writable \n";
		fl_output_buffer_flush();
		return (-1);
	}
	b = fwrite(sco, sizeof(struct scofmt) * FL_TOTAL_NAMES_ON_SCORE_BOARD, 1, pFile);
	if (b != 1) {
		cout << "ERROR: Loosers scoreboard is not writable\n";
		fclose(pFile);
		return (-1);
	}
	b = fwrite(winr, sizeof(struct wscofmt) * FL_TOTAL_NAMES_ON_SCORE_BOARD, 1, pFile);
	if (b != 1) {
		cout << "ERROR: Winners scoreboard is not writable\n";
		fclose(pFile);
		return (-1);
	}
	fclose(pFile);
	return 0;
}

/*
    makeboard()         Function to create a new scoreboard (wipe out old one)

    returns -1 if unable to write the scoreboard, returns 0 if all is OK
*/
int
makeboard(void) {
	int i;
	for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++) {
		winr[i].taxes = winr[i].score = sco[i].score = 0;
		winr[i].order = sco[i].order = (short) i;
	}
	if (writeboard()) {
		cout << "ERROR: unable to write a new scoreboard\n";
		return (-1);
	}
	/*  Why bother redefining a function?  Just use standard *NIX functions
		and be done with it. Windows won't even need to set permissions anyway
		so no need for 'cross platform' here. Sheesh.. ~Gibbon
	*/
	#if defined NIX || NIX_LOCAL
	chmod(scorefile, 0666);
	#endif
	return (0);
}

void
checkmail(void) {
	int i, gold, taxes;
	if (readboard() < 0) {
		return;  /* can't find scoreboard */
	}
	/* search through winners scoreboard */
	for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++)   /* search through winners scoreboard */
		if (strcmp(winr[i].who, logname) == 0 && winr[i].score > 0 && winr[i].hasmail) {
			winr[i].hasmail = 0;
			gold = taxes = winr[i].taxes;
			writeboard();
			/*  Intuit the amount of gold -- should have changed
			    the score file, but ...  TAXRATE is an fraction.
			*/
			while ((gold * TAXRATE) < taxes) {
				gold += taxes;
			}
			larn_read_junk_mail(gold);
		}
}

/*
    int paytaxes(x)         Function to pay taxes if any are due

    Enter with the amount (in gp) to pay on the taxes.
    Returns amount actually paid.
*/

int
paytaxes(int x) {
	int i, amt;
	if (x < 0) {
		return (0L);
	}
	if (readboard() < 0) {
		return (0L);
	}
	/* search for a winning entry for the player */
	for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++)
		if (strcmp(winr[i].who, logname) == 0)
			if (winr[i].score > 0) {
				amt = winr[i].taxes;
				if (amt > x)
					amt = x;
				winr[i].taxes -= amt;
				writeboard();
				return amt;
			}
	return 0;
}

/*
    showscores()        Function to show the scoreboard on the terminal

    Returns nothing of value
*/

void
showscores(int scoreflag) {
	int count, i, j;
	fl_invert_color_space("Player Score Data");
	fl_output_single_byte_to_output_buffer('\n');
	count = 0;
	for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++)
		for (j = 0; j < FL_TOTAL_NAMES_ON_SCORE_BOARD; j++)
			if (winr[j].order == i) {
				if (winr[j].score) {
					count++;
					lprintf("%-10d      %5d Mobuls     %s\n", winr[j].score, winr[j].timeused, winr[j].who);
				}
				break;
			}
	for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++)
		for (j = 0; j < FL_TOTAL_NAMES_ON_SCORE_BOARD; j++)
			if (sco[j].order == i) {
				if (sco[j].score) {
					count++;
					lprintf("%-10d  %s", sco[j].score, sco[j].who);
					if (sco[j].what < 256) {
						const char *p = monster[sco[j].what].name;
						lprintf(" killed by %s %s", ((*p == 'a' || *p == 'e' || *p == 'i' || *p == 'o' || *p == 'u') ? "an" : "a"), p);
					} else
						lprintf(" %s", whydead[sco[j].what - 256]);
					lprintf(" on level %s\n", levelname[sco[j].level]);
					if (scoreflag) {
						int k, n;
						for (k = 1; k < MAXOBJECT; k++)
							for (n = 0; n < MAXINVEN; n++)
								if (k == sco[j].sciv[n][0]) {
									lprintf("%c) %s", 'a' + n, objectname[sco[j].sciv[n][0]]);
									switch (sco[j].sciv[n][0]) {
										case OPOTION:
											lprintf("%s", potionname[sco[j].sciv[n][1]]);
											break;
										case OSCROLL:
											lprintf("%s", scrollname[sco[j].sciv[n][1]]);
											break;
										case ODIAMOND:
										case ORUBY:
										case OEMERALD:
										case OSAPPHIRE:
										case OCHEST:
										case OBOOK:
										case OCOOKIE:
										case OLARNEYE:
										case OSPIRITSCARAB:
										case OCUBEofUNDEAD:
										case ONOTHEFT:
											break;
										default:
											if (sco[j].sciv[n][1] > 0)
												lprintf(" +%d", sco[j].sciv[n][1]);
											else if (sco[j].sciv[n][1] < 0)
												lprintf(" %d", sco[j].sciv[n][1]);
											break;
									}
									fl_output_single_byte_to_output_buffer('\n');
								}
					}
				}
				break;
			}
}

/*
    sortboard()     Function to sort the scoreboard

    Returns 0 if no sorting done, else returns 1
*/
static int
sortboard(void) {
	int i, pos, j = 0, jdat;
	for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++)
		sco[i].order = winr[i].order = -1;
	pos = 0;
	while (pos < FL_TOTAL_NAMES_ON_SCORE_BOARD) {
		jdat = 0;
		for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++)
			if ((sco[i].order < 0) && (sco[i].score >= jdat)) {
				j = i;
				jdat = sco[i].score;
			}
		sco[j].order = pos++;
	}
	pos = 0;
	while (pos < FL_TOTAL_NAMES_ON_SCORE_BOARD) {
		jdat = 0;
		for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++)
			if ((winr[i].order < 0) && (winr[i].score >= jdat)) {
				j = i;
				jdat = winr[i].score;
			}
		winr[j].order = pos++;
	}
	return 1;
}

/*
    newscore(score, whoo, whyded, winner)   Function to add entry to scoreboard
       int score, winner, whyded;
       char *whoo;

    Enter with the total score in gp in score,  players name in whoo,
       fl_player_death() reason # in whyded, and TRUE/0 in winner if a winner
    ex.     newscore(1000, "player 1", 32, 0);
*/
static void
newscore(int score, char *whoo, int whyded, int winner) {
	int i, taxes;
	/* if a winner then delete all non-winning scores */
	if (winner) {
		for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++)
			if (strcmp(sco[i].who, logname) == 0)
				sco[i].score = 0;
		taxes = score * TAXRATE;
		score += 100000; /* bonus for winning */
		/* if he has a slot on the winning scoreboard update it if greater score */
		for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++)
			if (strcmp(winr[i].who, logname) == 0) {
				new1sub(score, i, whoo, taxes);
				return;
			}
		/* he had no entry. look for last entry and see if he has a greater score */
		for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++)
			if (winr[i].order == FL_TOTAL_NAMES_ON_SCORE_BOARD - 1) {
				new1sub(score, i, whoo, taxes);
				return;
			}
	} else { /* for not winning scoreboard */
		/* if he has a slot on the scoreboard update it if greater score */
		for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++)
			if (strcmp(sco[i].who, logname) == 0) {
				new2sub(score, i, whoo, whyded);
				return;
			}
		/* he had no entry. look for last entry and see if he has a greater score */
		for (i = 0; i < FL_TOTAL_NAMES_ON_SCORE_BOARD; i++)
			if (sco[i].order == FL_TOTAL_NAMES_ON_SCORE_BOARD - 1) {
				new2sub(score, i, whoo, whyded);
				return;
			}
	}
}

/*
    new1sub(score,i,whoo,taxes)       Subroutine to put player into a
       int score,i,whyded,taxes;         winning scoreboard entry if his score
       char *whoo;                       is high enough

    Enter with the total score in gp in score,  players name in whoo,
       fl_player_death() reason # in whyded, and TRUE/0 in winner if a winner
       slot in scoreboard in i, and the tax bill in taxes.
    Returns nothing of value
*/
static void
new1sub(int score, int i, char *whoo, int taxes) {
	struct wscofmt *p;
	p = &winr[i];
	p->taxes += taxes;
	if ((score >= p->score)) {
		strcpy(p->who, whoo);
		p->score = score;
		p->timeused = gtime / 100;
		p->hasmail = 1;
	}
}

/*
    new2sub(score,i,whoo,whyded)          Subroutine to put player into a
       int score,i,whyded,taxes;         non-winning scoreboard entry if his
       char *whoo;                       score is high enough

    Enter with the total score in gp in score,  players name in whoo,
       fl_player_death() reason # in whyded, and slot in scoreboard in i.
    Returns nothing of value
*/
static void
new2sub(int score, int i, char *whoo, int whyded) {
	int j;
	struct scofmt *p;
	p = &sco[i];
	if ((score >= p->score)) {
		strcpy(p->who, whoo);
		p->score = score;
		p->what = whyded;
		p->level = level;
		for (j = 0; j < 26; j++) {
			p->sciv[j][0] = iven[j];
			p->sciv[j][1] = ivenarg[j];
		}
	}
}

/*
    fl_player_death(x)     Subroutine to record who played larn, and what the score was
       int x;

    if x < 0 then don't show scores
    fl_player_death() never returns! (unless cdesc[FL_LIFE_PROTECTION] and a reincarnatable death!)

       < 256   killed by the monster number
       256     quit
       257     suspended
       258     self - annihilated
       259     shot by an arrow
       260     hit by a dart
       261     fell into a pit
       262     fell into a bottomless pit
       263     a winner
       264     trapped in solid rock
       265     killed by a missing save file
       266     killed by an old save file
       267     caught by the greedy cheater checker trap
       268     killed by a protected save file
       269     killed his family and killed himself
       270     erased by a wayward finger
       271     fell through a bottomless trap door
       272     fell through a trap door
       273     drank some poisonous water
       274     fried by an electric shock
       275     killed by the Temple of Larn
       276     killed by a stupid act of frustration
       277     attacked by a revolting demon
       278     hit by his own magic
       279     demolished by an unseen attacker
       280     fell into the dreadful sleep
       281     killed by an exploding chest
       282     killed by a missing maze data file
       283     killed by a sphere of annihilation
       284     died a post mortem death
       285     malloc() failure
       300     quick quit -- don't put on scoreboard
*/

void
fl_player_death(int x) {
	int win = 0;
	if (cdesc[FL_LIFE_PROTECTION] > 0) {
		switch (x) {
			case 262:
			case 263:
			case 269:
			case 271:
				goto cantprotect;
		};
		--cdesc[FL_LIFE_PROTECTION];
		cdesc[FL_HP] = cdesc[FL_HPMAX];
		--cdesc[FL_CONSTITUTION];
		fl_termcap_cursor_position(1, 24);
		fl_display_message("\nYou feel wiiieeeeerrrrrd all over! ");
		fl_output_buffer_flush();
		fl_wait(3000);
		return;
	}
cantprotect:
	fl_wait(3000);
	fl_termcap_cursor_position(1, 24);
	fl_display_message("\nPress any key to continue. ");
	ttgetch();
	fl_output_buffer_flush();
	fl_clear_and_reset_screen();
	enable_scroll = 0;
	cdesc[FL_GOLD] += cdesc[FL_BANK_ACCOUNT];
	cdesc[FL_BANK_ACCOUNT] = 0;
	newscore(cdesc[FL_GOLD], logname, x, win);
	if (x == 263)
		win = 1;
	if (FL_WIZARD_MODE_ENABLED == false) {
		if (sortboard()) {
			writeboard();
		}
	}
	fl_output_buffer_flush();
	fl_clear_and_reset_screen();
	enable_scroll = 0;
	showscores(x);     /* if we updated the scoreboard */
	fl_termcap_cursor_position(1, 24);
	fl_display_message("\nPress any key to exit. ");
	get_final_char_keyboard = ansiterm_getch;
	ttgetch();
	fl_clear_terminal();
	exit(EXIT_SUCCESS);
}
