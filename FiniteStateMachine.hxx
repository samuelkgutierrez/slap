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

#ifndef FINITE_STATE_MACHINE_INCLUDED
#define FINITE_STATE_MACHINE_INCLUDED

#include "Alphabet.hxx"
#include "AlphabetSymbol.hxx"
#include "FSMTransition.hxx"
#include "State.hxx"

class FiniteStateMachine {
private:

protected:
    bool beVerbose;
    AlphabetString alphabet;
    FSMTransitionTable transitionTable;
    State startState;
    StateSet acceptStates;
    StateSet allStates;

    bool acceptState(const State &state);

public:
    FiniteStateMachine(void) { }

    FiniteStateMachine(const AlphabetString &alpha,
                       const FSMTransitionTable &transitionTab,
                       const StateSet &allStates,
                       const State &startState,
                       const StateSet &acceptStates);

    FiniteStateMachine(const FiniteStateMachine &other);

    virtual ~FiniteStateMachine(void);

    void verbose(bool beVerbose);

    AlphabetString getAlphabet(void);

    FSMTransitionTable getTransitionTable(void);

    StateSet getAllStates(void);

    StateSet getAcceptStates(void);

    State getStartState(void);
    /* sets DFA start state */
    void setStart(const State &s);
    /* adds final states from set f */
    void addFinalStates(const StateSet &f);
    /* returns true if the alphaString is accepted by the machine */
    virtual bool accepts(const AlphabetString &alphaString) = 0;
};

#endif
