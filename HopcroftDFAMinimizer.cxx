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

#include "Constants.hxx"
#include "HopcroftDFAMinimizer.hxx"
#include "State.hxx"
#include "FSMTransition.hxx"
#include "SLAPException.hxx"
#include "Utils.hxx"

#include <iostream>
#include <algorithm>
#include <set>
#include <map>

/* set of sets --- help! */
#define SOS set<StateSet>

using namespace std;

/**
 * a dfa minimizer that uses Hopcroft's algorithm.
 */

/**
 * notes:
 * see: p. 180
 */

static bool verbose = false;

/* ////////////////////////////////////////////////////////////////////////// */
static void
echoSet(const StateSet &target,
        int numSpaces = 5)
{
    StateSet::const_iterator it;
    string pad;

    for (int i = 0; i < numSpaces; ++i) {
        pad.append(" ");
    }
    for (it = target.begin(); it != target.end(); ++it) {
        cout << pad << *it << endl;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
static
StateSet
getStateIntersection(const StateSet &s1,
                     const StateSet &s2)
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
static
StateSet
getStateDifference(const StateSet &s1,
                   const StateSet &s2)
{
    StateSet diff;

    set_difference(s1.begin(),
                   s1.end(),
                   s2.begin(),
                   s2.end(),
                   inserter(diff, diff.end()));
    return diff;
}

/* ////////////////////////////////////////////////////////////////////////// */
static StateSet
getStatesWhereCLeadsToA(const FSMTransitionTable &transTab,
                        const AlphabetSymbol &c,
                        const StateSet &a)
{
    StateSet stateSet;
    FSMTransitionTable::const_iterator it;
    StateSet::iterator sit;

    if (verbose) {
        cout << "   $ finding valid delta(s_i, " << c << ") "
             << endl;
    }
    for (sit = a.begin(); sit != a.end(); ++sit) {
        State s = *sit;
        for (it = transTab.begin(); it != transTab.end(); ++it) {
            if (c == it->second.getInput() && s == it->second.getTo()) {
                if (verbose) {
                    cout << "   $ " << it->first << " "
                         << it->second.getInput() << " --> "
                         << it->second.getTo() << endl;
                }
                stateSet.insert(it->first);
            }
        }
    }
    if (verbose) {
        cout << "   $" << endl;
    }
    return stateSet;
}

/* ////////////////////////////////////////////////////////////////////////// */
static State
getNewState(void)
{
    static int stateName = 0;
    return State(Utils::int2string(stateName++));
}

/* ////////////////////////////////////////////////////////////////////////// */
static StateSet
getStateSetStateIn(const SOS &group,
                   const State &state)
{
    StateSet empty;
    for (SOS::const_iterator git = group.begin(); group.end() != git; ++git) {
        StateSet ss = *git;
        if (ss.end() != ss.find(state)) {
            return ss;
        }
    }
    return empty;
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
echoTransitions(const FSMTransitionTable &transTab)
{
    for (FSMTransitionTable::const_iterator i = transTab.begin();
         transTab.end() != i;
         ++i) {
        cout << "   $ " << i->first << " " << i->second.getInput() << " --> "
             << i->second.getTo() << endl;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
static bool
stateHasTransition(const State &s,
                   const FSMTransition &trans,
                   const FSMTransitionTable &tt)
{
    FSMTransitionTable tcopy = tt;
    FSMTransitionTable::iterator t;

    while (tcopy.end() != (t = tcopy.find(s))) {
        if (t->second.getInput() == trans.getInput() &&
            t->second.getTo() == trans.getTo()) {
            return true;
        }
        tcopy.erase(t);
    }
    return false;
}

/* ////////////////////////////////////////////////////////////////////////// */
static DFA
merge(const SOS &P,
      const AlphabetString &alphabet,
      const FSMTransitionTable &transTab,
      const State &start,
      const StateSet &acceptStates)
{
    StateSet newStates;
    FSMTransitionTable newTransTab;
    /* map from equiv class set to new state that represents old set */
    map<StateSet, State> oldToNewMap;
    DFA minDFA;
    StateSet newAccepts;

    if (verbose) {
        cout << endl <<
        "   $ HopcroftDFAMinimizer: starting equivalence class merge $$$$$$$$$$"
             << endl;
        cout << "   $ creating " << P.size() << " new start states..." << endl;
    }
    /* create the new states */
    for (SOS::const_iterator S = P.begin(); P.end() != S; ++S) {
        State newState = getNewState();
        newStates.insert(newState);
        /* create the mapping from old set to new state name */
        oldToNewMap[*S] = newState;
    }
    if (verbose) {
        cout << "   $ creating new transition table" << endl;
    }
    /* for each old transition */
    for (FSMTransitionTable::const_iterator t = transTab.begin();
         transTab.end() != t;
         ++t) {
        State newFrom = oldToNewMap[getStateSetStateIn(P, t->first)];
        State newTo = oldToNewMap[getStateSetStateIn(P, t->second.getTo())];
        AlphabetSymbol input = t->second.getInput();
        FSMTransition newTransition = FSMTransition(input, newTo);
        /* this is really ugly... fix later -- poor performance */
        if (!stateHasTransition(newFrom, newTransition, newTransTab)) {
            newTransTab.insert(make_pair(newFrom, newTransition));
        }
    }
    if (verbose) {
        cout << "   $ done creating new transition table and here it is"
             << endl;
        echoTransitions(newTransTab);
        cout << "   $" << endl;
        cout << endl <<
        "   $ HopcroftDFAMinimizer: done with equivalence class merge $$$$$$$$$"
             << endl;
    }

    State newStart = oldToNewMap[getStateSetStateIn(P, start)];

    for (StateSet::iterator a = acceptStates.begin();
         acceptStates.end() != a;
         ++a) {
        State newAccept = oldToNewMap[getStateSetStateIn(P, *a)];
        newAccepts.insert(newAccept);
    }
    return DFA(alphabet, newTransTab, newStates, newStart, newAccepts);
}

/* ////////////////////////////////////////////////////////////////////////// */
static DFA
go(AlphabetString alphabet,
   const FSMTransitionTable &transTab,
   const State &start,
   const StateSet &sf,
   const StateSet &f)
{
    AlphabetString::iterator alphaIt;
    SOS::iterator wIt, y;
    SOS p;
    SOS w;

    /* populate P and W */
    p.insert(f);
    p.insert(sf);
    w =  p;

    if (verbose) {
        cout << endl <<
        "   $ HopcroftDFAMinimizer: starting main loop $$$$$$$$$$$$$$$$$$$$$$$$"
             << endl;
    }

    while (!w.empty()) {
        /* select and remove S from W */
        wIt = w.begin();
        StateSet s = *wIt;
        w.erase(wIt);
        if (verbose) {
            cout << "   $ working set:" << endl;
            echoSet(s);
            cout << "   $" << endl;
        }
        /* for each c in ∑ do */
        for (alphaIt = alphabet.begin(); alphaIt != alphabet.end(); ++alphaIt) {
            /* let X be the set of states for which a
             * transition on c leads to a state in A */
            StateSet x = getStatesWhereCLeadsToA(transTab, *alphaIt, s);
            /* for each set Y in P */
            SOS t = p;
            for (y = p.begin(); y != p.end(); ++y) {
                StateSet yIx = getStateIntersection(*y, x);
                StateSet yMyIx = getStateDifference(*y, yIx);
                /* for which X ∩ Y is nonempty and not in P */
                if (!yIx.empty() && t.find(yIx) == t.end()) {
                    /* replace Y in P by the two sets X ∩ Y and Y \ X */
                    t.insert(yIx);
                    t.insert(yMyIx);
                    t.erase(t.find(*y));
                    /* if Y is in W */
                    if (w.end() != w.find(*y)) {
                        /* replace Y in W by the same two sets */
                        w.insert(yIx);
                        w.insert(yMyIx);
                        w.erase(w.find(*y));
                    }
                    else if (yIx.size() <= yMyIx.size()) {
                        w.insert(yIx);
                    }
                    else {
                        w.insert(yMyIx);
                    }
                }
            }
            p = t;
        }
    }
    if (verbose) {
        cout <<
        "   $ HopcroftDFAMinimizer: done with main loop $$$$$$$$$$$$$$$$$$$$$$$"
             << endl << endl;
        cout <<
        "   $ HopcroftDFAMinimizer: here are the final equivalence classes $$$$"
             << endl;
        for (y = p.begin(); y != p.end(); ++y) {
            StateSet f = *y;
            cout << "   $ final set" << endl;
            echoSet(f);
            cout << "   $" << endl;
        }
        cout <<
        "   $ HopcroftDFAMinimizer: done listing final equivalence classes $$$$"
             << endl;
        }
    /* now merge the darn things */
    return merge(p, alphabet, transTab, start, f);
}

/* ////////////////////////////////////////////////////////////////////////// */
DFA
HopcroftDFAMinimizer::minimize(DFA targetDFA,
                               bool beVerbose)
{
    /* set some global state */
    verbose = beVerbose;
    /* our alphabet */
    AlphabetString alphabet = targetDFA.getAlphabet();
    /* construct a new transition table that we can play with */
    FSMTransitionTable transitionTable = targetDFA.getTransitionTable();
    /* first state with two sets: final states and everything else */
    StateSet allStates = targetDFA.getAllStates();
    /* all final states */
    StateSet finalStates = targetDFA.getAcceptStates();
    /* all states that are not accept states: S - F */
    StateSet sf = getStateDifference(allStates, finalStates);
    /* start state */
    State start = targetDFA.getStartState();
    DFA minDFA;

    if (verbose) {
        cout << endl <<
        "   $ HopcroftDFAMinimizer: start $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
             << endl;

        cout <<
        "   $ starting with: " << endl;
        cout << "   $ all states (S)" << endl;
        echoSet(allStates);

        cout << "   $ start state (s0)" << endl;
        cout << "     " << start << endl;

        cout << "   $ accept states (F)" << endl;
        echoSet(finalStates);

        cout << "   $ S - F" << endl;
        echoSet(sf);
        cout <<
        "   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
             << endl;
    }

    try {
        /* get to work */
        minDFA = go(alphabet, transitionTable, start, sf, finalStates);
    }
    catch (SLAPException &e) {
        return DFA(targetDFA);
    }
    return minDFA;
}
