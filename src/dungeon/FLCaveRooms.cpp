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
fillmroom(int n, int what, int arg) {
	int i;
	for (i = 0; i < n; i++) {
		fl_fill_the_room(what, arg);
	}
}

void
froom(int n, int itm, int arg) {
	if (TRnd(151) < n) {
		fl_fill_the_room(itm, arg);
	}
}

void
fl_fill_the_room(int what, int arg) {
	int x, y;
	x = TRnd(FL_MAX_HORIZONTAL_POSITION - 2);
	y = TRnd(FL_MAX_VERTICAL_POSITION - 2);
	while (object_identification[x][y]) {
		x += TRnd(3) - 2;
		y += TRnd(3) - 2;
		if (x > FL_MAX_HORIZONTAL_POSITION - 2) {
			x = 1;
		}
		if (x < 1) {
			x = FL_MAX_HORIZONTAL_POSITION - 2;
		}
		if (y > FL_MAX_VERTICAL_POSITION - 2) {
			y = 1;
		}
		if (y < 1) {
			y = FL_MAX_VERTICAL_POSITION - 2;
		}
	}
	object_identification[x][y] = what;
	object_argument[x][y] = arg;
}