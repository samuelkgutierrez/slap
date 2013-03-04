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

#ifndef ALPHABET_STRING_INCLUDED
#define ALPHABET_STRING_INCLUDED

#include <string>
#include <ostream>
#include <vector>

#include "AlphabetSymbol.hxx"

class AlphabetString {
private:
    std::vector<AlphabetSymbol> alphaString;
protected:
public:
    AlphabetString(void);
    void append(const AlphabetSymbol &sym);
    std::string stringify(void);
    /* ////////////////////////////////////////////////////////////////////// */
    typedef std::vector<AlphabetSymbol>::iterator iterator;
    typedef std::vector<AlphabetSymbol>::const_iterator const_iterator;
    iterator begin(void);
    const_iterator begin(void) const;
    iterator end(void);
    const_iterator end(void) const;
};

#endif