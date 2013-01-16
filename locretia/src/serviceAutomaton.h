/*****************************************************************************\
 Locretia -- generating logs...

 Copyright (c) 2012 Simon Heiden

 Locretia is free software: you can redistribute it and/or modify it under the
 terms of the GNU Affero General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 Locretia is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for
 more details.

 You should have received a copy of the GNU Affero General Public License
 along with Locretia.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#pragma once

#include <pnapi/pnapi.h>


/*!
 \brief service automaton

 All service automaton related functions and attributes.
*/
class serviceAutomaton {

    public: /* static functions */
		static int isFinalStateReachable(const int trace_max_length);

    public: /* static attributes */
        /// the open net that created these inner markings
        static pnapi::Automaton* sa;

    private: /* static attributes */

    public: /* member functions */

    private: /* member functions */
        static int traverse(const pnapi::State& state, int depth, int counter);

    public: /* member attributes */
};

