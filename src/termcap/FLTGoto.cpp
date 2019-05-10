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

#include <cstdio>
#include <string.h>
#include "FLTermcap.hpp"

static const char *in;		/* Internal copy of input string pointer */
static char *rp;			/* Pointer to ret array */
static int args[MAXARGS];	/* Maximum number of args to convert */
static int numval;			/* Count of args processed */
static char ret[24];		/* Converted string */

const char *
Termcap::atgoto(const char *cm, int destcol, int destline) {
	in = cm;
	rp = ret;
	args[0] = destline;
	args[1] = destcol;
	numval = 0;
	while (*in != '\0') {
		if (*in != '%') {
			*rp++ = *in++;
		} else {
			term_conversion();
		}
	}
	*rp = '\0';
	return (ret);
}

void
term_conversion(void) {
	int incr = 0;
	int temp;
	in++;
	switch (*in++) {
		case 'd':
			std::sprintf(rp, "%d", args[numval++]);
			numval = (numval + incr) % 1000;
			rp = &ret[strnlen(ret, 24)];
			if (numval > 99) {
				*rp++ = '0' + (numval / 100);
			}
			if (numval > 9) {
				*rp++ = '0' + (numval / 10) % 10;
			}
			*rp++ = '0' + (numval % 10);
			break;
		case '2':
			std::sprintf(rp, "%02d", args[numval++]);
			numval = (numval + incr) % 100;
			rp += 2;
			break;
		case '+':
			*rp++ = args[numval++] + *in++;
			break;
		case 'r':
			temp = args[numval];
			args[numval] = args[numval + 1];
			args[numval + 1] = temp;
			break;
		case 'i':
			args[numval]++;
			args[numval + 1]++;
			break;
		case '%':
			*rp++ = '%';
			break;
	}
}

