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
NFA
NFA::getSimpleNFA(const AlphabetSymbol &input, bool verbose)
{
    NFA nfa;
    State start = getNewState();
    State accept = getNewState();

    if (verbose) {
        cout << "   N creating new: " << start << " " << input << " --> "
             << accept << " transition" << endl;
    }
    nfa.beVerbose = verbose;
    nfa.alphabet.push_back(input);
    nfa.startState = start;
    nfa.acceptStates.insert(accept);
    nfa.allStates.insert(start);
    nfa.allStates.insert(accept);
    nfa.transitionTable.insert(make_pair(start, FSMTransition(input, accept)));
    return nfa;
}

/* ////////////////////////////////////////////////////////////////////////// */
static AlphabetString
getAlphabetStringUnion(const AlphabetString &a,
                       const AlphabetString &b)
{
    AlphabetString u = a;

    for (AlphabetString::const_iterator a = b.begin(); b.end() != a; ++a) {
        if (u.end() == find(u.begin(), u.end(), *a)) {
            u.push_back(*a);
        }
    }
    return u;
}

/* ////////////////////////////////////////////////////////////////////////// */
static StateSet
getStateSetUnion(const StateSet &a,
                 const StateSet &b)
{
    StateSet u;

    set_union(a.begin(),
              a.end(),
              b.begin(),
              b.end(),
              inserter(u, u.end()));
    return u;
}

/* ////////////////////////////////////////////////////////////////////////// */
NFA
NFA::getNFAUnion(const NFA &n, const NFA &m)
{
    NFA unfa;
    AlphabetSymbol eIn = AlphabetSymbol::epsilon();
    State newStart = getNewState();
    State newAccept = getNewState();

    unfa.beVerbose = n.beVerbose || m.beVerbose;

    if (unfa.beVerbose) {
        cout << "   N building union" << endl;
    }

    unfa.startState = newStart;
    unfa.acceptStates.insert(newAccept);

    /* add e transitions from newStart to starts of n and m */
    unfa.transitionTable.insert(make_pair(newStart,
                                          FSMTransition(eIn, n.startState)));
    unfa.transitionTable.insert(make_pair(newStart,
                                          FSMTransition(eIn, m.startState)));
    /* add e transitions from n and m accepts to newAccept */
    for (StateSet::const_iterator a = n.acceptStates.begin();
         n.acceptStates.end() != a;
         ++a) {
        unfa.transitionTable.insert(make_pair(*a, FSMTransition(eIn,
                                                                newAccept)));
    }
    for (StateSet::const_iterator a = m.acceptStates.begin();
         m.acceptStates.end() != a;
         ++a) {
        unfa.transitionTable.insert(make_pair(*a, FSMTransition(eIn,
                                                                newAccept)));
    }
    /* update transition table to include n and m transitions */
    for (FSMTransitionTable::const_iterator t = n.transitionTable.begin();
         n.transitionTable.end() != t;
         ++t) {
        unfa.transitionTable.insert(
            make_pair(t->first, FSMTransition(t->second.getInput(),
                                              t->second.getTo()))
        );
    }
    for (FSMTransitionTable::const_iterator t = m.transitionTable.begin();
         m.transitionTable.end() != t;
         ++t) {
        unfa.transitionTable.insert(
            make_pair(t->first, FSMTransition(t->second.getInput(),
                                              t->second.getTo()))
        );
    }
    /* /// create all states /// */
    unfa.allStates = getStateSetUnion(n.allStates, m.allStates);
    unfa.allStates.insert(newStart);
    unfa.allStates.insert(newAccept);

    /* /// create alphabet /// */
    unfa.alphabet = getAlphabetStringUnion(n.alphabet, m.alphabet);
    /* don't forget about the epsilons! */
    unfa.alphabet.push_back(eIn);

    if (unfa.beVerbose) {
        cout << "   N union start: " << unfa.startState << endl;
        cout << "   N union accept " << endl;
        for (StateSet::iterator a = unfa.acceptStates.begin();
             unfa.acceptStates.end() != a;
             ++a) {
            cout << "   N " << *a << endl;
        }
        cout << "   N end union accept" << endl;
        cout << "   N union states:" << endl;
        for (StateSet::const_iterator s = unfa.allStates.begin();
             unfa.allStates.end() != s;
             ++s) {
            cout << "   N " << *s << endl;
        }
        cout << "   N end union states" << endl;
        cout << "   N union alphabet:" << endl;
        for (AlphabetString::const_iterator a = unfa.alphabet.begin();
             unfa.alphabet.end() != a;
             ++a) {
            cout << "   N " << *a << endl;
        }
        cout << "   N end union alphabet" << endl;
        cout << "   N union transitions:" << endl;
        unfa.echoTransitions();
        cout << "   N end union transitions:" << endl;
        cout << "   N done building union" << endl;
    }

    return unfa;
}

