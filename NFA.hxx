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

#ifndef NFA_INCLUDED
#define NFA_INCLUDED

#include "Alphabet.hxx"
#include "AlphabetSymbol.hxx"
#include "State.hxx"
#include "FSMTransition.hxx"
#include "FiniteStateMachine.hxx"

class NFA : public FiniteStateMachine {
private:

protected:

public:
    NFA(void) { }

    NFA(const AlphabetString &alpha,
        const FSMTransitionTable &transitionTab,
        const StateSet &allStates,
        const State &startState,
        const StateSet &acceptStates);
    /* build new NFA from e expression or from alphabet input */
    NFA(const AlphabetSymbol &input);
    /* destructor */    
    ~NFA(void) { }
    /* copy contructor */
    NFA(const NFA &other);
    /* for inputs */
    static NFA getSimpleNFA(const AlphabetSymbol &input, bool verbose);
    /* for * */
    static NFA getKleeneNFA(const NFA &n);
    /* returns a new DFA that is the concatenation of n and m. that is n + m */
    static NFA getNFAConcat(const NFA &n, const NFA &m);
    /* returns DFA that is n | m */
    static NFA getNFAUnion(const NFA &n, const NFA &m);
    /* returns whether or not the string is accepted by the DFA */
    bool accepts(const AlphabetString &alphaString);
    /* returns a set of states that are reachable from s on input */
    StateSet getStatesReachableByTransition(const StateSet &s,
                                            const AlphabetSymbol &input) const;

    void echoTransitions(void);
};

#endif
