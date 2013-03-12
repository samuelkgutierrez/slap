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

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
State::State(const string &alphaSymbol)
{
    this->symbol = alphaSymbol;
}

/* ////////////////////////////////////////////////////////////////////////// */
State::~State(void)
{
    ;
}

/* ////////////////////////////////////////////////////////////////////////// */
/**
 * copy constructor.
 */
State::State(const State &other) :
    symbol(other.symbol)
{
    ;
}

/* ////////////////////////////////////////////////////////////////////////// */
string
State::str(void)
{
    return this->symbol;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
operator== (const State &s1, const State &s2)
{
    return s1.symbol == s2.symbol;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
operator< (const State &s1, const State &s2)
{
    return s1.symbol < s2.symbol;
}
