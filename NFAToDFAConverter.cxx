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
#include "Utils.hxx"

#include <iostream>
#include <stack>
#include <map>

using namespace std;

typedef set<StateSet> SOS;

/* ////////////////////////////////////////////////////////////////////////// */
static void
echoSet(const StateSet &target,
        int numSpaces = 5)
{
    StateSet::const_iterator it;
    string pad;
    bool first = true;

    for (int i = 0; i < numSpaces; ++i) {
        pad.append(" ");
    }
    cout << pad << "{";
    for (it = target.begin(); it != target.end(); ++it) {
        if (!first) {
            cout << ", ";
        }
        else {
            first = false;
        }
        cout << *it;
    }
    cout << "}";
}

/* ////////////////////////////////////////////////////////////////////////// */
static State
getNewState(void)
{
    static int stateName = 0;
    return State(Utils::int2string(stateName++));
}

/* ////////////////////////////////////////////////////////////////////////// */
/* returns set of nfa states reachable from nfa state T on an e-transition alone
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
                /* is this an epsilon transition to u and not in eClosure? */
                if (u->second.getInput() == AlphabetSymbol::epsilon() &&
                    eClosure.find(u->second.getTo()) == eClosure.end()) {
                    /* then add it to the eClosure */
                    eClosure.insert(u->second.getTo());
                    stateStack.push(u->second.getTo());
                }
            }
        }
    }
    if (this->beVerbose) {
        cout << "   X done constructing eClosure for" << endl;
        echoSet(T);
        cout << endl << "   X" << endl;
        cout << "   X eClosure is" << endl;
        echoSet(eClosure);
        cout << endl << "   X" << endl;
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
bool
NFAToDFAConverter::containsNFAFinalState(const StateSet &nfaFinal,
                                         const StateSet &ss,
                                         StateSet &commonFinalStates)
{
    for (StateSet::const_iterator state = ss.begin();
         ss.end() != state;
         ++state) {
        if (nfaFinal.end() != nfaFinal.find(*state)) {
            if (this->beVerbose) {
                cout << "   X common final: " << *state << endl;
            }
            commonFinalStates.insert(*state);
        }
    }
    if (!commonFinalStates.empty()) {
        return true;
    }
    else {
        return false;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
static StateSet
getStateSetFromTransTab(const FSMTransitionTable &tab)
{
    StateSet s;
    for (FSMTransitionTable::const_iterator it = tab.begin();
         it != tab.end();
         ++it) {
        s.insert(it->first);
        s.insert(it->second.getTo());
    }
    return s;
}

/* ////////////////////////////////////////////////////////////////////////// */
DFA
NFAToDFAConverter::getDFA(void)
{
    DFA dfa;
    SOS markedStates;
    SOS unmarkedStates;
    StateSet first;
    StateSet nfaInitial;
    State dfaInitial;
    map<StateSet, State> dfaStateNum;
    StateSet dfaFinalStates;

    AlphabetString alphabet(this->nfa.getAlphabet());
    StateSet nfaFinalStates(this->nfa.getAcceptStates());
    FSMTransitionTable dfaTransTab;

    if (this->beVerbose) {
        cout << endl <<
        "   X starting NFAToDFAConverter:subsetConstruct ######################"
            << endl;
    }

    nfaInitial.insert(this->nfa.getStartState());
    first = StateSet(eClosureT(nfaInitial));
    unmarkedStates.insert(first);

    dfaInitial = getNewState();
    dfaStateNum[first] = dfaInitial;
    dfa.setStart(dfaInitial);

    while (!unmarkedStates.empty()) {
        if (this->beVerbose) {
            cout <<
            "   X in main loop ################################################"
                << endl;
        }
        StateSet a = *unmarkedStates.begin();
        unmarkedStates.erase(unmarkedStates.begin());
        markedStates.insert(a);
        if (0 == a.size()) {
            continue;
        }
        StateSet commonFinal;
        if (containsNFAFinalState(nfaFinalStates, a, commonFinal)) {
            if (this->beVerbose) {
                cout << "   X found common final states" << endl;
                echoSet(commonFinal);
                cout << endl << "   X" << endl;
            }
            dfaFinalStates.insert(dfaStateNum[a]);
        }
        for (AlphabetString::iterator input = alphabet.begin();
             input != alphabet.end();
             ++input) {
            StateSet reachable =
                this->nfa.getStatesReachableByTransition(a, *input);
            if (this->beVerbose) {
                cout << "   X states reachable by " << *input << endl;
                cout << "   X (S)" << endl;
                echoSet(a);
                cout << endl << "   X" << endl;
                cout << "   X reachable" << endl;
                echoSet(reachable);
                cout << endl << "   X" << endl;
            }
            StateSet next = eClosureT(reachable);
            if (this->beVerbose) {
                cout << "   X next eClosure" << endl;
                echoSet(next);
                cout << endl << "   X" << endl;
            }
            if (unmarkedStates.end() == unmarkedStates.find(next) &&
                markedStates.end() == markedStates.find(next)) {
                unmarkedStates.insert(next);
                dfaStateNum[next] = getNewState();
            }
            if (this->beVerbose) {
                cout << "   X states merged: state " << dfaStateNum[a] << " is:"
                     << endl;
                echoSet(a);
                cout << endl << "   X" << endl;
            }
            if (next.size() == 0) {
                continue;
            }
            else { 
                dfaTransTab.insert(make_pair(dfaStateNum[a],
                                             FSMTransition(*input,
                                                           dfaStateNum[next])));
            }
        }
    }
    if (this->beVerbose) {
        cout << "   X dfa start state: " << endl << "     " << dfaInitial
             << endl << "   X" << endl;
        cout << "   X dfa accept states: "<< endl;
        echoSet(dfaFinalStates);
        cout << endl << "   X" << endl;
        cout << "   X dfa transition table: " << endl;
        for (FSMTransitionTable::iterator it = dfaTransTab.begin();
            it != dfaTransTab.end();
            ++it) {
            cout << "     " << it->first << " " << it->second.getInput() << " --> "
                 << it->second.getTo() << endl;
        }
        cout << "   X" << endl;
    }

    if (this->beVerbose) {
        cout <<
        "   X done with NFAToDFAConverter:subsetConstruct #####################"
            << endl;
    }

    StateSet dfaAllStates = getStateSetFromTransTab(dfaTransTab);
    dfa = DFA(alphabet, dfaTransTab, dfaAllStates, dfaInitial, dfaFinalStates);

    return dfa;
}

/* ////////////////////////////////////////////////////////////////////////// */
void
NFAToDFAConverter::verbose(bool beVerbose)
{
    this->beVerbose = beVerbose;
}
