/**
 * Copyright (c) 2013 Samuel K. Gutierrez All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NFA_TO_DFACONVERTER_INCLUDED
#define NFA_TO_DFACONVERTER_INCLUDED

#include "State.hxx"
#include "DFA.hxx"
#include "NFA.hxx"

#include <string>

class NFAToDFAConverter {
private:
    bool beVerbose;

    NFA nfa;

    StateSet eClosureT(StateSet T);

     bool containsNFAFinalState(const StateSet &nfaFinal,
                                const StateSet &ss,
                                StateSet &commonFinalStates);

protected:
public:
    NFAToDFAConverter(void) { }

    ~NFAToDFAConverter(void) { }

    NFAToDFAConverter(const NFA &nfa);

    DFA getDFA(void);

    void verbose(bool beVerbose);
};

#endif
