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

#include <cstdlib>
using namespace std;

template<typename T>
inline T TRnd(const T& x) {
	return static_cast<T>(rand() % (x)) + 1;
}
template<typename T>
inline T TRund(const T& x) {
	return static_cast<T>(rand() % (x));
}
template<typename T>
inline T TMathMin(const T& x, const T& y) {
	return x > y ? y : x;
}
template<typename T>
inline T TMathMax(const T& x, const T& y) {
	return x > y ? x : y;
}
