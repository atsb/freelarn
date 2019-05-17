/*  Copyright 2016 Gibbon aka 'atsb'
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
#include "../core/FLInventory.hpp"
#include "../core/FLTermCapIO.hpp"
#include "FLRegenerate.hpp"

void
fl_regen_hp_and_spells(void) {
	int i, flag;
	long *larn_cdesc_id;
	larn_cdesc_id = cdesc;
	larn_cdesc_id[FL_MOVESMADE]++;
	if (larn_cdesc_id[FL_TIMESTOP]) {
		if (--larn_cdesc_id[FL_TIMESTOP] <= 0) {
			bottomline();
		}
		return;
	}				/* for stop time spell */
	flag = 0;
	if (larn_cdesc_id[FL_STRENGTH] < 3) {
		larn_cdesc_id[FL_STRENGTH] = 3;
		flag = 1;
	}
	if (larn_cdesc_id[FL_HP] != larn_cdesc_id[FL_HPMAX])
		if (larn_cdesc_id[FL_REGENCOUNTER]-- <= 0) {	/* regenerate hit points */
			larn_cdesc_id[FL_REGENCOUNTER] = 20 + larn_cdesc_id[FL_LEVEL];
			if ((larn_cdesc_id[FL_HP] += larn_cdesc_id[FL_REGEN]) > larn_cdesc_id[FL_HPMAX]) {
				larn_cdesc_id[FL_HP] = larn_cdesc_id[FL_HPMAX];
			}
			bottomhp();
		}
	/* regenerate spells */
	if (larn_cdesc_id[FL_SPELLS] < larn_cdesc_id[FL_SPELLMAX])
		if (larn_cdesc_id[FL_ECOUNTER]-- <= 0) {
			larn_cdesc_id[FL_ECOUNTER] = 100 + 4 * larn_cdesc_id[FL_LEVEL] - larn_cdesc_id[FL_ENERGY];
			larn_cdesc_id[FL_SPELLS]++;
			bottomspell();
		}
	if (larn_cdesc_id[FL_HERO])
		if (--larn_cdesc_id[FL_HERO] <= 0) {
			for (i = 0; i < 5; i++) {
				larn_cdesc_id[i] -= 10;
			}
			flag = 1;
		}
	if (larn_cdesc_id[FL_ALTPRO])
		if (--larn_cdesc_id[FL_ALTPRO] <= 0) {
			larn_cdesc_id[FL_MOREDEFENSES] -= 3;
			flag = 1;
		}
	if (larn_cdesc_id[FL_PROTECTIONTIME])
		if (--larn_cdesc_id[FL_PROTECTIONTIME] <= 0) {
			larn_cdesc_id[FL_MOREDEFENSES] -= 2;
			flag = 1;
		}
	if (larn_cdesc_id[FL_DEXCOUNT])
		if (--larn_cdesc_id[FL_DEXCOUNT] <= 0) {
			larn_cdesc_id[FL_DEXTERITY] -= 3;
			flag = 1;
		}
	if (larn_cdesc_id[FL_STRCOUNT])
		if (--larn_cdesc_id[FL_STRCOUNT] <= 0) {
			larn_cdesc_id[FL_EXTRA_STRENGTH] -= 3;
			flag = 1;
		}
	if (larn_cdesc_id[FL_BLINDCOUNT])
		if (--larn_cdesc_id[FL_BLINDCOUNT] <= 0) {
			fl_termcap_cursor_position(1, 24);
			fl_display_message("\nThe blindness subsides");
		}
	if (larn_cdesc_id[FL_CONFUSE])
		if (--larn_cdesc_id[FL_CONFUSE] <= 0) {
			fl_termcap_cursor_position(1, 24);
			fl_display_message("\nYou regain your senses");
		}
	if (larn_cdesc_id[FL_GIANTSTR])
		if (--larn_cdesc_id[FL_GIANTSTR] <= 0) {
			larn_cdesc_id[FL_EXTRA_STRENGTH] -= 20;
			flag = 1;
		}
	if (larn_cdesc_id[FL_CHARMCOUNT])
		if ((--larn_cdesc_id[FL_CHARMCOUNT]) <= 0) {
			flag = 1;
		}
	if (larn_cdesc_id[FL_INVISIBILITY])
		if ((--larn_cdesc_id[FL_INVISIBILITY]) <= 0) {
			flag = 1;
		}
	if (larn_cdesc_id[FL_CANCELLATION])
		if ((--larn_cdesc_id[FL_CANCELLATION]) <= 0) {
			flag = 1;
		}
	if (larn_cdesc_id[FL_WALK_THROUGH_WALLS])
		if ((--larn_cdesc_id[FL_WALK_THROUGH_WALLS]) <= 0) {
			flag = 1;
		}
	if (larn_cdesc_id[FL_HASTESELF])
		if ((--larn_cdesc_id[FL_HASTESELF]) <= 0) {
			flag = 1;
		}
	if (larn_cdesc_id[FL_AGGRAVATE]) {
		--larn_cdesc_id[FL_AGGRAVATE];
	}
	if (larn_cdesc_id[FL_SCAREMONST])
		if ((--larn_cdesc_id[FL_SCAREMONST]) <= 0) {
			flag = 1;
		}
	if (larn_cdesc_id[FL_STEALTH])
		if ((--larn_cdesc_id[FL_STEALTH]) <= 0) {
			flag = 1;
		}
	if (larn_cdesc_id[FL_AWARENESS]) {
		--larn_cdesc_id[FL_AWARENESS];
	}
	if (larn_cdesc_id[FL_HOLDMONST])
		if ((--larn_cdesc_id[FL_HOLDMONST]) <= 0) {
			flag = 1;
		}
	if (larn_cdesc_id[FL_HASTEMONST]) {
		--larn_cdesc_id[FL_HASTEMONST];
	}
	if (larn_cdesc_id[FL_FIRERESISTANCE])
		if ((--larn_cdesc_id[FL_FIRERESISTANCE]) <= 0) {
			flag = 1;
		}
	if (larn_cdesc_id[FL_GLOBE])
		if (--larn_cdesc_id[FL_GLOBE] <= 0) {
			larn_cdesc_id[FL_MOREDEFENSES] -= 10;
			flag = 1;
		}
	if (larn_cdesc_id[FL_SPIRITPRO])
		if (--larn_cdesc_id[FL_SPIRITPRO] <= 0) {
			flag = 1;
		}
	if (larn_cdesc_id[FL_UNDEADPRO])
		if (--larn_cdesc_id[FL_UNDEADPRO] <= 0) {
			flag = 1;
		}
	if (larn_cdesc_id[FL_HALFDAM])
		if (--larn_cdesc_id[FL_HALFDAM] <= 0) {
			fl_termcap_cursor_position(1, 24);
			fl_display_message("\nYou're feeling better");
		}
	if (larn_cdesc_id[FL_SEEINVISIBLE])
		if (--larn_cdesc_id[FL_SEEINVISIBLE] <= 0) {
			monstnamelist[INVISIBLEDIWATA] = floorc;
			if (!larn_cdesc_id[FL_BLINDCOUNT]) {
				fl_termcap_cursor_position(1, 24);
				fl_display_message("\nYour vision returns to normal");
			}
		}
	if (larn_cdesc_id[FL_ITCHING]) {
		if (larn_cdesc_id[FL_ITCHING] > 1)
			if ((larn_cdesc_id[FL_WEAR] != -1) || (larn_cdesc_id[FL_SHIELD] != -1))
				if (TRnd(100) < 50) {
					larn_cdesc_id[FL_WEAR] = larn_cdesc_id[FL_SHIELD] = -1;
					fl_termcap_cursor_position(1, 24);
					fl_display_message("\nThe unbareable itching forces you to remove your armor!");
					fl_recalculate_armor_class();
					bottomline();
				}
		if (--larn_cdesc_id[FL_ITCHING] <= 0) {
			fl_termcap_cursor_position(1, 24);
			fl_display_message("\nThe irritation subsides!");
		}
	}
	if (larn_cdesc_id[FL_CLUMSINESS]) {
		if (larn_cdesc_id[FL_WIELD] != -1)
			if (larn_cdesc_id[FL_CLUMSINESS] > 1)
				if (object_identification[player_horizontal_position][player_vertical_position] == 0)	/* only if nothing there */
					if (TRnd(100) < 33) {		/* drop your weapon due to clumsiness */
						drop_object((int)larn_cdesc_id[FL_WIELD]);
					}
		if (--larn_cdesc_id[FL_CLUMSINESS] <= 0) {
			fl_termcap_cursor_position(1, 24);
			fl_display_message("\nYou feel less awkward!");
		}
	}
	if (flag) {
		bottomline();
	}
}
