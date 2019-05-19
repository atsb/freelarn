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

extern char *fl_buffer_pointer;
extern int regen_bottom;

/* Turn on bold display for the terminal */
template<typename T>
T TSetTextBold(const T& x) {
	return *fl_buffer_pointer++ = x;
}
/* Turn off bold display for the terminal */
template<typename T>
T TResetTextBold(const T& x) {
	return *fl_buffer_pointer++ = x;
}
template<typename T>
T TClearAndResetScreen(const T& x) {
	return *fl_buffer_pointer++ = x, regen_bottom;
}
template<typename T>
T TClearToEOL(const T& x) {
	return *fl_buffer_pointer++ = x;
}
