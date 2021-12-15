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
#include <string>

void ansiterm_init(void);
void ansiterm_clean_up(void);
void ansiterm_command(int, const char *, const char *);
void bottomline(void);
void bottomhp(void);
void bottomspell(void);
void bottomdo(void);
void bot_linex(void);
void draws(int, int, int, int);
void drawscreen(void);
void showcell(int, int);
void fl_show_designated_cell_only(int, int);
void showplayer(void);
int moveplayer(int);
void run(int dir);
void seemagic(int);
