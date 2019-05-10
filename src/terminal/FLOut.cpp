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

#include "FLOut.hpp"

void
ansiterm_out(const char *output_buffer, int n_chars) {
	int i;
	i = 0;
	while (i < n_chars) {
		if (output_buffer[i] == ANSITERM_ESC) {
			char ansi_param[10];
			char param1[5];
			char param2[5];
			int ansi_cmd;
			int j;
			*param1 = '\0';
			*param2 = '\0';
			++i;
			++i;
			j = 0;
			while (!isalpha(output_buffer[i])) {
				ansi_param[j] = output_buffer[i];
				++i;
				++j;
			}
			ansi_param[j] = '\0';
			ansi_cmd = output_buffer[i];
			++i;
			if (*ansi_param != '\0') {
				char *p;
				int k;
				p = ansi_param;
				k = 0;
				while (*p != ';' && *p != '\0') {
					param1[k] = *p;
					++p;
					++k;
				}
				param1[k] = '\0';
				if (*p == ';') {
					++p;
				}
				k = 0;
				while (*p != ';' && *p != '\0') {
					param2[k] = *p;
					++p;
					++k;
				}
				param2[k] = '\0';
			}
			ansiterm_command(ansi_cmd, param1, param2);
		} else {
			ansiterm_putchar(output_buffer[i]);
			++i;
		}
	}
	llrefresh();
}

