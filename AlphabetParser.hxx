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

#ifndef ALPHABET_PARSER_INCLUDED
#define ALPHABET_PARSER_INCLUDED

#include "Alphabet.hxx"

#include <string>
#include <set>

class AlphabetParser {
private:
    /* buffer that holds input text */
    char *cInputStr;
    /* set containing compete alphabet */
    std::set<std::string> alphabet;
    AlphabetParser(void);
protected:
public:
    AlphabetParser(const std::string &fileToParse);
    ~AlphabetParser(void);
    AlphabetString getAlphabet(void);
    void parse(void);
};

#endif
