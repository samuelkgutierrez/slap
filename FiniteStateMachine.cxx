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

#include "FiniteStateMachine.hxx"
#include "AlphabetSymbol.hxx"

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
FiniteStateMachine::FiniteStateMachine(AlphabetString alpha,
                                       FSMTransitionTable *newTransitionTable,
                                       StateSet *newAllStates,
                                       State startState,
                                       StateSet *newAcceptStates)
{
    this->alphabet = alpha;
    this->transitionTable = newTransitionTable;
    this->allStates = newAllStates;
    this->startState = startState;
    this->acceptStates = newAcceptStates;
}

/* ////////////////////////////////////////////////////////////////////////// */
FiniteStateMachine::FiniteStateMachine(const FiniteStateMachine &other)
{
    this->beVerbose = other.beVerbose;
    this->alphabet = other.alphabet;
    this->transitionTable = new FSMTransitionTable(*other.transitionTable);
    this->allStates = new StateSet(*other.allStates);
    this->startState = other.startState;
    this->acceptStates = new StateSet(*other.acceptStates);
}

/* ////////////////////////////////////////////////////////////////////////// */
FiniteStateMachine::~FiniteStateMachine(void)
{
    delete this->transitionTable;
    delete this->allStates;
    delete this->acceptStates;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
FiniteStateMachine::acceptState(const State &state)
{
    return this->acceptStates->find(state) != this->acceptStates->end();
}

/* ////////////////////////////////////////////////////////////////////////// */
void
FiniteStateMachine::verbose(bool beVerbose)
{
    this->beVerbose = beVerbose;
}

/* ////////////////////////////////////////////////////////////////////////// */
FSMTransitionTable *
FiniteStateMachine::getNewTransitionTable(void)
{
    return new FSMTransitionTable(*this->transitionTable);
}

/* ////////////////////////////////////////////////////////////////////////// */
StateSet *
FiniteStateMachine::getNewAcceptStates(void)
{
    return new StateSet(*this->acceptStates);
}

/* ////////////////////////////////////////////////////////////////////////// */
StateSet *
FiniteStateMachine::getNewAllStates(void)
{
    return new StateSet(*this->allStates);
}

/* ////////////////////////////////////////////////////////////////////////// */
State
FiniteStateMachine::getStartState(void)
{
    return this->startState;
}
/* ////////////////////////////////////////////////////////////////////////// */
AlphabetString
FiniteStateMachine::getAlphabet(void)
{
    return this->alphabet;
}
