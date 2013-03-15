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

#ifndef FSM_TRANSITION_INCLUDED
#define FSM_TRANSITION_INCLUDED

#include "AlphabetSymbol.hxx"
#include "State.hxx"

#include <string>
#include <map>

/* VVV makes C++ programmers :-( */
#define FSMTransitionTable std::multimap<State, FSMTransition>

class FSMTransition {
private:
    AlphabetSymbol alphaSymbol;
    State to;
protected:
public:
    FSMTransition(void) { }

    FSMTransition(const AlphabetSymbol &alphaSymbol,
                  const State &to);

    FSMTransition(const FSMTransition &other);

    ~FSMTransition(void) { }

    std::string str(void) const;

    AlphabetSymbol getInput(void) const;

    State getTo(void) const;

    friend bool operator==(const FSMTransition &t1,
                           const FSMTransition &t2);

    friend bool operator<(const FSMTransition &t1,
                          const FSMTransition &t2);
};

#endif
