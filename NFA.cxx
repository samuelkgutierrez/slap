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

#include "NFA.hxx"
#include "NFAToDFAConverter.hxx"
#include "Utils.hxx"

#include <algorithm>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
static State
getNewState(void)
{
    static int stateName = 0;
    return State(Utils::int2string(stateName++));
}

/* ////////////////////////////////////////////////////////////////////////// */
NFA::NFA(const AlphabetString &alpha,
         const FSMTransitionTable &transitionTab,
         const StateSet &allStates,
         const State &startState,
         const StateSet &acceptStates) :
     FiniteStateMachine(alpha,
                        transitionTab,
                        allStates,
                        startState,
                        acceptStates)
{
    ;
}

/* ////////////////////////////////////////////////////////////////////////// */
NFA::NFA(const NFA &other) :
     FiniteStateMachine(other)
{
     ;
}

/* ////////////////////////////////////////////////////////////////////////// */
NFA::NFA(const AlphabetSymbol &input)
{
    State start = getNewState();
    State accept = getNewState();

    if (this->beVerbose) {
        cout << "   N creating new: " << start << " " << input << " --> "
             << accept << " transition" << endl;
    }
    if (this->alphabet.end() == find(this->alphabet.begin(),
                                     this->alphabet.end(), input)) {
        this->alphabet.push_back(input);
    }
    this->startState = start;
    this->acceptStates.insert(accept);
    this->transitionTable.insert(make_pair(start,
                                           FSMTransition(input, accept)));
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
NFA::accepts(const AlphabetString &alphaString)
{
    DFA dfa;
    NFAToDFAConverter converter(*this);
    converter.verbose(this->beVerbose);
    dfa = converter.getDFA();
    dfa.verbose(this->beVerbose);

    return dfa.accepts(alphaString);
}

/* ////////////////////////////////////////////////////////////////////////// */
StateSet
NFA::getStatesReachableByTransition(const StateSet &s,
                                    const AlphabetSymbol &input) const
{
    StateSet reachable;
    FSMTransitionTable copy = this->transitionTable;
    FSMTransitionTable::iterator it;

    for (StateSet::const_iterator state = s.begin();
         state != s.end();
         ++state) {
        while (copy.end() != (it = copy.find(*state))) {
            if (input == it->second.getInput()) {
                reachable.insert(it->second.getTo());
            }
            copy.erase(it);
        }
    }
    return reachable;
}
