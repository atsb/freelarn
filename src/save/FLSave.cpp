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

#include <string>
#include "FLSave.hpp"
#include "../config/FLIncludeData.hpp"
#include "../templates/FLTMath.hpp"
#include "../core/FLTermCapIO.hpp"
#include "../core/FLScores.hpp"
#include "../core/FLStore.hpp"
#include "../core/FLSystemDependant.hpp"
#include "../dungeon/FLDungeon.hpp"
#include "../core/FLFuncs.hpp"

void
Save::save(void) {
	struct cel *pcel;
	int *pitem, *pknow, *pmitem;
	int *phitp, *piarg;
	struct cel *pecel;
	pcel = NULL;
	/* pointer to past end of this level's cells */
	pecel = pcel;
	pitem = object_identification[0];
	piarg = object_argument[0];
	pknow = been_here_before[0];
	pmitem = monster_identification[0];
	phitp = monster_hit_points[0];
	while (pcel < pecel) {
		pcel->monster_identification = *pmitem++;
		pcel->monster_hit_points = *phitp++;
		pcel->object_identification = *pitem++;
		pcel->been_here_before = *pknow++;
		pcel->object_argument = *piarg++;
		pcel++;
	}
}

/*
    routine to restore a level from storage
*/
void
Load::load(void) {
	struct cel *pcel;
	int *pitem;
	int *pknow;
	int *pmitem;
	int *phitp, *piarg;
	struct cel *pecel;
	pcel = NULL;
	/* pointer to past end of this level's cells */
	pecel = pcel;
	pitem = object_identification[0];
	piarg = object_argument[0];
	pknow = been_here_before[0];
	pmitem = monster_identification[0];
	phitp = monster_hit_points[0];
	while (pcel < pecel) {
		*pmitem++ = pcel->monster_identification;
		*phitp++ = pcel->monster_hit_points;
		*pitem++ = pcel->object_identification;
		*pknow++ = pcel->been_here_before;
		*piarg++ = pcel->object_argument;
		pcel++;
	}
}

static time_t zzz = 0;

int
Save::savegame(char *fname) {
	int i, k;
	struct sphere *sp;
	time_t temptime;
	Save save;
	FLStore Store;
	fl_output_buffer_flush();
	save.save();
	Store.InterestRate();
	if (fl_create_new_file(fname) < 0) {
		fl_create_new_file(reinterpret_cast<char *>(0));
		lprintf("\nCan't open file <%s> to save game\n", fname);
		return (-1);
	}
	enable_scroll = -1;
	fl_write_current_buffer_to_output_buffer(logname, FL_MAX_PLAYER_NAME_LENGTH);
	fl_write_current_buffer_to_output_buffer(reinterpret_cast<char *>(beenhere), (sizeof(int) * (FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL)));
	fl_write_current_buffer_to_output_buffer(reinterpret_cast<char *>(&cdesc[0]), 100 * sizeof(long));
	fl_write_current_buffer_to_output_buffer(reinterpret_cast<char *>(&gtime), 1 * sizeof(long));
	lprint(level);
	lprint(player_horizontal_position);
	lprint(player_vertical_position);
	fl_write_current_buffer_to_output_buffer(reinterpret_cast<char *>(iven), 26 * sizeof(int));
	fl_write_current_buffer_to_output_buffer(reinterpret_cast<char *>(ivenarg), 26 * sizeof(int));
	for (k = 0; k < MAXSCROLL; k++) {
		fl_output_single_byte_to_output_buffer(scrollname[k][0]);
	}
	for (k = 0; k < MAXPOTION; k++) {
		fl_output_single_byte_to_output_buffer(potionname[k][0]);
	}
	fl_write_current_buffer_to_output_buffer(reinterpret_cast<char *>(spelknow), FL_MAX_SPELLS_IN_GAME * sizeof(int));
	lprint(wizard);
	lprint(random_monster_creation);		/*  random monster generation counter */
	for (i = 0; i < 90; i++) {
		lprint(dnd_item[i].qty);
	}
	lprint(cheat);
	for (i = 0; i < FL_MAX_MONSTERS_IN_GAME; i++)
		lprint(monster[i].genocided);	/* genocide info */
	for (sp = spheres; sp; sp = sp->p)
		fl_write_current_buffer_to_output_buffer(reinterpret_cast<char *>(sp), sizeof(struct sphere));	/* save spheres of annihilation */
	time(&zzz);
	temptime = zzz - initialtime;
	fl_write_current_buffer_to_output_buffer(reinterpret_cast<char *>(&temptime), sizeof(time_t));
	lprintf("%s", FL_BUILD_VERSION);
	fl_close_file_flush();
	lastmonst[0] = 0;
	enable_scroll = 1;
	fl_create_new_file(reinterpret_cast<char *>(0));
	return (0);
}

