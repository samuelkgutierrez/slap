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

#include "AlphabetSymbol.hxx"
#include "AlphabetString.hxx"
#include "State.hxx"

class FiniteStateMachine {
private:
    FiniteStateMachine(void);
protected:
    virtual State move(const State &state, const AlphabetSymbol &symbol);
public:
    virtual int addState(const State &state) = 0;
    virtual bool addTransition(const State &from,
                               const AlphabetSymbol &symbol,
                               const State &to) = 0;
    virtual int accepts(const AlphabetString &alphaString) = 0;
};

#endif
