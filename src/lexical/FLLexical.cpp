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

#if defined NIX || NIX_LOCAL
	#include <fcntl.h>
#endif

#include <ctype.h>
#include <sys/types.h>
#include "../config/FLIncludeData.hpp"
#include "../templates/FLTMath.hpp"
#include "FLTok.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../core/FLTermCapIO.hpp"
#include "../core/FLScores.hpp"

static char larn_final_k = 0;
int y_larn_rep = 0;
int move_no_pickup = 0;

/*
    lexical analyzer
*/
int
yylex(void) {
	char cc;
	char first_time = 1;
	if (hit2flag) {
		hit2flag = 0;
		y_larn_rep = 0;
		return (' ');
	}
	if (y_larn_rep > 0) {
		--y_larn_rep;
		return (larn_final_k);
	} else {
		y_larn_rep = 0;
	}
	if (y_larn_rep == 0) {
		bottomdo();
		showplayer();		/* show where the player is */
		move_no_pickup = 0;	/* clear 'm' flag */
	}
	fl_output_buffer_flush();
	for (;;) {
		cdesc[FL_BYTES_IN]++;
		cc = ttgetch();
		/*  get repeat count, showing to player
		*/
		if ((cc <= '9') && (cc >= '0')) {
			y_larn_rep = y_larn_rep * 10 + cc - '0';
			/*  show count to player for feedback
			*/
			if (y_larn_rep >= 10) {
				fl_termcap_cursor_position(1, 24);
				if (first_time) {
					fl_display_message("\n");
				}
				lprintf("count: %d", (int) y_larn_rep);
				first_time = 0;
				fl_output_buffer_flush();	/* show count */
			}
		} else {
			/*  check for multi-character commands and handle.
			*/
			if (cc == 'm') {
				move_no_pickup = 1;
				cc = ttgetch();
			}
			if (y_larn_rep > 0) {
				--y_larn_rep;
			}
			return (larn_final_k = cc);
		}
	}
}
