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
#include "FSMTransition.hxx"
#include "State.hxx"

class FiniteStateMachine {
private:
    FiniteStateMachine(void);

protected:
    bool beVerbose;
    Alphabet *alphabet;
    FSMTransitionTable *transitionTable;
    State startState;
    StateSet *acceptStates;

    bool acceptState(const State &state);

public:
    FiniteStateMachine(Alphabet *newAlphabet,
                       FSMTransitionTable *newTransitionTable,
                       State startState,
                       StateSet *newAcceptStates);

    FiniteStateMachine(const FiniteStateMachine &other);

    ~FiniteStateMachine(void);

    void verbose(bool beVerbose);

    virtual bool accepts(AlphabetString &alphaString) = 0;
};

#endif
