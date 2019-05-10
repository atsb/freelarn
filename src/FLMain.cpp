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

#include "templates/FLTMath.hpp"
#include "terminal/FLTerminal.hpp"
#include "terminal/FLGetChar.hpp"
#include "lexical/FLTok.hpp"
#include "dungeon/FLDungeon.hpp"
#include "core/FLHelp.hpp"
#include "core/FLInventory.hpp"
#include "core/FLTermCapIO.hpp"
#include "config/FLIncludeData.hpp"
#include "player/FLPlayerActions.hpp"
#include "player/FLRegenerate.hpp"
#include "core/FLScores.hpp"
#include "save/FLSave.hpp"
#include "player/FLHunger.hpp"
#include "core/FLNewGame.hpp"
#include "core/FLFuncs.hpp"

using std::cout;

extern char viewflag;
int dropflag = 0;		/* if 1 then don't fl_look_for_an_object_and_give_options() next round */
int random_monster_creation = 80;			/*  random monster creation counter     */
int nomove = 0;			/* if (nomove) then don't count next iteration as a move */
signed int save_mode = 0;	/* 1 if doing a save game */
int restorflag = 0;

/*
************
    MAIN PROGRAM
************
*/
int main() {
	FILE *pFile;
	//Classes
	Load load;
	FLHunger FLHunger;
	FLGame FLGame;
	/*
	    first task is to identify the player
	*/
	/*init curses ~Gibbon */
	fl_initialize_termcap_terminal_handling();			/* setup the terminal (find out what type) for termcap */
	get_final_char_keyboard = ansiterm_getch;
	/*
	    second task is to prepare the pathnames the player will need
	*/
	/*  Set up the input and output buffers.
	*/
	lpbuf = (char *) operator new ((5 * FL_OUTPUT_BUFFER) >>
	                               2);	/* output buffer */
	inbuffer = (char *) operator new ((5 * FL_INPUT_BUFFER) >>
	                                  2);	/* output buffer */
	if ((lpbuf == 0) || (inbuffer == 0)) {
		fl_player_death(-285);     /* operator failure */
	}
	strncpy(scorefile, FL_SCORE_FILE_NAME, 50);	/* the larn scoreboard filename */
	strncpy(savefilename, FL_SAVE_FILE_NAME, 50);
	/*
	    now make scoreboard if it is not there (don't clear)
	*/
	pFile = fopen(scorefile, "r");
	if (pFile == 0) {	/* not there */
		makeboard();
	} else {
		fclose(pFile);
	}
	/*
	    He really wants to play, so malloc the memory for the dungeon.
	*/
	const char *cell = (const char*) operator new ((sizeof * cell) * (FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL) *
	                   FL_MAX_HORIZONTAL_POSITION * FL_MAX_VERTICAL_POSITION);
	if (cell == NULL) {
		/* malloc failure */
		fl_player_death(-285);
	}
	fl_create_new_file((char *) 0);
	FLGame.NewGame();			/*  set the initial clock  */
	pFile = fopen(savefilename, "r");
	if (pFile != 0) {	/* restore game if need to */
		fclose(pFile);
		fl_clear_and_reset_screen();
		restorflag = 1;
		hitflag = 1;
		load.restoregame(savefilename);	/* restore last game    */
		remove(savefilename);
	}
	fl_setup_terminal();		/*  setup the terminal special mode             */
	if (cdesc[FL_HP] == 0) {	/* create new game */
		predostuff =
		    1;		/* tell signals that we are in the welcome screen */
		welcome();		/* welcome the player to the game */
		fl_create_the_player_character();		/*  make the character that will play           */
		fl_generate_a_new_dungeon_level(0);		/*  make the dungeon                            */
		/*  Display their mail if they've just won the previous game
		*/
		checkmail();
	}
	fl_output_single_byte_to_output_buffer(T_INIT);		/* Reinit the screen because of welcome and check mail
				  having embedded escape sequences.*/
	drawscreen();		/*  show the initial dungeon */
	/* tell the trap functions that they must do a showplayer() from here on */
	predostuff = 2;
	y_larn_rep = hit2flag = 0;
	/*
	    init previous player position to be current position, so we don't
	    reveal any stuff on the screen prematurely.
	*/
	oldx = player_horizontal_position;
	oldy = player_vertical_position;
	/* gtime = -1; */
	/*  MAINLOOP
	    find objects, move stuff, get commands, regenerate
	*/
	for (;;) {
		if (dropflag == 0) {
			/*  see if there is an object here.

			    If in prompt mode, identify and prompt; else
			    identify, never prompt.
			*/
			fl_look_for_an_object_and_give_options(1, 0, 0);
		} else {
			dropflag = 0;		/* don't show it just dropped an object */
		}
		/*  handle global activity
		    update game time, move spheres, move walls, move monsters
		    all the stuff affected by FL_TIMESTOP and FL_HASTESELF
		*/
		if (cdesc[FL_TIMESTOP] <= 0)
			if (cdesc[FL_HASTESELF] == 0 || (cdesc[FL_HASTESELF] & 1) == 0) {
				gtime++;
				fl_move_spheres_of_annihilation();
				if (hitflag == 0) {
					if (cdesc[FL_HASTEMONST]) {
						fl_move_a_monster();
					}
					fl_move_a_monster();
				}
			}
		/*  show stuff around the player
		*/
		if (viewflag == 0) {
			showcell(player_horizontal_position, player_vertical_position);
		} else {
			viewflag = 0;
		}
		if (hit3flag)
		#if defined WINDOWS
			while (kbhit()) {
				ansiterm_getch();
			}
		#endif
		#if defined NIX || NIX_LOCAL
		fflush(NULL);
		#endif
		hitflag = hit3flag = 0;
		bot_linex();		/* update bottom line */
		/*  get commands and make moves
		*/
		nomove = 1;
		while (nomove) {
			if (hit3flag)
			#if defined WINDOWS
				while (kbhit()) {
					ansiterm_getch();
				}
			#endif
			#if defined NIX || NIX_LOCAL
			fflush(NULL);
			#endif
			nomove = 0;
			parse();
		}
		fl_regen_hp_and_spells();			/*  regenerate hp and spells            */
		FLHunger.HungerLose();
		if (cdesc[FL_TIMESTOP] == 0)
			if (--random_monster_creation <= 0) {
				random_monster_creation = 120 - (level << 2);
				fl_fill_dungeon_with_monsters(fl_create_monster(level));
			}
	}
}