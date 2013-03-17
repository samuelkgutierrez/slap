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

bool verbose = false;

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

#if 0
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
#endif

/* ////////////////////////////////////////////////////////////////////////// */
static
StateSet
getStateUnion(const StateSet &s1,
              const StateSet &s2)
{
    StateSet u;

    set_union(s1.begin(),
              s1.end(),
              s2.begin(),
              s2.end(),
              inserter(u, u.end()));
    return u;
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

#if 0
/* ////////////////////////////////////////////////////////////////////////// */
/* returns {s1, s2, ..., sn} that contains a set of states that 'a' can reach on
 * a transition on c */
static StateSet
getStatesReachableByAonC(const FSMTransitionTable &transTab,
                         const State &a,
                         const AlphabetSymbol &c)
{
    StateSet stateSet;
    FSMTransitionTable myTransTab = transTab;
    FSMTransitionTable::iterator it;

    if (verbose) {
        cout << "   $ finding valid delta(" << a << ", " << c << ") " << endl;
    }
    while (myTransTab.end() != (it = myTransTab.find(a))) {
        if (verbose) {
            cout << "   $ " << it->first << " "
                 << it->second.getInput() << " --> "
                 << it->second.getTo() << endl;
        }
        stateSet.insert(it->second.getTo());
        myTransTab.erase(it);
    }
    if (verbose) {
        cout << "   $" << endl;
    }
    return stateSet;
}
#endif

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
static FSMTransitionTable
buildTransitionsFromStart(const FSMTransitionTable &transTab,
                          const StateSet &startSet)
{
    FSMTransitionTable retTransTab;
    FSMTransition transition;

    return retTransTab;

}

/* ////////////////////////////////////////////////////////////////////////// */
static State
getNewState(void)
{
    static int stateName = 0;
    return State(Utils::int2string(stateName++));
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
merge(SOS &P,
      const AlphabetString &alphabet,
      const FSMTransitionTable &transTab,
      const State &start,
      const StateSet &acceptStates)
{
    SOS::iterator ssi;
    StateSet startSet;

    if (verbose) {
        cout << endl <<
        "   $ HopcroftDFAMinimizer: starting equivalence class merge $$$$$$$$$$"
             << endl;
        cout << "   $ looking for start state" << endl;
    }
    startSet = getStateSetStateIn(P, start);
    /* start better be by itself */
    if (startSet.size() == 1) {
        if (verbose) {
            cout << "   $ found it" << endl;
            echoSet(startSet);
        }
    }
    else {
        throw SLAPException(SLAP_WHERE, "i <3 dfas");
    }
    /* now let's construct a new transition table based on the start state */
    if (verbose) {
        cout <<
        "   $ HopcroftDFAMinimizer: starting to build new DFA from start $$$$$$"
             << endl;
    }

    /* /// start building up the DFA pieces /// */
    /* init stuff we know */
    /* start state is okay */
    State minStart = start;
    /* alphabet isn't going to change across the minimization */
    AlphabetString minAlphabet = alphabet;
    /* the rest we'll fill in next */
    FSMTransitionTable minTransTab;
    /* accept states for min DFA */
    StateSet minAcceptStates;
    /* all states within the min DFA */
    StateSet minAllStates;
    /* used to build up final transition table */
    FSMTransitionTable tmpTransTab;

    if (verbose) {
        cout <<
        "   $ HopcroftDFAMinimizer: building accept state set $$$$$$$$$$$$$$$$$"
             << endl;
    }
    /* /// get accept states /// */
    for (StateSet::iterator ssi = acceptStates.begin();
         ssi != acceptStates.end();
         ++ssi) {
        StateSet newAccepts;
        newAccepts = getStateSetStateIn(P, *ssi);
        if (newAccepts.size() > 0) {
            minAcceptStates = getStateUnion(minAcceptStates, newAccepts);
        }
    }
    if (verbose) {
        echoSet(minAcceptStates);
        cout <<
        "   $ HopcroftDFAMinimizer: done building accept state set $$$$$$$$$$$$"
             << endl;
    }
    /* build the transition table from the start set */
    minTransTab = buildTransitionsFromStart(transTab, startSet);
    if (verbose) {
        cout << endl <<
        "   $ HopcroftDFAMinimizer: done with equivalence class merge $$$$$$$$$"
             << endl;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
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
    merge(p, alphabet, transTab, start, f);
}

/* ////////////////////////////////////////////////////////////////////////// */
DFA *
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
        go(alphabet, transitionTable, start, sf, finalStates);
    }
    catch (SLAPException &e) {
        return new DFA(targetDFA);
    }
    /* XXX */
    return new DFA(targetDFA);
}
