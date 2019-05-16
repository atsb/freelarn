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

#include "FLDungeon.hpp"
#include "../config/FLIncludeData.hpp"
#include "../templates/FLTMath.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../terminal/FLGetChar.hpp"
#include "../core/FLFuncs.hpp"
#include "../core/FLTermCapIO.hpp"
#include "../core/FLScores.hpp"
#include "../core/FLInventory.hpp"
#include "../save/FLSave.hpp"
#include "../lexical/FLTok.hpp"

using namespace std;

extern int (*get_final_char_keyboard)(void);

void
fl_create_the_player_character(void) {
	int i;

	get_final_char_keyboard = ansiterm_getch;
	fl_clear_and_reset_screen();

	cdesc[FL_HPMAX] = cdesc[FL_HP] = 100;
	cdesc[FL_HUNGERMAX] = cdesc[FL_HUNGER] = 100;
	/*  player starts at level one          */
	cdesc[FL_LEVEL] = 1;
	/*  total # spells starts off as 3  */
	cdesc[FL_SPELLMAX] = cdesc[FL_SPELLS] = 3;
	/* start regeneration correctly */
	cdesc[FL_REGENCOUNTER] = 16;
	cdesc[FL_ECOUNTER] = 96;
	cdesc[FL_SHIELD] = cdesc[FL_WEAR] = cdesc[FL_WIELD] = -1;
	for (i = 0; i < MAXINVEN; i++) {
		iven[i] = 0;
	}
	/* he knows protection, magic missile */
	spelknow[0] = spelknow[1] = 1;
	iven[0] = 25;
	iven[1] = 31;
	iven[2] = 0;
	ivenarg[1] = ivenarg[0] = cdesc[FL_WEAR] = 0;
	cdesc[FL_WIELD] = 1;
	player_horizontal_position = TRnd(FL_MAX_HORIZONTAL_POSITION - 2);
	player_vertical_position = TRnd(FL_MAX_VERTICAL_POSITION - 2);
	regen_bottom = 1;
	/* make the attributes, ie str, int, etc. */
	for (i = 0; i < 6; i++) {
		cdesc[i] = MAXINVEN;
	}
	fl_recalculate_armor_class();
	fl_clear_and_reset_screen();
	enter_name();
}
