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
#include "FLFuncs.hpp"
#include "FLInventory.hpp"
#include "FLTermCapIO.hpp"
#include "../player/FLPlayerActions.hpp"
#include "FLScores.hpp"
#include "FLStore.hpp"
#include "FLSystemDependant.hpp"

static void fl_dnd_item_list(int i);
static void fl_show_trading_store_inventory(void);
static void fl_clear_trading_inventory(int);

int dndcount = 0;
int dnditm = 0;

/*  this is the data for the stuff in the dnd store */
struct _itm dnd_item[90] = {
	/*  cost   iven name   iven arg   how
	    gp      iven[]    ivenarg[]  many */

	{2, OLEATHER, 0, 3},
	{10, OSTUDLEATHER, 0, 2},
	{40, ORING, 0, 2},
	{85, OCHAIN, 0, 2},
	{220, OSPLINT, 0, 1},
	{400, OPLATE, 0, 1},
	{900, OPLATEARMOR, 0, 1},
	{750, OFIRETUNIC, 0, 1},
	{2600, OSSPLATE, 0, 1},
	{150, OSHIELD, 0, 1},

	/*  cost    memory    iven name   iven arg   how
	    gp     pointer      iven[]    ivenarg[]  many */

	{2, ODAGGER, 0, 3},
	{20, OSPEAR, 0, 3},
	{150, OBATTLEAXE, 0, 2},
	{450, OLONGSWORD, 0, 2},
	{1000, O2SWORD, 0, 2},
	{5000, OSWORD, 0, 1},

	/* lets make it twice as expensive. ~Gibbon */
	{30000, OGREATSWORD, 0, 1},
	{6000, OSWORDofSLASHING, 0, 0},
	{10000, OHAMMER, 0, 0},

	/*  cost    memory    iven name   iven arg   how
	    gp     pointer      iven[]    ivenarg[]  many */

	{150, OPROTRING, 1, 1},
	{85, OSTRRING, 1, 1},
	{120, ODEXRING, 1, 1},
	{120, OCLEVERRING, 1, 1},
	{180, OENERGYRING, 0, 1},
	{125, ODAMRING, 0, 1},
	{220, OREGENRING, 0, 1},
	{1000, ORINGOFEXTRA, 0, 1},

	{280, OBELT, 0, 1},

	{400, OAMULET, 0, 1},

	{6500, OORBOFDRAGON, 0, 0},
	{5500, OSPIRITSCARAB, 0, 0},
	{5000, OCUBEofUNDEAD, 0, 0},
	{6000, ONOTHEFT, 0, 0},

	{590, OCHEST, 6, 1},
	{200, OBOOK, 8, 1},
	{100, OPRAYERBOOK, 7, 1},
	{10, OCOOKIE, 0, 3},

	/*  cost    memory    iven name   iven arg   how
	    gp     pointer      iven[]    ivenarg[]  many */

	{20, OPOTION, 0, 6},
	{90, OPOTION, 1, 5},
	{520, OPOTION, 2, 1},
	{100, OPOTION, 3, 2},
	{50, OPOTION, 4, 2},
	{150, OPOTION, 5, 2},
	{70, OPOTION, 6, 1},
	{30, OPOTION, 7, 7},
	{200, OPOTION, 8, 1},
	{50, OPOTION, 9, 1},
	{80, OPOTION, 10, 1},

	/*  cost    memory    iven name   iven arg   how
	    gp     pointer      iven[]    ivenarg[]  many */

	{30, OPOTION, 11, 3},
	{20, OPOTION, 12, 5},
	{40, OPOTION, 13, 3},
	{35, OPOTION, 14, 2},
	{520, OPOTION, 15, 1},
	{90, OPOTION, 16, 2},
	{200, OPOTION, 17, 2},
	{220, OPOTION, 18, 4},
	{80, OPOTION, 19, 6},
	{370, OPOTION, 20, 3},
	{50, OPOTION, 22, 1},
	{150, OPOTION, 23, 3},

	/*  cost    memory    iven name   iven arg   how
	    gp     pointer      iven[]    ivenarg[]  many */

