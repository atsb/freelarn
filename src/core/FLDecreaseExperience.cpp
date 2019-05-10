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
#include "../templates/FLTMath.hpp"
#include "../config/FLIncludeData.hpp"
#include "../terminal/FLTerminal.hpp"
#include "FLTermCapIO.hpp"

void FLCoreFuncs::DecreaseExperience(long x) {
	int i, tmp;
	i = cdesc[FL_LEVEL];
	cdesc[FL_EXPERIENCE] -= x;
	if (cdesc[FL_EXPERIENCE] < 0) {
		cdesc[FL_EXPERIENCE] = 0;
	}
	while (cdesc[FL_EXPERIENCE] < skill[cdesc[FL_LEVEL] - 1]) {
		if (--cdesc[FL_LEVEL] <= 1) {
			cdesc[FL_LEVEL] = 1;  /*  down one level      */
		}
		tmp = (cdesc[FL_CONSTITUTION]) >> 1;	/* lose hpoints */
		FL_LOSEMAXHEALTH(TRnd((tmp > 0) ? tmp : 1));	/* lose hpoints */
		if (cdesc[FL_LEVEL] < 7) {
			FL_LOSEMAXSPELLS((cdesc[FL_CONSTITUTION] >> 2));
		}
		FL_LOSEMAXHEALTH(TRund(3));	/*  lose spells     */
	}
	if (i != cdesc[FL_LEVEL]) {
		fl_termcap_cursor_position(1, 24);
		lprintf("\nYou went down to level %d!", cdesc[FL_LEVEL]);
	}
	bottomline();
}