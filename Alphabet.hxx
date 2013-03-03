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

#ifndef ALPHABET_INCLUDED
#define ALPHABET_INCLUDED

#include <string>
#include <set>

#include "AlphabetSymbol.hxx"

class Alphabet {
private:
    Alphabet(void);
    std::set<std::string> alphabet;
protected:
public:
    Alphabet(const std::set<std::string> &alphaSet);
    bool isMember(AlphabetSymbol &symbol);
    void echo(void);

};

#endif