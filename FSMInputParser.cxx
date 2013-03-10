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

/* parses slap input */

#include "Constants.hxx"
#include "FSMInputParser.hxx"
#include "SLAPException.hxx"
#include "AlphabetParser.hxx"
#include "Utils.hxx"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <locale>
#include <map>

#include <errno.h>
#include <string.h>

using namespace std;

enum FSMType {
    DFA,
    NFA,
    UNKNOWN
};

/* ////////////////////////////////////////////////////////////////////////// */
static string
bufferFile(ifstream &fin)
{
    string input, line;

    while (fin.good()) {
        getline(fin, line);
        input += (line + "\n");
    }
    return input;
}

/* ////////////////////////////////////////////////////////////////////////// */
static FSMType
determineFSMTypeFromInput(char *input)
{
    map<FSMType, string> validTypes;
    /* populate with valid types */
    validTypes[DFA] = "dfa";
    validTypes[NFA] = "nfa";
    char *typeStart = NULL;

    for (map<FSMType, string>::iterator it = validTypes.begin();
         it != validTypes.end();
         ++it) {
        if (NULL != (typeStart = strstr(input, it->second.c_str()))) {
            /* now make sure that we are just dealing with a valid keyword */
            if (Utils::strictlyCStr(input, typeStart,
                strlen(it->second.c_str()))) {
                return it->first;
            }
        }
    }
    return UNKNOWN;
}

/* ////////////////////////////////////////////////////////////////////////// */
FSMInputParser::FSMInputParser(const string &fileToParse)
{
    char *cInputStr = NULL;
    string inputStr;
    ifstream file(fileToParse.c_str());

    if (!file.is_open()) {
        int err = errno;
        string eStr = "cannot open " + fileToParse +
                      ". " + strerror(err) + ".\n";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    /* buffer the file */
    inputStr = bufferFile(file);
    /* convert to C string because it's easier to mess with C strings */
    cInputStr = Utils::getNewCString(inputStr);

    /* first get the alphabet - all FSM input will have one of these */
    this->alphaParser = new AlphabetParser(cInputStr);
    this->alphabet = alphaParser->getNewAlphabet();

    /* now parse the finite state machine (FSM) description */
    this->parse(cInputStr);

    delete[] cInputStr;
    file.close();
}

/* ////////////////////////////////////////////////////////////////////////// */
FSMInputParser::~FSMInputParser(void)
{
    delete this->alphaParser;
    delete this->alphabet;
}

/* ////////////////////////////////////////////////////////////////////////// */
void
FSMInputParser::parse(char *cInputStr)
{
    FSMType type = determineFSMTypeFromInput(cInputStr);

    if (UNKNOWN == type) {
    }
}
