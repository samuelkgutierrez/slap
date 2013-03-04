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

#include "AlphabetString.hxx"

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
AlphabetString::AlphabetString(void)
{
    ;
}

/* ////////////////////////////////////////////////////////////////////////// */
string
AlphabetString::stringify(void)
{
    string out;
    vector<AlphabetSymbol>::iterator it;

    for (it = this->alphaString.begin(); it != this->alphaString.end(); ++it) {
        out.append(it->stringify());
    }
    return out;
}

/* ////////////////////////////////////////////////////////////////////////// */
void
AlphabetString::append(const AlphabetSymbol &sym)
{
    this->alphaString.push_back(sym);
}

/* ////////////////////////////////////////////////////////////////////////// */
vector<AlphabetSymbol>::iterator
AlphabetString::begin(void)
{
    return this->alphaString.begin();
}

/* ////////////////////////////////////////////////////////////////////////// */
vector<AlphabetSymbol>::const_iterator
AlphabetString::begin(void) const
{
    return this->alphaString.begin();
}

/* ////////////////////////////////////////////////////////////////////////// */
vector<AlphabetSymbol>::iterator
AlphabetString::end(void)
{
    return this->alphaString.end();
}

/* ////////////////////////////////////////////////////////////////////////// */
vector<AlphabetSymbol>::const_iterator
AlphabetString::end(void) const
{
    return this->alphaString.end();
}