/* ////////////////////////////////////////////////////////////////////////// */
/* XXX this is busted! */
NFA
NFA::getNFAConcat(const NFA &n, const NFA &m)
{
    NFA cnfa;

    cnfa.beVerbose = n.beVerbose || m.beVerbose;

    if (cnfa.beVerbose) {
        cout << "   N building concatenation" << endl;
    }
    /* set new start and accepts */
    cnfa.startState = n.startState;
    cnfa.acceptStates = m.acceptStates;
    /* create alphabet */
    cnfa.alphabet = getAlphabetStringUnion(n.alphabet, m.alphabet);
    /* create all states */
    cnfa.allStates = getStateSetUnion(n.allStates, m.allStates);

    /* build the new transition table */
    cnfa.transitionTable = n.transitionTable;
    for (FSMTransitionTable::const_iterator t = m.transitionTable.begin();
         m.transitionTable.end() != t;
         ++t) {
        cnfa.transitionTable.insert(*t);
    }
    /* XXX */
    if (cnfa.beVerbose) {
        cout << "   N concat start: " << cnfa.startState << endl;
        cout << "   N concat accept " << endl;
        for (StateSet::iterator a = cnfa.acceptStates.begin();
             cnfa.acceptStates.end() != a;
             ++a) {
            cout << "   N " << *a << endl;
        }
        cout << "   N end concat accept" << endl;
        cout << "   N concat transitions:" << endl;
        cnfa.echoTransitions();
        cout << "   N end concat transitions:" << endl;
        cout << "   N done building concatenation" << endl;
    }

    return cnfa;
}

/* ////////////////////////////////////////////////////////////////////////// */
NFA
NFA::getKleeneNFA(const NFA &n)
{
    NFA nfa;
    State nStart = getNewState();
    State nAccept = getNewState();
    AlphabetSymbol eIn = AlphabetSymbol::epsilon();

    State oStart = n.startState;
    StateSet oAccepts = n.acceptStates;

    nfa.beVerbose = n.beVerbose;

    if (nfa.beVerbose) {
        cout << "   N building kleene star" << endl;
    }

    /* add new start and accept states */
    nfa.startState = nStart;
    /* add all old accepts to new */
    nfa.acceptStates = n.acceptStates;
    /* don't forget about the new accept */
    nfa.acceptStates.insert(nAccept);

    nfa.alphabet = n.alphabet;
    nfa.transitionTable = n.transitionTable;

    /* add e to new start and final */
    nfa.transitionTable.insert(make_pair(nStart,
                                         FSMTransition(eIn, nAccept)));
    nfa.transitionTable.insert(make_pair(nStart,
                                         FSMTransition(eIn, oStart)));
    /* add e from old accept to old start */
    nfa.transitionTable.insert(make_pair(*oAccepts.begin(),
                                         FSMTransition(eIn, oStart)));

    if (nfa.beVerbose) {
        cout << "   N kleen start: " << nfa.startState << endl;
        cout << "   N kleen accept " << endl;
        for (StateSet::iterator a = nfa.acceptStates.begin();
             nfa.acceptStates.end() != a;
             ++a) {
            cout << "   N " << *a << endl;
        }
        cout << "   N end kleen accept" << endl;
        cout << "   N kleen transitions:" << endl;
        nfa.echoTransitions();
        cout << "   N end kleen transitions";
        cout << "   N done building kleene star" << endl;
    }
    return nfa;
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

/* ////////////////////////////////////////////////////////////////////////// */
void
NFA::echoTransitions(void)
{
    for (FSMTransitionTable::const_iterator i = this->transitionTable.begin();
         this->transitionTable.end() != i;
         ++i) {
        cout << "   N " << i->first << " " << i->second.getInput() << " --> "
             << i->second.getTo() << endl;
    }
}
