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

#include "FLFuncs.hpp"
#include "FLSystemDependant.hpp"
#include "FLScores.hpp"
#include "../templates/FLTMath.hpp"
#include "../config/FLIncludeData.hpp"
#include "../terminal/FLTerminal.hpp"
#include "FLTermCapIO.hpp"

void FLCoreFuncs::DecreasePHealth(int x) {
	if ((cdesc[FL_HP] -= x) <= 0) {
		fl_display_message("\nYou have died!");
		fl_wait(FL_WAIT_DURATION);
		fl_player_death(lastnum);
	}
}