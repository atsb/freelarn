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

#include <time.h>
#include "FLNewGame.hpp"
#include "FLTermCapIO.hpp"
#include "../config/FLIncludeData.hpp"

/*
    Subroutine to save the initial time and seed TRnd()
*/
void FLGame::NewGame(void) {
	long *p, *pe;
	for (p = cdesc, pe = cdesc + 100; p < pe; p++) {
		*p = 0;
	}
	time(&initialtime);
	#if defined NIX || NIX_LOCAL
	srand(reinterpret_cast<long>(initialtime));
	#else
	srand(reinterpret_cast<long long>(initialtime));
	#endif
	//buffering for output
	fl_create_new_file(reinterpret_cast<char *>(0));
}