	{100, OSCROLL, 0, 2},
	{125, OSCROLL, 1, 2},
	{60, OSCROLL, 2, 4},
	{10, OSCROLL, 3, 4},
	{100, OSCROLL, 4, 3},
	{200, OSCROLL, 5, 2},
	{110, OSCROLL, 6, 1},
	{500, OSCROLL, 7, 2},
	{200, OSCROLL, 8, 2},
	{250, OSCROLL, 9, 4},
	{20, OSCROLL, 10, 5},
	{30, OSCROLL, 11, 3},

	/*  cost    memory    iven name   iven arg   how
	    gp     pointer      iven[]    ivenarg[]  many */

	{340, OSCROLL, 12, 1},
	{340, OSCROLL, 13, 1},
	{300, OSCROLL, 14, 2},
	{400, OSCROLL, 15, 2},
	{500, OSCROLL, 16, 2},
	{1000, OSCROLL, 17, 1},
	{500, OSCROLL, 18, 1},
	{340, OSCROLL, 19, 2},
	{220, OSCROLL, 20, 3},
	{3900, OSCROLL, 21, 0},
	{610, OSCROLL, 22, 1},
	{3000, OSCROLL, 23, 0}
};

void
FLStore::DNDStore(void) {
	int i, inventory_limit;
	inventory_limit = 0;
	dnditm = 0;
	fl_clear_and_reset_screen();
	fl_display_message("Welcome to the Larn Thrift Shoppe. We stock many items explorers find useful\n");
	fl_display_message("in their adventures. Feel free to browse to your hearts content.\n");
	if (outstanding_taxes > 0) {
		fl_display_message
		("\n\nThe Larn Revenue Service has ordered us to not do business with tax evaders.\n");
		lprintf
		("They have also told us that you owe %d gp in back taxes, and as we must\n", outstanding_taxes);
		fl_display_message
		("comply with the law, we cannot serve you at this time.  Soo Sorry.\n");
		fl_termcap_cursor_position(1, 24);
		fl_display_message("\nPress ");
		fl_invert_color_space("escape");
		fl_display_message(" to leave: ");
		fl_output_buffer_flush();
		i = 0;
		while (i != '\33') {
			i = ttgetch();
		}
		drawscreen();
		return;
	}
	for (i = dnditm; i < MAXDNDSIZE + dnditm; i++) {
		fl_dnd_item_list(i);
	}
	for (;;) {
		fl_termcap_cursor_position(1, 19);
		fl_display_message("You have ");
		lprintf("%ld ", cdesc[FL_GOLD]);
		lprintf("gold pieces");
		FL_CLEAR_TO_END_OF_LINE();
		fl_clear_screen_from_to_end_of_display(1, 20);
		fl_display_message("\nEnter your transaction [");
		fl_invert_color_space("space");
		fl_display_message(" for more, ");
		fl_invert_color_space("escape");
		fl_display_message(" to leave]? ");
		i = 0;
		while ((i < 'a' || i > 'z') && (i != ' ') && (i != '\33')
		        && (i != MAXINVEN)) {
			i = ttgetch();
		}
		if (i == MAXINVEN) {
			fl_clear_and_reset_screen();
			fl_display_message("Welcome to the Larn Thrift Shoppe. We stock many items explorers find useful\n");
			fl_display_message("in their adventures. Feel free to browse to your hearts content.\n");
			for (i = dnditm; i < MAXDNDSIZE + dnditm; i++) {
				fl_dnd_item_list(i);
			}
		} else if (i == '\33') {
			drawscreen();
			return;
		} else if (i == ' ') {
			fl_clear_screen_from_to_end_of_display(1, 4);
			if ((dnditm += MAXDNDSIZE) >= MAXITM) {
				dnditm = 0;
			}
			for (i = dnditm; i < MAXDNDSIZE + dnditm; i++) {
				fl_dnd_item_list(i);
			}
		} else {
			/* buy something */
			fl_output_single_byte_to_output_buffer((char) i);	/* echo the byte */
			i += dnditm - 'a';
			if (i >= MAXITM) {
				fl_display_message("\nSorry, but we are out of that object_identification.");
				fl_output_buffer_flush();
				fl_wait(FL_WAIT_DURATION);
			} else if (dnd_item[i].qty <= 0) {
				fl_display_message("\nSorry, but we are out of that object_identification.");
				fl_output_buffer_flush();
				fl_wait(FL_WAIT_DURATION);
			} else if (inventory_limit == MAXINVEN) {
				fl_display_message("\nYou can't carry anything more!");
				fl_output_buffer_flush();
				fl_wait(FL_WAIT_DURATION);
			} else if (cdesc[FL_GOLD] < (dnd_item[i].price) * 10L) {
				fl_display_message("\nYou don't have enough gold to pay for that!");
				fl_output_buffer_flush();
				fl_wait(2200);
			} else {
				if (dnd_item[i].obj == OPOTION) {
					potionname[dnd_item[i].arg][0] = ' ';
				} else if (dnd_item[i].obj == OSCROLL) {
					scrollname[dnd_item[i].arg][0] = ' ';
				}
				cdesc[FL_GOLD] -= dnd_item[i].price * 10;
				dnd_item[i].qty--;
				take(dnd_item[i].obj, dnd_item[i].arg);
				if (dnd_item[i].qty == 0) {
					fl_dnd_item_list(i);
				}
				fl_output_buffer_flush();
				fl_wait(FL_WAIT_DURATION);
			}
		}
	}
}

