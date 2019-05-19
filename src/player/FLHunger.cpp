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

#include "../config/FLIncludeData.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../core/FLTermCapIO.hpp"
#include "../templates/FLTHunger.hpp"
#include "FLHunger.hpp"
#include "../core/FLScores.hpp"

void FLHunger::HungerLose(void) {
	if (THungerTime(69000)) {
		cdesc[FL_HUNGER] -= 20;
	}
	if (THungerTime(60000)) {
		cdesc[FL_HUNGER] -= 20;
	}
	if (THungerTime(55000)) {
		cdesc[FL_HUNGER] -= 20;
	}
	if (THungerTime(49000)) {
		cdesc[FL_HUNGER] -= 20;
	}
	if (THungerTime(45000)) {
		cdesc[FL_HUNGER] -= 40;
	}
	if (THungerTime(25000)) {
		cdesc[FL_HUNGER] -= 40;
	}
	if (THungerTime(10000)) {
		cdesc[FL_HUNGER] -= 40;
	}
	if (THungerTime(5000)) {
		cdesc[FL_HUNGER] -= 60;
	}
	if (THungerTime(1000)) {
		cdesc[FL_HUNGER] -= 60;
	}
	if (cdesc[FL_HUNGER] == 0) {
		cdesc[FL_HP] = 0;
		fl_display_message("You have died of starvation!");
		fl_player_death(-265); //starved to death :(
	}
}
