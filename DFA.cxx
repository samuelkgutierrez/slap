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

#include "DFA.hxx"
#include "HopcroftDFAMinimizer.hxx"

#include <iostream>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
DFA::DFA(const AlphabetString &alpha,
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
DFA::DFA(const DFA &other) :
     FiniteStateMachine(other)
{
     ;
}

/* ////////////////////////////////////////////////////////////////////////// */
State
DFA::move(FSMTransitionTable transTabCopy,
          const State &cur,
          const AlphabetSymbol &in)
{
    FSMTransitionTable::iterator it;

    while (transTabCopy.end() != (it = transTabCopy.find(cur))) {
        if (in == it->second.getInput()) {
            break;
        }
        /* else delete the entry and try again */
        transTabCopy.erase(it);
    }
    /* didn't find a valid transition */
    if (it == transTabCopy.end()) {
        return State::StateInvalid();
    }
    if (this->beVerbose) {
        cout << cur << " "
             << it->second.getInput() << " --> "
             << it->second.getTo() << endl;
    }
    /* return next state */
    return it->second.getTo();
}

/* ////////////////////////////////////////////////////////////////////////// */
DFA *
DFA::minimize(void)
{
    return HopcroftDFAMinimizer::minimize(*this);
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
DFA::accepts(const AlphabetString &alphaString)
{
    AlphabetString::const_iterator it;
    State cur = this->startState;
    State invalidState = State::StateInvalid();

    for (it = alphaString.begin(); it != alphaString.end(); ++it) {
        if (invalidState == cur) {
            return false;
        }
        cur = move(this->transitionTable, cur, *it);
    }
    /* if we are here, then return if the current state is an accept state */
    return this->acceptState(cur);
}


