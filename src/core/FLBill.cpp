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

#include "FLBill.hpp"
#include "FLHelp.hpp"
#include "../config/FLIncludeData.hpp"
#include "../templates/FLTMath.hpp"
#include "FLTermCapIO.hpp"

static int junk_mail1(int);
static int junk_mail2(void);
static int junk_mail3(void);
static int junk_mail4(void);

/*  I will start to change these mail messages.  Some of them can be changed,
	to make them more funny and interesting.  Right now, they are kinda boring and uninteresting.
    -Gibbon
*/

/*
    function to create the tax bill for the user
*/
static int
junk_mail1(int gold_count) {
	enable_scroll = 0;
	fl_clear_and_reset_screen();
	fl_invert_color_space("From:");
	fl_display_message("the LRS");
	fl_invert_color_space("\nSubject:");
	fl_display_message("Income\n");
	fl_display_message("\nNews has come to us that you have indeed survived the caverns!");
	fl_display_message("\nCongratulations..");
	fl_display_message("\n\nWe received news that you have received");
	lprintf("\n%d Gold Coins.", gold_count);
	fl_display_message("\nAs you been_here_before, Larn is in economic trouble, thus we have prepared your tax bill.");
	lprintf("\nYou owe %d Gold Coins.", gold_count * TAXRATE);
	fl_display_message("\nPlease pay within 5 days or there will be consequences.\n");
	retcont();
	return (1);
}

static int
junk_mail2(void) {
	enable_scroll = 0;
	fl_clear_and_reset_screen();
	fl_invert_color_space("From:");
	fl_display_message("Monk Chilfred\n");
	fl_invert_color_space("\nSubject:");
	fl_display_message("I have heard of your astounding adventure\n");
	fl_display_message("Would you like to take up my offer of another?\n");
	fl_display_message("I have news of a faraway land which has dangers and riches.\n");
	fl_display_message("We have news that our friend Bazar the Ronin has met his fate.\n");
	fl_display_message("We would be grateful if you could locate his bones,\n");
	fl_display_message("he went to the caves of n'hak where he was\n");
	fl_display_message("killed by a giant bat while sleeping..\n\n");
	fl_display_message("If you take us up on this offer, you will be rewarded.\n");
	retcont();
	return (1);
}

static int
junk_mail3(void) {
	enable_scroll = 0;
	fl_clear_and_reset_screen();
	fl_invert_color_space("From:");
	fl_display_message("Count Englewond of Chi\n");
	fl_invert_color_space("\nSubject:");
	fl_display_message("!!!!\n");
	fl_display_message("\nI heard of your journey. Congratulations!");
	fl_display_message("\nWith several attempts I have yet to endure the");
	fl_display_message("caverns of Larn,\nand you, a nobody, makes the journey! From this time");
	fl_display_message("onward, bewarned that.....argh this darn keyboa..\n");
	retcont();
	return (1);
}

static int
junk_mail4(void) {
	enable_scroll = 0;
	fl_clear_and_reset_screen();
	fl_invert_color_space("From:");
	fl_display_message("Hakaz the Elderly\n");
	fl_invert_color_space("\nSubject:");
	fl_display_message("Adomonis\n");
	fl_display_message("\nA terrible plague is sweeping Larn and people are dying a terrible death!");
	fl_display_message("\nI beseech thee to help save souls..");
	fl_display_message("\nI was an adventurer like yourself, saving the world from dangers.");
	fl_display_message("\nAny help you can spare will be appreciated.\n");
	retcont();
	return (1);
}

/* Entry point for junk mail. ~Gibbon */
void
larn_read_junk_mail(int gold_count) {
	junk_mail1(gold_count);
	junk_mail2();
	junk_mail3();
	junk_mail4();
}