static void
fl_dnd_item_list(int i) {
	int j, k;
	int price;
	if (i >= MAXITM) {
		return;
	}
	fl_termcap_cursor_position((j = (i & 1) * 40 + 1), (k = ((i % MAXDNDSIZE) >> 1) + 5));
	if (dnd_item[i].qty == 0) {
		lprintf("%39s", "");
		return;
	}
	lprintf("%c) ", (i % MAXDNDSIZE) + 'a');
	if (dnd_item[i].obj == OPOTION) {
		fl_display_message("potion ");
		lprintf("%s", &potionname[dnd_item[i].arg][1]);
	} else if (dnd_item[i].obj == OSCROLL) {
		fl_display_message("scroll ");
		lprintf("%s", &scrollname[dnd_item[i].arg][1]);
	} else {
		lprintf("%s", objectname[dnd_item[i].obj]);
	}
	fl_termcap_cursor_position(j + 31, k);
	price = (dnd_item[i].price) * 10L;
	lprintf("%6ld", price);
}

/*
    for the first national bank of Larn
*/
int lasttime = 0;		/* last time he was in bank */

void
banktitle(const char *str) {
	FLStore Store;
	/* the reference to screen location for each gem */
	int gemorder[26];
	/* the appraisal of the gems */
	int gemvalue[26];
	int amt;
	int i, k, gems_sold = 0;
	fl_clear_and_reset_screen();
	fl_display_message(str);
	if (outstanding_taxes > 0) {
		fl_display_message
		("\n\nThe Larn Revenue Service has ordered that your account be frozen until all\n");
		lprintf
		("levied taxes have been paid.  They have also told us that you owe %d gp in\n", outstanding_taxes);
		fl_display_message
		("taxes, and we must comply with them. We cannot serve you at this time.  Sorry.\n");
		fl_display_message("We suggest you go to the LRS office and pay your taxes.\n");
		fl_termcap_cursor_position(1, 24);
		fl_display_message("\nPress ");
		fl_invert_color_space("escape");
		fl_display_message(" to leave: ");
		fl_output_buffer_flush();
		i = 0;
		while (i != '\33') {
			i = ttgetch();
		}
		drawscreen();
		return;
	}
	fl_display_message("\n\n\tGemstone\t      Appraisal\t\tGemstone\t      Appraisal");
	/* credit any needed interest */
	Store.InterestRate();
	for (k = i = 0; i < MAXDNDSIZE; i++)
		switch (iven[i]) {
			case OLARNEYE:
			case ODIAMOND:
			case OEMERALD:
			case ORUBY:
			case OSAPPHIRE:
				if (iven[i] == OLARNEYE) {
					gemvalue[i] = 250000 - ((gtime * 7) / 100) * 100;
					if (gemvalue[i] < 50000) {
						gemvalue[i] = 50000;
					}
				} else {
					gemvalue[i] = (255 & ivenarg[i]) * 100;
				}
				gemorder[i] = k;
				fl_termcap_cursor_position((k % 2) * 40 + 1, (k >> 1) + 4);
				lprintf("%c) %s", i + 'a', objectname[iven[i]]);
				fl_termcap_cursor_position((k % 2) * 40 + 33, (k >> 1) + 4);
				lprintf("%5d", (int) gemvalue[i]);
				k++;
				break;
			default:			/* make sure player can't sell non-existant gems */
				gemvalue[i] = 0;
				gemorder[i] = 0;
		};
	/* Cleaning up the awful UI here for the text. -Gibbon */
	fl_termcap_cursor_position(1, 13);
	lprintf("Account Summary:");
	fl_termcap_cursor_position(1, 15);
	lprintf("Gold in Bank Account");
	fl_termcap_cursor_position(1, 16);
	lprintf("%8d", cdesc[FL_BANK_ACCOUNT]);
	fl_termcap_cursor_position(1, 18);
	lprintf("Gold in inventory");
	fl_termcap_cursor_position(1, 19);
	lprintf("%8d", cdesc[FL_GOLD]);
	if (cdesc[FL_BANK_ACCOUNT] + cdesc[FL_GOLD] >= 500000)
		fl_display_message
		("\nNote:  Larndom law states that only deposits under 500,000gp  can earn interest.");
	for (;;) {
		fl_clear_screen_from_to_end_of_display(1, 20);
		fl_display_message("\nYour wish? [(");
		fl_invert_color_space("d");
		fl_display_message(") deposit, (");
		fl_invert_color_space("w");
		fl_display_message(") withdraw, (");
		fl_invert_color_space("s");
		fl_display_message(") sell a stone, or ");
		fl_invert_color_space("escape");
		fl_display_message("]  ");
		y_larn_rep = 0;
		i = 0;
		while (i != 'd' && i != 'w' && i != 's' && i != '\33') {
			i = ttgetch();
		}
		switch (i) {
			case 'd':
				fl_display_message("deposit\n");
				FL_CLEAR_TO_END_OF_LINE();
				fl_display_message("How much? ");
				amt = readnum(cdesc[FL_GOLD]);
				if (amt > cdesc[FL_GOLD]) {
					fl_display_message("You don't have that much.");
					fl_wait(FL_WAIT_DURATION);
				}
				/* Added if statement for catching 0 value to deposit. -Gibbon */
				if (amt == 0) {
					fl_display_message("You cannot deposit nothing");
					fl_wait(FL_WAIT_DURATION);
				} else {
					cdesc[FL_GOLD] -= amt;
					cdesc[FL_BANK_ACCOUNT] += amt;
				}
				break;
			case 'w':
				fl_display_message("withdraw\nHow much? ");
				amt = readnum(cdesc[FL_BANK_ACCOUNT]);
				if (amt > cdesc[FL_BANK_ACCOUNT]) {
					fl_display_message("\nYou don't have that much in the bank!");
					fl_wait(FL_WAIT_DURATION);
				} else {
					cdesc[FL_GOLD] += amt;
					cdesc[FL_BANK_ACCOUNT] -= amt;
				}
				break;
			case 's':
				fl_display_message("\nWhich stone would you like to sell? ");
				i = 0;
				while ((i < 'a' || i > 'z') && i != '*' && i != '\33') {
					i = ttgetch();
				}
				if (i == '*') {
					for (i = 0; i < MAXDNDSIZE; i++) {
						if (gemvalue[i]) {
							gems_sold = 1;
							cdesc[FL_GOLD] += gemvalue[i];
							iven[i] = 0;
							gemvalue[i] = 0;
							k = gemorder[i];
							fl_termcap_cursor_position((k % 2) * 40 + 1, (k >> 1) + 4);
							lprintf("%39s", "");
						}
					}
					if (!gems_sold) {
						fl_display_message("\nYou have no gems to sell!");
						fl_wait(FL_WAIT_DURATION);
					}
				} else if (i != '\33') {
					if (gemvalue[i = i - 'a'] == 0) {
						lprintf("\nItem %c is not a gemstone!", i + 'a');
						fl_wait(FL_WAIT_DURATION);
						break;
					}
					cdesc[FL_GOLD] += gemvalue[i];
					iven[i] = 0;
					gemvalue[i] = 0;
					k = gemorder[i];
					fl_termcap_cursor_position((k % 2) * 40 + 1, (k >> 1) + 4);
					lprintf("%39s", "");
				}
				break;
			case '\33':
				drawscreen();
				return;
		};
		/*Fix for #38 -Gibbon*/
		fl_termcap_cursor_position(1, 16);
		lprintf("%8d", cdesc[FL_BANK_ACCOUNT]);
		fl_termcap_cursor_position(1, 19);
		lprintf("%8d", cdesc[FL_GOLD]);
	}
	fl_clear_terminal();
	drawscreen();
}



