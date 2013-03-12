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

FSMTransition::FSMTransition(const State &from,
                             const AlphabetSymbol &alphaSymbol,
                             const State &to)
{
    this->from = from;
    this->alphaSymbol = alphaSymbol;
    this->to = to;
}

/* ////////////////////////////////////////////////////////////////////////// */
/**
 * copy constructor.
 */
FSMTransition::FSMTransition(const FSMTransition &other)
{
    this->from = other.from;
    this->alphaSymbol = other.alphaSymbol;
    this->to = other.to;
}

/* ////////////////////////////////////////////////////////////////////////// */
string
FSMTransition::str(void)
{
    string ret(this->from.str() + " " +
               this->alphaSymbol.str() + " " +
               this->to.str());
    return ret;
}