void
Load::restoregame(char *fname) {
	FLCoreFuncs CoreFuncs;
	int i, k;
	struct sphere *sp, *sp2;
	/*struct stat filetimes; */
	time_t temptime;
	Load load;
	fl_termcap_cursor_position(1, 24);
	fl_display_message("\nRestoring . . .");
	fl_output_buffer_flush();
	if (fl_open_file(fname) <= 0) {
		fl_create_new_file(reinterpret_cast<char *>(0));
		lprintf("\nCan't open file <%s>to restore game\n", fname);
		fl_wait(FL_WAIT_DURATION);
		cdesc[FL_GOLD] = cdesc[FL_BANK_ACCOUNT] = 0;
		fl_player_death(-265);
		return;
	}
	lrfill(logname, FL_MAX_PLAYER_NAME_LENGTH);
	lrfill(reinterpret_cast<char *>(beenhere), (sizeof(int) * (FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL)));
	lrfill(reinterpret_cast<char *>(&cdesc[0]), 100 * sizeof(long));
	lrfill(reinterpret_cast<char *>(&gtime), 1 * sizeof(long));
	level = cdesc[FL_CAVE_LEVEL] = larint();
	player_horizontal_position = larint();
	player_vertical_position = larint();
	fl_generate_a_new_dungeon_level(level);
	lrfill(reinterpret_cast<char *>(iven), 26 * sizeof(int));
	lrfill(reinterpret_cast<char *>(ivenarg), 26 * sizeof(int));
	for (k = 0; k < MAXSCROLL; k++) {
		scrollname[k][0] = fl_read_char_from_input_buffer();
	}
	for (k = 0; k < MAXPOTION; k++) {
		potionname[k][0] = fl_read_char_from_input_buffer();
	}
	lrfill(reinterpret_cast<char *>(spelknow), FL_MAX_SPELLS_IN_GAME * sizeof(int));
	wizard = larint();
	random_monster_creation = larint();		/*  random monster creation flag */
	for (i = 0; i < 90; i++) {
		dnd_item[i].qty = larint();
	}
	cheat = larint();
	for (i = 0; i < FL_MAX_MONSTERS_IN_GAME; i++)
		monster[i].genocided = larint();	/* genocide info */
	for (sp = 0, i = 0; i < cdesc[FL_SPHERE_CAST]; i++) {
		sp2 = sp;
		sp = (struct sphere *) operator new (sizeof(struct sphere));
		if (sp == 0) {
			fprintf(stderr, "Can't malloc() for sphere space\n");
			break;
		}
		lrfill(reinterpret_cast<char *>(sp), sizeof(struct sphere));	/* get spheres of annihilation */
		sp->p = 0;		/* null out pointer */
		if (i == 0)
			spheres = sp;		/* beginning of list */
		else
			sp2->p = sp;
	}
	time(&zzz);
	lrfill(reinterpret_cast<char *>(&temptime), sizeof(time_t));
	initialtime = zzz - temptime;
	fl_close_the_file();
	oldx = oldy = 0;
	/* died a post mortem death */
	if (cdesc[FL_HP] < 0) {
		fl_player_death(284);
		return;
	}
	/* if patch up lev 25 player */
	if (cdesc[FL_LEVEL] == 25 && cdesc[FL_EXPERIENCE] > skill[24]) {
		long tmp;
		tmp = cdesc[FL_EXPERIENCE] - skill[24];	/* amount to go up */
		cdesc[FL_EXPERIENCE] = skill[24];
		CoreFuncs.IncreaseExperience(tmp);
	}
	load.load();
	gtime -= 1;			/* HACK for time advancing either on save or reload */
	lasttime = gtime - 1;
}