/*
    function to put interest on your bank account
*/
void
FLStore::InterestRate(void) {
	int i;
	if (cdesc[FL_BANK_ACCOUNT] < 0) {
		cdesc[FL_BANK_ACCOUNT] = 0;
	} else if ((cdesc[FL_BANK_ACCOUNT] > 0)
	           && (cdesc[FL_BANK_ACCOUNT] < 500000)) {
		i = (gtime - lasttime) / 100;	/* # mobuls elapsed */
		while ((i-- > 0) && (cdesc[FL_BANK_ACCOUNT] < 500000)) {
			cdesc[FL_BANK_ACCOUNT] += cdesc[FL_BANK_ACCOUNT] / 250;
		}
		if (cdesc[FL_BANK_ACCOUNT] > 500000) {
			cdesc[FL_BANK_ACCOUNT] = 500000;  /* interest limit */
		}
	}
	lasttime = (gtime / 100) * 100;
}

static int
tradorder[12];	/* screen locations for trading post inventory */

static void
fl_show_trading_store_inventory(void) {
	int i, j;
	/* Print user's inventory like bank */
	for (j = i = 0; i < MAXINVEN; i++)
		if (iven[i]) {
			fl_termcap_cursor_position((j % 2) * 40 + 1, (j >> 1) + 8);
			tradorder[i] = 0;	/* init position on screen to zero */
			switch (iven[i]) {
				case OPOTION:
					if (potionname[ivenarg[i]][0] != 0) {
						tradorder[i] = j++;	/* will display only if identified */
						lprintf("%c) %s", i + 'a', objectname[iven[i]]);
						lprintf("%s", potionname[ivenarg[i]]);
					}
					break;
				case OSCROLL:
					if (scrollname[ivenarg[i]][0] != 0) {
						tradorder[i] = j++;	/* will display only if identified */
						lprintf("%c) %s", i + 'a', objectname[iven[i]]);
						lprintf("%s", scrollname[ivenarg[i]]);
					}
					break;
				case OLARNEYE:
				case OBOOK:
				case OPRAYERBOOK:
				case OSPIRITSCARAB:
				case ODIAMOND:
				case ORUBY:
				case OEMERALD:
				case OCHEST:
				case OSAPPHIRE:
				case OCUBEofUNDEAD:
				case OCOOKIE:
				case ONOTHEFT:
					tradorder[i] = j++;	/* put on screen */
					lprintf("%c) ", i + 'a');
					lprintf("%s", objectname[iven[i]]);
					break;
				default:
					tradorder[i] = j++;	/* put on screen */
					lprintf("%c) ", i + 'a');
					lprintf("%s", objectname[iven[i]]);
					if (ivenarg[i] > 0) {
						lprintf(" +%d", ivenarg[i]);
					} else if (ivenarg[i] < 0) {
						lprintf(" %d", ivenarg[i]);
					}
					break;
			}
		} else {
			tradorder[i] = 0;  /* make sure order array is clear */
		}
}



