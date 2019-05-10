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

#include "FLStore.hpp"
#include "../config/FLIncludeData.hpp"
#include "FLTermCapIO.hpp"

/*  new function for displaying gold in inventory inside trading posts.
    part of feature request by hymie0. ~Gibbon */
int FLStore::AmountGoldTrad(void) {
	lprintf("You have");
	lprintf(" %-6d ", cdesc[FL_GOLD]);
	lprintf("gold pieces.");
	return (0);
}
/*Fix for bug #23 ~Gibbon*/
int FLStore::LRSWelcomeText(void) {
	fl_clear_and_reset_screen();
	enable_scroll = 0;
	lprintf("Welcome to the Larn Revenue Service\n");
	return (0);
}
