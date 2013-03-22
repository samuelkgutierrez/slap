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

#ifndef LEX_DESC_INCLUDED
#define LEX_DESC_INCLUDED

#include "DFA.hxx"
#include "AlphabetSymbol.hxx"

#include <string>
#include <fstream>

class LexDesc {
private:
    DFA dfa;
    std::string id;
    std::string desc;
    std::string semRelStr;

protected:

public:
    LexDesc(void) { }

    ~LexDesc(void) { }

    LexDesc(const AlphabetString &alphabet,
            std::string id,
            std::string desc,
            std::string semRelStr);

    bool accepts(const AlphabetString &input);

    std::string getID(void);
    std::string getRelStr(void);
};

#endif
