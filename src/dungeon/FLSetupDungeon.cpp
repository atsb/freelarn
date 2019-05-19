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
#include "../terminal/FLTerminal.hpp"
#include "../core/FLFuncs.hpp"
#include "../core/FLTermCapIO.hpp"
#include "../core/FLScores.hpp"
#include "../save/FLSave.hpp"
#include "FLDungeon.hpp"

void
fl_make_a_maze(int k) {
	int i, j, tmp, tmp2, mx, mxl, mxh, my, myl, myh, z;
	if (k > 1 && (TRnd(17) <= 4 || k == FL_MAX_CAVE_OF_LARN_LEVEL - 1 || k == FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL - 1)) {
		/* read maze from data file */
		if (fl_maze_from_char(k))
			return;
	}
	if (k == 0) {
		tmp = 0;
	} else {
		tmp = OWALL;
	}
	for (i = 0; i < FL_MAX_VERTICAL_POSITION; i++)
		for (j = 0; j < FL_MAX_HORIZONTAL_POSITION; j++)
			object_identification[j][i] = tmp;
	if (k == 0) {
		return;
	}
	fl_carve_out_the_dungeon(1, 1);
	if (k == 1)
		object_identification[33][FL_MAX_VERTICAL_POSITION - 1] = OENTRANCE;
	/*  now for open spaces -- not on level 10  */
	if (k != FL_MAX_CAVE_OF_LARN_LEVEL - 1) {
		tmp2 = TRnd(3) + 3;
		for (tmp = 0; tmp < tmp2; tmp++) {
			my = TRnd(11) + 2;
			myl = my - TRnd(2);
			myh = my + TRnd(2);
			if (k < FL_MAX_CAVE_OF_LARN_LEVEL) {
				mx = TRnd(44) + 5;
				mxl = mx - TRnd(4);
				mxh = mx + TRnd(12) + 3;
				z = 0;
			} else {
				mx = TRnd(60) + 3;
				mxl = mx - TRnd(2);
				mxh = mx + TRnd(2);
				z = fl_create_monster(k);
			}
			for (i = mxl; i < mxh; i++)
				for (j = myl; j < myh; j++) {
					object_identification[i][j] = 0;
					monster_identification[i][j] = z;
					if (monster_identification[i][j] != 0)
						monster_hit_points[i][j] = monster[z].hitpoints;
				}
		}
	}
	if (k != FL_MAX_CAVE_OF_LARN_LEVEL - 1) {
		my = TRnd(FL_MAX_VERTICAL_POSITION - 2);
		for (i = 1; i < FL_MAX_HORIZONTAL_POSITION - 1; i++) {
			object_identification[i][my] = 0;
		}
	}
	if (k > 1) {
		fl_the_treasure_room(k);
	}
}

