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

class HitMonster {
	public:
		int hitm(int, int, int);
		int hpoints;
		int amt2;
		int lastx;
		int lasty;
};

void specify_obj_cursor(void);
void specify_obj_nocurs(void);
void fillmroom(int, int, int);
void froom(int, int, int);
void fl_fill_the_room(int, int);
void troom(int, int, int, int, int, int);
void fl_the_treasure_room(int);
int fl_maze_from_char(int);
void fl_create_the_player_character(void);
void fl_generate_a_new_dungeon_level(int);
void fl_carve_out_the_dungeon(int, int);
int fl_fill_dungeon_with_monsters(int);
void fl_create_new_monster_set(int);
void fl_make_an_object(int);
void fl_make_a_maze(int);
int fl_a_new_sphere_of_annihilation(int, int, int, int);
int fl_remove_sphere_of_annihilation(int, int);
void fl_effect_of_sphere_detonation(int x, int y);
void fl_move_spheres_of_annihilation(void);
int fl_sphere_boom(int, int);
void fl_create_an_item(int, int);
void fl_create_a_monster(int);
void fl_drop_rand_gold_around_player_location(int);
void fl_extra_object_func_throne(int);
void fl_change_char_level(int);
void fl_open_an_object(void);
void fl_close_an_object(void);
void fl_move_a_monster(void);
void fl_put_queue(int, int, int);
void fl_get_queue(int, int, int);
void fl_hit_a_monster(int, int);
void fl_hit_the_player(int, int);
int fl_new_rand_object(int, int *);
void fl_create_rand_object_around_player_location(int);
int fl_verify_bound_coordinates(int *, int *);