static void
fl_clear_trading_inventory(int i) {
	int j;
	j = tradorder[i];
	fl_termcap_cursor_position((j % 2) * 40 + 1, (j >> 1) + 8);
	lprintf("%39s", "");
	tradorder[i] = 0;
}



void
FLStore::TradingPost(void) {
	int i, j, isub, izarg, found;
	int value;
	fl_clear_and_reset_screen();
	enable_scroll = 0;
	dnditm = dndcount = 0;
	fl_display_message
	("Welcome to the Larn Trading Post.  We buy items that explorers no longer find\n");
	fl_display_message
	("useful.  Since the condition of the items you bring in is not certain,\n");
	fl_display_message
	("and we incur great expense in reconditioning the items, we usually pay\n");
	fl_display_message
	("only 20 percent of their value were they to be new.  If the items are badly\n");
	fl_display_message("damaged, we will pay only 10 percent of their new value.\n\n");
	fl_display_message("Here are the items we would be willing to buy from you:\n");
	fl_show_trading_store_inventory();
	FLStore Store;
	for (;;) {
		fl_termcap_cursor_position(1, 23);
		fl_display_message("\nWhat object do you want to sell to us [");
		fl_invert_color_space("escape");
		fl_display_message("] ? ");
		/* display gold in inventory ~Gibbon */
		fl_termcap_cursor_position(1, 22);
		Store.AmountGoldTrad();
		i = 0;
		while ((i > 'z' || i < 'a') && i != MAXINVEN && i != '\33') {
			i = ttgetch();
		}
		if (i == '\33') {
			fl_recalculate_armor_class();
			drawscreen();
			return;
		}
		for (;;) {		/* inner loop for simpler control */
			if (i == MAXINVEN) {
				fl_clear_and_reset_screen();
				enable_scroll = 0;
				fl_display_message
				("Welcome to the Larn Trading Post.  We buy items that explorers no longer find\n");
				fl_display_message
				("useful.  Since the condition of the items you bring in is not certain,\n");
				fl_display_message
				("and we incur great expense in reconditioning the items, we usually pay\n");
				fl_display_message
				("only 20 percent of their value were they to be new.  If the items are badly\n");
				fl_display_message("damaged, we will pay only 10 percent of their new value.\n\n");
				fl_display_message("Here are the items we would be willing to buy from you:\n");
				fl_show_trading_store_inventory();
				break;		/* leave inner while */
			}
			isub = i - 'a';
			if (iven[isub] == 0) {
				lprintf("\nYou don't have object %c!", isub + 'a');
				fl_wait(FL_WAIT_DURATION);
				break;		/* leave inner while */
			}
			if (iven[isub] == OSCROLL)
				if (scrollname[ivenarg[isub]][0] == 0) {
					fl_display_message("\nSorry, we can't accept unidentified objects.");
					fl_wait(2000);
					break;		/* leave inner while */
				}
			if (iven[isub] == OPOTION)
				if (potionname[ivenarg[isub]][0] == 0) {
					fl_display_message("\nSorry, we can't accept unidentified objects.");
					fl_wait(2000);
					break;		/* leave inner while */
				}
			if (iven[isub] == ODIAMOND ||
			        iven[isub] == ORUBY ||
			        iven[isub] == OEMERALD || iven[isub] == OSAPPHIRE) {
				value = 20L * (ivenarg[isub] & 255);
			} else if (iven[isub] == OLARNEYE) {
				value = 50000 - (((gtime * 7) / 100) * 20);
				if (value < 10000) {
					value = 10000;
				}
			} else {
				/* find object in dnd_item[] list for price info */
				found = MAXITM;
				for (j = 0; j < MAXITM; j++)
					if (dnd_item[j].obj == iven[isub]) {
						found = j;
						break;	/* leave for loop */
					}
				if (found == MAXITM) {
					fl_display_message
					("\nSo sorry, but we are not authorized to accept that object.");
					fl_wait(FL_WAIT_DURATION);
					break;	/* leave inner while */
				}
				if (iven[isub] == OSCROLL || iven[isub] == OPOTION) {
					value = 2 * dnd_item[j + ivenarg[isub]].price;
				} else {
					izarg = ivenarg[isub];
					value = dnd_item[j].price;
					/* appreciate if a +n object */
					if (izarg >= 0) {
						value *= 2;
					}
					while ((izarg-- > 0)
					        && ((value = 14 * (67 + value) / 10) < 500000));
				}
			}
			/*  we have now found the value of the object, and dealt with any error
			    cases.  Print the object's value, let the user sell it.
			*/
			lprintf
			("\nItem (%c) is worth %d gold pieces to us.  Do you want to sell it? ",
			 i, value);
			y_larn_rep = 0;
			if (getyn() == 'y') {
				fl_display_message("yes\n");
				cdesc[FL_GOLD] += value;
				if (cdesc[FL_WEAR] == isub) {
					cdesc[FL_WEAR] = -1;
				}
				if (cdesc[FL_WIELD] == isub) {
					cdesc[FL_WIELD] = -1;
				}
				if (cdesc[FL_SHIELD] == isub) {
					cdesc[FL_SHIELD] = -1;
				}
				adjustcvalues(iven[isub], ivenarg[isub]);
				iven[isub] = 0;
				fl_clear_trading_inventory(isub);
				/*  clear and display functions again so gold is re-calculated
				    part of feature request from hymie0. ~Gibbon */
				fl_clear_and_reset_screen();
				enable_scroll = 0;
				fl_display_message
				("Welcome to the Larn Trading Post.  We buy items that explorers no longer find\n");
				fl_display_message
				("useful.  Since the condition of the items you bring in is not certain,\n");
				fl_display_message
				("and we incur great expense in reconditioning the items, we usually pay\n");
				fl_display_message
				("only 20 percent of their value were they to be new.  If the items are badly\n");
				fl_display_message("damaged, we will pay only 10 percent of their new value.\n\n");
				fl_display_message("Here are the items we would be willing to buy from you:\n");
				fl_show_trading_store_inventory();
			} else {
				/* refresh screen when saying no ~Gibbon */
				fl_clear_and_reset_screen();
				enable_scroll = 0;
				fl_display_message
				("Welcome to the Larn Trading Post.  We buy items that explorers no longer find\n");
				fl_display_message
				("useful.  Since the condition of the items you bring in is not certain,\n");
				fl_display_message
				("and we incur great expense in reconditioning the items, we usually pay\n");
				fl_display_message
				("only 20 percent of their value were they to be new.  If the items are badly\n");
				fl_display_message("damaged, we will pay only 10 percent of their new value.\n\n");
				fl_display_message("Here are the items we would be willing to buy from you:\n");
				fl_show_trading_store_inventory();
			}
			break;		/* exit inner while */
		}			/* end of inner while */
	}				/* end of outer while */
}				/* end of routine */

