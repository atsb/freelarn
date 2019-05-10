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

void fl_init_inventory(void);
int showstr(char);
int fl_display_inventory(int, char);
int take(int, int);
int drop_object(int);

/* Allow only 12 items (a to l) in the player's inventory */
inline const int MAXINVEN = 12;

/*  The starting limit to the number of items the player can carry.
    The limit should probably be based on player strength and the
    weight of the items.
*/
inline const int MIN_LIMIT = 10;

/*  define a sentinel to place at the end of the sorted inventory.
    (speeds up display reads )
*/
inline const int END_SENTINEL = 255;
