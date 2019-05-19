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
#include "../templates/FLTMath.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../core/FLFuncs.hpp"
#include "../core/FLTermCapIO.hpp"
#include "../core/FLScores.hpp"
#include "../save/FLSave.hpp"
#include "FLDungeon.hpp"

/*
    subroutine to put monsters into an empty room without walls or other
    monsters
*/
int
fl_fill_dungeon_with_monsters(int what) {
	int x, y, trys;
	for (trys = 5; trys > 0; --trys) {	/* max # of creation attempts */
		x = TRnd(FL_MAX_HORIZONTAL_POSITION - 2);
		y = TRnd(FL_MAX_VERTICAL_POSITION - 2);
		if ((object_identification[x][y] == 0) && (monster_identification[x][y] == 0)
		        && ((player_horizontal_position != x) || (player_vertical_position != y))) {
			monster_identification[x][y] = what;
			been_here_before[x][y] &= ~KNOWHERE;
			monster_hit_points[x][y] = monster[what].hitpoints;
			return (0);
		}
	}
	return -1;			/* creation failure */
}
