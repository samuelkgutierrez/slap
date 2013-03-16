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

#include <iostream>

#include "AlphabetSymbol.hxx"

#define ALPHABET_SYMBOL_INVALID_STR "___XXX___0xFOOBAZ__XXX___"
/* safe to have ' at epsilon start because parser always removes them */
#define ALPHABET_SYMBOL_EPSILON_STR "'epsilon"

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
AlphabetSymbol::AlphabetSymbol(void)
{
    this->symbol = ALPHABET_SYMBOL_INVALID_STR;
}

/* ////////////////////////////////////////////////////////////////////////// */
AlphabetSymbol::AlphabetSymbol(const std::string &strSymbol)
{
    this->symbol = strSymbol;
}

/* ////////////////////////////////////////////////////////////////////////// */
AlphabetSymbol
AlphabetSymbol::epsilon(void)
{
    return AlphabetSymbol(ALPHABET_SYMBOL_EPSILON_STR);
}

/* ////////////////////////////////////////////////////////////////////////// */
/**
 * copy constructor.
 */
AlphabetSymbol::AlphabetSymbol(const AlphabetSymbol &other)
{
    this->symbol = other.symbol;
}

/* ////////////////////////////////////////////////////////////////////////// */
string
AlphabetSymbol::str(void) const
{
    return this->symbol;
}

/* ////////////////////////////////////////////////////////////////////////// */
ostream &
operator<<(ostream &out, const AlphabetSymbol &sym)
{
    out << sym.symbol;
    return out;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
operator==(const AlphabetSymbol &s1,
           const AlphabetSymbol &s2)
{
    return s1.symbol == s2.symbol;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
operator<(const AlphabetSymbol &s1,
          const AlphabetSymbol &s2)
{
    return s1.symbol < s2.symbol;
}
