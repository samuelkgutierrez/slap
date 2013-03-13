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

#include "State.hxx"

#define STATE_INVALID_STR "___XXX___0xDEADBEEF__XXX___"

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
State::State(void)
{
    this->name = STATE_INVALID_STR;
}

/* ////////////////////////////////////////////////////////////////////////// */
State::State(const string &alphaSymbol)
{
    this->name = alphaSymbol;
}

/* ////////////////////////////////////////////////////////////////////////// */
/**
 * copy constructor.
 */
State::State(const State &other)
{
    this->name = other.name;
}

/* ////////////////////////////////////////////////////////////////////////// */
State
State::StateInvalid(void)
{
    return State(STATE_INVALID_STR);
}

/* ////////////////////////////////////////////////////////////////////////// */
string
State::str(void)
{
    return this->name;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
operator==(const State &s1, const State &s2)
{
    return s1.name == s2.name;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
operator<(const State &s1, const State &s2)
{
    return s1.name < s2.name;
}

/* ////////////////////////////////////////////////////////////////////////// */
ostream &
operator<<(ostream &out, const State &state)
{
    out << state.name;
    return out;
}
