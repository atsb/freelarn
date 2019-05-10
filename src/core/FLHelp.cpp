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
#include "../player/FLPlayerActions.hpp"
#include "../templates/FLTMath.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../terminal/FLTerminal.hpp"
#include "FLTermCapIO.hpp"
#include "FLHelp.hpp"
#include "FLSystemDependant.hpp"

/*  I coded a nice line-reader for this.  Unfortunately it's very hard to get a good
    cursor and text position when reading a file from inside a curses program.
    so I did this instead, I actually prefer it. -Gibbon

*/

int
display_help_text(void) {
	fl_clear_and_reset_screen();
	fl_invert_color_space("Help File for Larn\n");
	lprintf("b  move southwest          B  run southwest            A  desecrate an altar\n");
	lprintf("c  cast a spell            C  close a door             Z  teleport yourself	\n");
	lprintf("d  drop an object            D  drink at a fountain      <  go up stairs or	\n");
	lprintf("e  eat something           E  enter a store, dungeon      volcanic shaft	\n");
	lprintf("g  get present pack weight                             >  go down stairs or	\n");
	lprintf("h  move left               H  run left                    volcanic shaft	\n");
	lprintf("i  inventory your pockets  I  list all known items							\n");
	lprintf("j  move down               J  run down                 ^  identify a trap	\n");
	lprintf("k  move up                 K  run up                   ,  pick up object		\n");
	lprintf("l  move right              L  run right                :  look at object you\n");
	lprintf("n  move southeast          N  run southeast               are standing on	\n");
	lprintf("                           O  open a door or chest     .  stay here			\n");
	lprintf("p  pray at an altar        P  give tax status          m  move without picking\n");
	lprintf("q  quaff a potion          Q  quit the game (suicide)     up an object		\n");
	lprintf("r  read a scroll           R  remove gems from throne  /  identify objects in\n");
	lprintf("s  sit on a throne         S  save the game               the game			\n");
	lprintf("t  tidy up at a fountain   T  take off armor  								\n");
	lprintf("u  move northeast          U  run northeast									\n");
	lprintf("v  print program version													\n");
	lprintf("w  wield a weapon          W  wear armor 									\n");
	lprintf("y  move northwest          Y  run northwest            ^L redraw the screen	\n");
	retcont();
	fl_clear_and_reset_screen();
	fl_invert_color_space("Special Notes\n");
	lprintf("When dropping gold, if you type '*' as your amount, all your gold gets dropped.\n");
	lprintf("In general, typing in '*' means all of what your interested in.  This is true\n");
	lprintf("when visiting the bank, or when contributing at altars.\n\n");
	lprintf("When in the store, trading post or home, an <escape> will get you out.\n\n");
	lprintf("When casting a spell, if you need a list of spells you can cast, type 'I' as\n");
	lprintf("the first letter of your spell.  The available list of spells will be shown,\n");
	lprintf("after which you may enter the spell code.  This only works on the 1st letter\n");
	lprintf("of the spell you are casting.\n\n");
	lprintf("When an inventory list is on the screen from a drop, quaff, read, or similar\n");
	lprintf("command, you can type the letter of the object that you wish to act apon,\n");
	lprintf("without having to type a space to get back to the prompt.\n\n");
	lprintf("If NumLock is off, the Keypad functions in the obvious way for movement. Hold\n");
	lprintf("Shift when pressing any direction on the Keypad to run in that direction. The\n");
	lprintf("5 key on the Keypad is the same as \"stay here\", which really means to\n");
	lprintf("skip your turn.");
	retcont();
	fl_clear_and_reset_screen();
	fl_invert_color_space("Explanation of the Larn scoreboard facility\n");
	lprintf("Larn supports TWO scoreboards, one for winners, and one for deceased\n");
	lprintf("characters.  Each player (by the name entered when you start the game)\n");
	lprintf("is allowed one slot on each scoreboard, if the score is in the top ten for\n");
	lprintf("that scoreboard.  This design helps insure that frequent players of Larn\n");
	lprintf("do not hog the scoreboard, and gives more players a chance for glory.\n\n");
	lprintf("Level of difficulty is also noted on the scoreboards, and this takes precedence\n");
	lprintf("over score for determining what entry is on the scoreboard.  For example:\n");
	lprintf("if \"Yar, the Bug Slayer\" has a score of 128003 on the scoreboard at diff 0,\n");
	lprintf("then a game at diff 1 and a score of 4112 would replace the previous\n");
	lprintf("entry on the scoreboard.  Note that when a player dies, the inventory is\n");
	lprintf("stored in the scoreboard so that everyone can see what items the player had\n");
	lprintf("at the time of death.");
	return_to_game();
	drawscreen();
	enable_scroll = 1;
	return (0);
}

/*
    function to display the welcome message and background
*/
void
welcome(void) {
	lprintf("Welcome to the game of Larn.\n\n  At this moment, you face a great problem.");
	lprintf("\nYour daughter has contracted a strange disease, and none of your");
	lprintf("\nhome remedies seem to have any effect. You sense that she is in");
	lprintf("\nmortal danger, and you must try to save her.");
	lprintf("\n\n  Time ago you heard of a land of great danger and opportunity.");
	lprintf("\nPerhaps here is the solution you need.");
	lprintf("\n\n  It has been said that there once was a great magician who called himself");
	lprintf("\nPolinneaus.  Many years ago after having many miraculous successes,");
	lprintf("\nPolinneaus retired to the caverns of Larn, where he devoted most of his");
	lprintf("\ntime to the creation of magic. Rumors have it that one day Polinneaus set");
	lprintf("\nout to dispel an attacking army in a forest some distance to the north.");
	lprintf("\nIt is believed that here he met his demise. The caverns of Larn it is");
	lprintf("\nthought, must be magnificent in design and contain much magic and treasure.");
	lprintf("\nOne option you have is to undertake a journey into these caverns in hope");
	lprintf("\nof finding a cure.");
	lprintf("\n");
	lprintf("\n\nGood Luck!  You're going to need it!");
	/* press return to continue */
	retcont();
}

/*
    function to say press return to continue and reset scroll when done
*/
void
retcont(void) {
	fl_termcap_cursor_position(1, 24);
	lprintf("Press ");
	fl_invert_color_space("return");
	lprintf(" to continue: ");
	while (ttgetch() != '\n');
	enable_scroll = 1;
}

/* Added for places where it points you back to the game. -Gibbon */
void
return_to_game(void) {
	fl_termcap_cursor_position(1, 24);
	lprintf("Press ");
	fl_invert_color_space("return");
	lprintf(" to go back to the game: ");
	while (ttgetch() != '\n');
	enable_scroll = 1;
}
