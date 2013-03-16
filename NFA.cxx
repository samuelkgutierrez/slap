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

using namespace std;

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
bool
NFA::accepts(const AlphabetString &alphaString)
{
    /* XXX convert to DFA, minimize, run, return */
    FSMTransitionTable::iterator it;

    if (this->beVerbose) {
        for (it = this->transitionTable.begin();
             it != this->transitionTable.end();
             ++it) {
            cout << it->first << " "
                << it->second.getInput() << " --> "
                << it->second.getTo() << endl;
        }
    }
    return true;
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
            reachable.insert(it->second.getTo());
            copy.erase(it);
        }
    }
    return reachable;
}