/*
    subroutine to create the objects in the maze for the given level
*/
void
fl_make_an_object(int j) {
	int i;
	if (j == 0) {
		/*  entrance to dungeon */
		fl_fill_the_room(OENTRANCE, 0);
		/*  the DND STORE */
		fl_fill_the_room(ODNDSTORE, 0);
		fl_fill_the_room(OBANK, 0);	/*  1st national bank of larn   */
		fl_fill_the_room(FL_OBJECT_TEMPLE_IN, 0);
		fl_fill_the_room(OHOME, 0);	/*  the players home & family   */
		fl_fill_the_room(OTRADEPOST, 0);	/*  the trading post            */
		fl_fill_the_room(OLRS, 0);	/*  the larn revenue service    */
		return;
	}
	if (j == FL_MAX_CAVE_OF_LARN_LEVEL) {
		fl_fill_the_room(FL_OBJECT_TEMPLE_OUT, 0);
	}
	/*  make the fixed objects in the maze STAIRS   */
	if ((j > 0) && (j != FL_MAX_CAVE_OF_LARN_LEVEL - 1) && (j != FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL - 1)) {
		fl_fill_the_room(OSTAIRSDOWN, 0);
	}
	if ((j > 1) && (j != FL_MAX_CAVE_OF_LARN_LEVEL)) {
		fl_fill_the_room(OSTAIRSUP, 0);
	}
	/*  make the random objects in the maze     */
	fillmroom(TRund(3), OBOOK, j);
	fillmroom(TRund(3), OPRAYERBOOK, j);
	fillmroom(TRund(3), OALTAR, 0);
	fillmroom(TRund(3), OSTATUE, 0);
	fillmroom(TRund(3), OPIT, 0);
	fillmroom(TRund(3), OFOUNTAIN, 0);
	fillmroom(TRnd(3) - 2, OIVTELETRAP, 0);
	fillmroom(TRund(2), OTHRONE, 0);
	fillmroom(TRund(2), OMIRROR, 0);
	fillmroom(TRund(2), OTRAPARROWIV, 0);
	fillmroom(TRnd(3) - 2, OIVDARTRAP, 0);
	fillmroom(TRund(3), OCOOKIE, 0);
	if (j == 1) {
		fillmroom(1, OCHEST, j);
	} else {
		fillmroom(TRund(2), OCHEST, j);
	}
	if ((j != FL_MAX_CAVE_OF_LARN_LEVEL - 1) && (j != FL_MAX_CAVE_OF_LARN_LEVEL + FL_MAX_TEMPLE_OF_LARN_LEVEL - 1)) {
		fillmroom(TRund(2), OIVTRAPDOOR, 0);
	}
	if (j <= 10) {
		fillmroom((TRund(2)), ODIAMOND, TRnd(10 * j + 1) + 10);
		fillmroom(TRund(2), ORUBY, TRnd(6 * j + 1) + 6);
		fillmroom(TRund(2), OEMERALD, TRnd(4 * j + 1) + 4);
		fillmroom(TRund(2), OSAPPHIRE, TRnd(3 * j + 1) + 2);
	}
	for (i = 0; i < TRnd(4) + 3; i++) {
		fl_fill_the_room(OPOTION, potion_probability[TRund(41)]);      /*  make a POTION   */
	}
	for (i = 0; i < TRnd(5) + 3; i++) {
		fl_fill_the_room(OSCROLL, scroll_probability[TRund(81)]);      /*  make a SCROLL   */
	}
	for (i = 0; i < TRnd(12) + 11; i++) {
		fl_fill_the_room(OGOLDPILE, 12 * TRnd(j + 1) + (j << 3) + 10);          /* make FL_GOLD */
	}
	if (j == 5) {
		fl_fill_the_room(OBANK2, 0);     /* branch office of the bank */
	}
	froom(2, ORING, 0);		/* a ring mail          */
	froom(1, OSTUDLEATHER, 0);	/* a studded leather    */
	froom(3, OSPLINT, 0);	/* a splint mail        */
	froom(5, OSHIELD, TRund(3));	/* a shield             */
	froom(2, OBATTLEAXE, TRund(3));	/* a battle axe         */
	froom(5, OLONGSWORD, TRund(3));	/* a long sword         */
	froom(4, OREGENRING, TRund(3));	/* ring of regeneration */
	froom(1, OPROTRING, TRund(3));	/* ring of protection   */
	froom(2, OSTRRING, 1 + TRnd(3));	/* ring of strength     */
	froom(7, OSPEAR, TRnd(5));	/* a spear              */
	froom(3, OORBOFDRAGON, 0);	/* orb of dragon slaying */
	froom(4, OSPIRITSCARAB, 0);	/* scarab of negate spirit */
	froom(4, OCUBEofUNDEAD, 0);	/* cube of undead control   */
	froom(2, ORINGOFEXTRA, 0);	/* ring of extra regeneneration      */
	froom(3, ONOTHEFT, 0);	/* device of antitheft      */
	froom(2, OSWORDofSLASHING, 0);	/* sword of slashing */
	if (cdesc[FL_BESS_MANN] == 0) {
		froom(4, OHAMMER, 0);	/*Bessman's flailing hammer */
		cdesc[FL_BESS_MANN] = 1;
	}
	if ((TRnd(4) == 3)) {
		if (j > 3) {
			froom(3, OSWORD, 3);	/* sunsword + 3         */
			froom(5, O2SWORD, TRnd(4));	/* a two handed sword */
			froom(5, OHSWORD, TRnd(4));	/* a longsword of Hymie     */
			froom(3, OBELT, 4);	/* belt of striking     */
			froom(3, OENERGYRING, 3);	/* energy ring          */
			froom(4, OPLATE, 5);	/* platemail + 5        */
			froom(3, OCLEVERRING, 1 + TRnd(2));	/* ring of cleverness */
		}
	}
}

void
fl_create_new_monster_set(int flg) {
	int i, j;
	if (flg) {
		for (i = 0; i < FL_MAX_VERTICAL_POSITION; i++) {
			for (j = 0; j < FL_MAX_HORIZONTAL_POSITION; j++) {
				stealth[j][i] = 0;
			}
		}
	}
	/* if teleported and found level 1 then been_here_before level we are on */
	if (level == 0) {
		cdesc[FL_TELEPORT_FLAG] = 0;
		return;
	}
	if (flg) {
		j = TRnd(12) + 2 + (level >> 1);
	} else {
		j = (level >> 1) + 1;
	}
	for (i = 0; i < j; i++) {
		fl_fill_dungeon_with_monsters(fl_create_monster(level));
	}
}
