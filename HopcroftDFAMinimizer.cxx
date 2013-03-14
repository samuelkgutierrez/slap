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

#include "HopcroftDFAMinimizer.hxx"
#include "State.hxx"
#include "FSMTransition.hxx"

#include <iostream>
#include <algorithm>
#include <set>

/* set of sets --- help! */
#define SoS set<StateSet>

using namespace std;

/**
 * a dfa minimizer that uses Hopcroft's algorithm.
 */

bool verbose = false;

/* ////////////////////////////////////////////////////////////////////////// */
static void
echoSet(StateSet *target)
{
    StateSet::iterator it;

    for (it = target->begin(); it != target->end(); ++it) {
        cout << *it << endl;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
echoTransitionSet(set<FSMTransition> t)
{
    set<FSMTransition>::iterator it;

    for (it = t.begin(); it != t.end(); ++it) {
        FSMTransition tmp = *it;
        cout << tmp.getInput() << " " << tmp.getTo() << endl;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
static bool
statesConsistent(FSMTransitionTable *transTab,
                 State s1,
                 State s2)
{
    FSMTransitionTable tt;
    FSMTransitionTable::iterator it;
    set<FSMTransition> set1, set2;
    bool consistent = false;

    tt = FSMTransitionTable(*transTab);
    while (tt.end() != (it = tt.find(s1))) {
        set1.insert(it->second);
        tt.erase(it);
    }
    tt = FSMTransitionTable(*transTab);
    while (tt.end() != (it = tt.find(s2))) {
        set2.insert(it->second);
        tt.erase(it);
    }
    consistent = set1 == set2;

    if (verbose) {
        cout << "### checking consistency ###" << endl;
        cout << "- " << s1 << endl;
        echoTransitionSet(set1);
        cout << "- " << s2 << endl;
        echoTransitionSet(set2);
        cout << "consistent: " << consistent << endl;
        cout << "############################" << endl;
    }

    return consistent;
}

/* ////////////////////////////////////////////////////////////////////////// */
static bool
groupConsistent(SoS g)
{
    return true;
}

/* ////////////////////////////////////////////////////////////////////////// */
static StateSet
getStatesThatCLeadsTo(FSMTransitionTable transTab,
                      const AlphabetSymbol &c)
{
    StateSet stateSet;
    FSMTransitionTable::iterator it;

    for (it = transTab.begin(); it != transTab.end(); ++it) {
        if (c == it->second.getInput()) {
            stateSet.insert(it->second.getTo());
        }
    }
    if (verbose) {
        cout << "### " << c << " leads to ###" << endl;
        echoSet(&stateSet);
        cout << "############################" << endl;
    }
    return stateSet;
}

/* ////////////////////////////////////////////////////////////////////////// */
StateSet
getStateIntersection(StateSet s1,
                     StateSet s2)
{
    StateSet inter;

    set_intersection(s1.begin(),
                     s1.end(),
                     s2.begin(),
                     s2.end(),
                     inserter(inter, inter.end()));
    return inter;
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
go(AlphabetString alphabet,
   FSMTransitionTable *transTab,
   StateSet *sf,
   StateSet *f)
{
    AlphabetString::iterator alphaIt;

    // while w is not empty
    for (alphaIt = alphabet.begin(); alphaIt != alphabet.end(); ++alphaIt) {
        StateSet X = getStatesThatCLeadsTo(*transTab, *alphaIt);
    }
#if 0
    AlphabetSymbol a("a");
    AlphabetSymbol b("a");
    StateSet X = getStatesThatCLeadsTo(*transTab, a);
    StateSet Y = getStatesThatCLeadsTo(*transTab, b);
    StateSet Z = getStateIntersection(X, Y);
    echoSet(&Z);
    cout << "empty: " << Z.empty() << endl;
#endif

}

/* ////////////////////////////////////////////////////////////////////////// */
DFA *
HopcroftDFAMinimizer::minimize(DFA &targetDFA,
                               bool beVerbose)
{
    /* set some global state */
    verbose = beVerbose;
    /* our alphabet */
    AlphabetString alphabet = targetDFA.getAlphabet();
    /* construct a new transition table that we can play with */
    FSMTransitionTable *transitionTable = targetDFA.getNewTransitionTable();
    /* first state with two sets: final states and everything else */
    StateSet *allStates = targetDFA.getNewAllStates();
    /* all final states */
    StateSet *finalStates = targetDFA.getNewAcceptStates();
    /* all states that are not accept states: S - F */
    StateSet *sf = new StateSet();

    set_difference(allStates->begin(),
                   allStates->end(),
                   finalStates->begin(),
                   finalStates->end(),
                   inserter(*sf, sf->end()));

    if (verbose) {
        cout << "### starting DFA minimization ###" << endl;

        cout << "### starting with ###" << endl;
        cout << "--- all states (S) ---" << endl;
        echoSet(allStates);

        cout << "--- accept states (F) ---" << endl;
        echoSet(finalStates);

        cout << "--- S - F ---" << endl;
        echoSet(sf);
        cout << "#####################" << endl;
    }

    go(alphabet, transitionTable, sf, finalStates);

    /* XXX */
    return new DFA(targetDFA);
}
