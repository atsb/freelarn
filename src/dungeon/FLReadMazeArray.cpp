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
#include "../core/FLFuncs.hpp"
#include "../core/FLTermCapIO.hpp"
#include "../mazes/FLMaze.hpp"
#include "FLDungeon.hpp"

int
fl_maze_from_char(int dungeon_level) {
	int i, j, it, mit, arg, marg;
	const char *fl_larn_mazes = FL_Larn_Maze_Chars[TRund(FL_MAX_MAZES_IN_GAME)];
	for (i = 0; i < FL_MAX_VERTICAL_POSITION; i++)
		for (j = 0; j < FL_MAX_HORIZONTAL_POSITION; j++) {
			it = mit = arg = marg = 0;
			switch (*fl_larn_mazes++) {
				case '#': /* wall */
					it = OWALL;
					break;
				case 'D': /* door */
					it = OCLOSEDDOOR;
					arg = TRnd(30);
					break;
				case '~': /* eye of larn */
					if (dungeon_level != FL_MAX_CAVE_OF_LARN_LEVEL - 1)
						break;
					it = OLARNEYE;
					mit = TRund(8) + DEMONLORD;
					marg = monster[mit].hitpoints;
					break;
				case '!': /* cure dianthroritis */
					if (dungeon_level != FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL - 1)
						break;
					it = OPOTION;
					arg = 21;
					mit = DEMONLORD + 7;
					marg = monster[mit].hitpoints;
					break;
				case '.': /* random monster */
					if (dungeon_level < FL_MAX_CAVE_OF_LARN_LEVEL)
						break;
					mit = fl_create_monster(dungeon_level + 1);
					marg = monster[mit].hitpoints;
					break;
				case '-': /* random object */
					it = fl_new_rand_object(dungeon_level + 1, &arg);
					break;
			}
			object_identification[j][i] = it;
			object_argument[j][i] = arg;
			monster_identification[j][i] = mit;
			monster_hit_points[j][i] = marg;
			been_here_before[j][i] = 0;
		}
	return 1;
}