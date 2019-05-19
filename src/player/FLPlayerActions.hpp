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

void fl_a_remove_the_gems(int);
void fl_a_sit_on_throne(int);
void fl_a_travel_up_stairs(void);
void fl_a_travel_down_stairs(void);
void fl_a_drink_from_fountain(void);
void fl_a_wash_in_fountain(void);
void fl_a_enter_the_temple(void);
void fl_a_exit_the_temple(void);
void fl_a_desecrate_an_altar(void);
void fl_a_give_donation(void);
void fl_a_just_pray(void);
void fl_a_give_thanks(void);
void fl_a_ignore_an_altar(void);
void fl_a_open_chest(int, int);
int fl_a_open_dungeon_door(int, int);
void fl_look_for_an_object_and_give_options(char, char, char);
void fl_teleport(int);
void fl_drink_potion(int, int);
void fl_adjust_time(int);
void fl_read_scroll(int);
void fl_read_a_book(int);
void fl_read_a_prayer_book(int);
void fl_player_home(void);
void fl_cast_a_spell(void);
int fl_is_fullhit(int);
void fl_go_direction(int, int, const char *, int, const char);
void fl_player_is_blind(int, int);
int fl_direction(int *, int *);
void fl_do_annihilation(void);
int readnum(int);
void wield(void);
void wear(void);
void dropobj(void);
int floor_consume(int, const char *);
int whatitem(const char *);
void consume(int, const char *);

