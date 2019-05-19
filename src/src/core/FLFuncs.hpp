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
#include <curses.h>
#include <cstdlib>

class FLCoreFuncs {
	public:
		void IncreasePlayerLevel(void);
		void DecreasePlayerLevel(void);
		void IncreaseExperience(long);
		void DecreaseExperience(long);
		void DecreasePHealth(int);
		void IncreasePHealth(int);
};

int fl_create_monster(int);
void fl_place_the_player(void);
void fl_recalculate_armor_class(void);
void fl_exit_the_game(void);
int fl_more_info(char);
void fl_enchant_armor(void);
void fl_enchant_weapon(void);
int fl_monster_closeby(void);
int fl_stolen(void);
int fl_empty_handed(void);
void fl_create_gem(void);
void adjustcvalues(int, int);
char getyn(void);
int packweight(void);
void parse2(void);
void parse(void);
