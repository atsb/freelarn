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
    function to eat away a filled in maze
*/
void
fl_carve_out_the_dungeon(int xx, int yy) {
	int sphere_direction, fl_eat_try;
	sphere_direction = TRnd(4);
	fl_eat_try = 2;
	while (fl_eat_try) {
		switch (sphere_direction) {
			case 1:
				if (xx <= 2)
					break;		/*  west    */
				if ((object_identification[xx - 1][yy] != OWALL) || (object_identification[xx - 2][yy] != OWALL))
					break;
				object_identification[xx - 1][yy] = object_identification[xx - 2][yy] = 0;
				fl_carve_out_the_dungeon(xx - 2, yy);
				break;
			case 2:
				if (xx >= FL_MAX_HORIZONTAL_POSITION - 3)
					break;		/*  east    */
				if ((object_identification[xx + 1][yy] != OWALL) || (object_identification[xx + 2][yy] != OWALL))
					break;
				object_identification[xx + 1][yy] = object_identification[xx + 2][yy] = 0;
				fl_carve_out_the_dungeon(xx + 2, yy);
				break;
			case 3:
				if (yy <= 2)
					break;		/*  south   */
				if ((object_identification[xx][yy - 1] != OWALL) || (object_identification[xx][yy - 2] != OWALL))
					break;
				object_identification[xx][yy - 1] = object_identification[xx][yy - 2] = 0;
				fl_carve_out_the_dungeon(xx, yy - 2);
				break;
			case 4:
				if (yy >= FL_MAX_VERTICAL_POSITION - 3)
					break;		/*  north   */
				if ((object_identification[xx][yy + 1] != OWALL) || (object_identification[xx][yy + 2] != OWALL))
					break;
				object_identification[xx][yy + 1] = object_identification[xx][yy + 2] = 0;
				fl_carve_out_the_dungeon(xx, yy + 2);
				break;
		};
		if (++sphere_direction > 4) {
			sphere_direction = 1;
			--fl_eat_try;
		}
	}
}