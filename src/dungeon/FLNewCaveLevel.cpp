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
    fl_generate_a_new_dungeon_level(level)
    int level;

    function to enter a new level.  This routine must be called anytime the
    player changes levels.  If that level is unknown it will be created.
    A new set of monsters will be created for a new level, and existing
    levels will get a few more monsters.
    Note that it is here we remove genocided monsters from the present level.
*/

void
fl_generate_a_new_dungeon_level(int x) {
	int i, j, genocided_x, genocided_y;
	level = x;	/* get the new level and put in working storage*/
	for (i = 0; i < FL_MAX_VERTICAL_POSITION ; i++) {
		for (j = 0 ; j < FL_MAX_HORIZONTAL_POSITION ; j++) {
			been_here_before[j][i] = 0;
			monster_identification[j][i] = 0;
		}
	}
	fl_make_a_maze(x);
	fl_make_an_object(x);
	beenhere[x] = 1; /* first time here */
	fl_create_new_monster_set(1);
	if (wizard || x == 0)
		for (j = 0; j < FL_MAX_VERTICAL_POSITION; j++)
			for (i = 0; i < FL_MAX_HORIZONTAL_POSITION; i++)
				been_here_before[i][j] = 1;
	for (genocided_y = 0; genocided_y < FL_MAX_VERTICAL_POSITION; genocided_y++)
		for (genocided_x = 0; genocided_x < FL_MAX_HORIZONTAL_POSITION; genocided_x++)
			if (monster[monster_identification[genocided_x][genocided_y]].genocided)
				monster_identification[genocided_x][genocided_y] = 0; /* no more monster */
}