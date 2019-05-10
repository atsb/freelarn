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

class FLStore {
	public:
		int AmountGoldTrad(void);
		int LRSWelcomeText(void);
		void DNDStore(void);
		void InterestRate(void);
		void TradingPost(void);
		void TaxOffice(void);
};

void banktitle(const char*);
/* number of items in the dnd inventory table */
inline const int MAXITM = 83;
/* number of items shown in the store */
inline const int MAXDNDSIZE = 26;
