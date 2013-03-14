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

#include "FSMTransition.hxx"

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
FSMTransition::FSMTransition(const AlphabetSymbol &alphaSymbol,
                             const State &to)
{
    this->alphaSymbol = alphaSymbol;
    this->to = to;
}

/* ////////////////////////////////////////////////////////////////////////// */
/**
 * copy constructor.
 */
FSMTransition::FSMTransition(const FSMTransition &other)
{
    this->alphaSymbol = other.alphaSymbol;
    this->to = other.to;
}

/* ////////////////////////////////////////////////////////////////////////// */
string
FSMTransition::str(void)
{
    string ret(this->alphaSymbol.str() + " " +
               this->to.str());
    return ret;
}

/* ////////////////////////////////////////////////////////////////////////// */
State
FSMTransition::getTo(void)
{
    return this->to;
}
/* ////////////////////////////////////////////////////////////////////////// */
AlphabetSymbol
FSMTransition::getInput(void)
{
    return this->alphaSymbol;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
operator==(const FSMTransition &t1,
           const FSMTransition &t2)
{
    return t1.alphaSymbol == t2.alphaSymbol && t1.to == t2.to;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
operator<(const FSMTransition &t1,
          const FSMTransition &t2)
{
    return (t1.alphaSymbol < t2.alphaSymbol) && (t1.to < t2.to);
}
