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

#include <string>
#include <iostream>
#include <cstdio>

#include "Alphabet.hxx"

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
Alphabet::Alphabet(const set<string> &alphaSet)
{
    this->alphabet = alphaSet;
}

/* ////////////////////////////////////////////////////////////////////////// */
Alphabet::Alphabet(const Alphabet &other)
{
    this->alphabet = other.alphabet;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
Alphabet::isMember(const AlphabetSymbol &symbol)
{
    return (this->alphabet.end() != this->alphabet.find(symbol.str()));
}

/* ////////////////////////////////////////////////////////////////////////// */
std::ostream &
operator<<(std::ostream &out, const Alphabet &a)
{
    set<string>::iterator setIter;

    for (setIter = a.alphabet.begin();
         setIter != a.alphabet.end();
         ++setIter) {
        out << *setIter << endl;
    }
    return out;
}

/* ////////////////////////////////////////////////////////////////////////// */
std::ostream &
operator<<(std::ostream &out, const Alphabet *a)
{
    set<string>::iterator setIter;

    for (setIter = a->alphabet.begin();
         setIter != a->alphabet.end();
         ++setIter) {
        out << *setIter << endl;
    }
    return out;
}
