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
FiniteStateMachine::FiniteStateMachine(const AlphabetString &alpha,
                                       const FSMTransitionTable &transitionTab,
                                       const StateSet &allStates,
                                       const State &startState,
                                       const StateSet &acceptStates)
{
    this->beVerbose = false;
    this->alphabet = alpha;
    this->transitionTable = transitionTab;
    this->allStates = allStates;
    this->startState = startState;
    this->acceptStates = acceptStates;
}

/* ////////////////////////////////////////////////////////////////////////// */
FiniteStateMachine::FiniteStateMachine(const FiniteStateMachine &other)
{
    this->beVerbose = other.beVerbose;
    this->alphabet = other.alphabet;
    this->transitionTable = other.transitionTable;
    this->allStates = other.allStates;
    this->startState = other.startState;
    this->acceptStates = other.acceptStates;
}

/* ////////////////////////////////////////////////////////////////////////// */
FiniteStateMachine::~FiniteStateMachine(void)
{
    ;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
FiniteStateMachine::acceptState(const State &state)
{
    return this->acceptStates.find(state) != this->acceptStates.end();
}

/* ////////////////////////////////////////////////////////////////////////// */
void
FiniteStateMachine::verbose(bool beVerbose)
{
    this->beVerbose = beVerbose;
}

/* ////////////////////////////////////////////////////////////////////////// */
FSMTransitionTable
FiniteStateMachine::getTransitionTable(void)
{
    return this->transitionTable;
}

/* ////////////////////////////////////////////////////////////////////////// */
StateSet
FiniteStateMachine::getAcceptStates(void)
{
    return this->acceptStates;
}

/* ////////////////////////////////////////////////////////////////////////// */
StateSet
FiniteStateMachine::getAllStates(void)
{
    return this->allStates;
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


/* ////////////////////////////////////////////////////////////////////////// */
void
FiniteStateMachine::addFinalStates(const StateSet &f)
{
    for (StateSet::const_iterator s = f.begin(); s != f.end(); ++s) {
        this->acceptStates.insert(*s);
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
void
FiniteStateMachine::setStart(const State &s)
{
    this->startState = s;
}
