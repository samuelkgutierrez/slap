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

#ifndef LEX_DESC_PARSER_INCLUDED
#define LEX_DESC_PARSER_INCLUDED

#include "AlphabetSymbol.hxx"
#include "FiniteStateMachine.hxx"
#include "DFA.hxx"
#include "NFA.hxx"
#include "LexDesc.hxx"

#include <string>
#include <fstream>
#include <vector>
#include <utility>

class LexDescParser {
private:
    bool beVerbose;
    std::string fileToParse;
    /* buffer that holds input text */
    char *cInputStr;
    /* the alphabet */
    AlphabetString alphabet;

    LexDescParser(void);

    void parseAlphabet(void);

    void parseClasses(void);

    std::vector<LexDesc> lexs;
    bool someoneAccepts(const AlphabetString &str);

protected:

public:
    LexDescParser(const std::string &fileToParse);

    ~LexDescParser(void);

    void verbose(bool beVerbose = false);

    void parse(const AlphabetString &input);
};

#endif
