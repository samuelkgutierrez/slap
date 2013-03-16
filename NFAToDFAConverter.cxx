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

#include "NFAToDFAConverter.hxx"
#include "State.hxx"
#include "FSMTransition.hxx"
#include "AlphabetSymbol.hxx"

#include <iostream>
#include <stack>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
/* returns set of nfa states reachable from nfa state S on an e-transition alone
 */
StateSet
NFAToDFAConverter::eClosureT(StateSet T)
{
    State state;
    stack<State> stateStack;
    StateSet eClosure = T;
    FSMTransitionTable transTab = this->nfa.getTransitionTable();
    FSMTransitionTable::iterator u;

    for (StateSet::iterator t = T.begin(); t != T.end(); ++t) {
        stateStack.push(*t);
    }
    while (!stateStack.empty()) {
        state = stateStack.top();
        stateStack.pop();
        /* for each state u with an epsilon transition from t to u */ 
        /* that is: t 'e --> u */
        for (u = transTab.begin(); u != transTab.end(); ++u) {
            /* is this the state that we care about? that is: u->first == t? */
            if (u->first == state) {
                /* is this an epsilon transition and not in eClosure? */
                if (u->second.getInput() == AlphabetSymbol::epsilon() &&
                    eClosure.find(u->second.getTo()) == eClosure.end()) {
                    /* then add it to the eClosure */
                    eClosure.insert(u->second.getTo());
                }
            }
        }
    }



    return eClosure;
}

/* ////////////////////////////////////////////////////////////////////////// */
NFAToDFAConverter::NFAToDFAConverter(const NFA &nfa)
{
    this->nfa = nfa;
    this->beVerbose = false;
}

/* ////////////////////////////////////////////////////////////////////////// */
DFA
NFAToDFAConverter::getDFA(void)
{
    DFA dfa;
    StateSet eReachableStates;

    eReachableStates = eClosureT(this->nfa.getAllStates());

    return dfa;
}

/* ////////////////////////////////////////////////////////////////////////// */
void
NFAToDFAConverter::verbose(bool beVerbose)
{
    this->beVerbose = beVerbose;
}
