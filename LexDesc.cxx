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

#include "LexDesc.hxx"
#include "Constants.hxx"
#include "SLAPException.hxx"
#include "DFA.hxx"
#include "NFA.hxx"
#include "RegExpInputParser.hxx"
#include "AlphabetSymbol.hxx"
#include "NFAToDFAConverter.hxx"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>

#include <string.h>
#include <ctype.h>

using namespace std;

LexDesc::LexDesc(const AlphabetString &alphabet,
                 std::string id,
                 std::string desc,
                 std::string semRelStr)
{
    NFA nfa;
    RegExpInputParser reParser(alphabet, desc);

    this->id = id;
    this->desc = desc;
    this->semRelStr = semRelStr;
    reParser.parse();
    nfa = reParser.getNFA();    
    NFAToDFAConverter nfaToDFAConv(nfa);
    dfa = nfaToDFAConv.getDFA();
}
