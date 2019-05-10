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
#include "FLDungeon.hpp"

void
fl_the_treasure_room(int lv) {
	int tx, ty, xsize, ysize;
	for (tx = 1 + TRnd(10); tx < FL_MAX_HORIZONTAL_POSITION - 10; tx += 10)
		if ((lv == FL_MAX_CAVE_OF_LARN_LEVEL - 1) || (lv == FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL - 1) || TRnd(13) == 2) {
			xsize = TRnd(6) + 3;
			ysize = TRnd(3) + 3;
			ty = TRnd(FL_MAX_VERTICAL_POSITION - 9) + 1; /* upper left corner of room */
			if (lv == FL_MAX_CAVE_OF_LARN_LEVEL - 1 || lv == FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL - 1) {
				troom(lv, xsize, ysize, tx = tx + TRnd(FL_MAX_HORIZONTAL_POSITION - 24), ty, TRnd(3) + 6);
			} else {
				troom(lv, xsize, ysize, tx, ty, TRnd(9));
			}
		}
}

void
troom(int lv, int xsize, int ysize, int tx, int ty, int glyph) {
	int i, j;
	int tp1, tp2;
	for (j = ty - 1; j <= ty + ysize; j++)
		for (i = tx - 1; i <= tx + xsize; i++)	/* clear out space for room */
			object_identification[i][j] = 0;
	for (j = ty; j < ty + ysize; j++)
		for (i = tx; i < tx + xsize; i++) {	/* now put in the walls */
			object_identification[i][j] = OWALL;
			monster_identification[i][j] = 0;
		}
	for (j = ty + 1; j < ty + ysize - 1; j++)
		for (i = tx + 1; i < tx + xsize - 1; i++)	/* now clear out interior */
			object_identification[i][j] = 0;
	switch (TRnd(2)) {	/* locate the door on the treasure room */
		case 1:
			object_identification[i = tx + TRund(xsize)][j = ty + (ysize - 1) * TRund(2)] =
			    OCLOSEDDOOR;
			object_argument[i][j] = glyph;	/* on horizontal walls */
			break;
		case 2:
			object_identification[i = tx + (xsize - 1) * TRund(2)][j = ty + TRund(ysize)] =
			    OCLOSEDDOOR;
			object_argument[i][j] = glyph;	/* on vertical walls */
			break;
	};
	tp1 = player_horizontal_position;
	tp2 = player_vertical_position;
	player_vertical_position = ty + (ysize >> 1);
	for (player_horizontal_position = tx + 1; player_horizontal_position <= tx + xsize - 2; player_horizontal_position += 2) {
		for (i = 0, j = TRnd(6); i <= j; i++) {
			fl_create_rand_object_around_player_location(lv + 2);
			fl_create_a_monster(fl_create_monster(lv + 1));
		}
	}
	player_horizontal_position = tp1;
	player_vertical_position = tp2;
}