/*
    I have cleaned this up and extended it with reusable functions.
    ~Gibbon
*/
void
FLStore::TaxOffice(void) {
	int i, first;
	int amt;
	FLStore Store;
	Store.LRSWelcomeText();
	/* disable signals */
	first = 1;
	for (;;) {
		if (first) {
			first = 0;
			fl_termcap_cursor_position(1, 6);
			if (outstanding_taxes > 0) {
				lprintf("You presently owe %d gp in taxes.", outstanding_taxes);
			} else {
				fl_display_message("You do not owe us any taxes.");
			}
			fl_termcap_cursor_position(1, 19);
			if (cdesc[FL_GOLD] > 0) {
				Store.AmountGoldTrad();
			} else {
				fl_display_message("You have no gold pieces.");
			}
		}
		enable_scroll = 1;
		fl_termcap_cursor_position(1, 21);
		fl_display_message("\n\nYour wish? [(");
		fl_invert_color_space("p");
		fl_display_message(") pay taxes, or ");
		fl_invert_color_space("escape");
		fl_display_message("]  ");
		y_larn_rep = 0;
		i = 0;
		while (i != 'p' && i != '\33') {
			i = ttgetch();
		}
		switch (i) {
			case 'p':
				fl_display_message("pay taxes\nHow much? ");
				amt = readnum(cdesc[FL_GOLD]);
				if (amt > cdesc[FL_GOLD]) {
					fl_display_message("  You don't have that much.\n");
				} else {
					/*Fix for bug #23 ~Gibbon*/
					cdesc[FL_GOLD] -= paytaxes(amt);
					Store.LRSWelcomeText();
				}
				break;
			case '\33':
				enable_scroll = 1;
				drawscreen();
				return;
		};
	}
}
