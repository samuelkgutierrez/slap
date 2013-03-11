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

#define STATES_START_KEYWORD        "states"
#define STATES_END_KEYWORD          "end;"
#define INITIAL_STATE_START_KEYWORD "initial"
#define ACCEPT_STATE_START_KEYWORD  "accept"

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
    /* close the file */
    file.close();
    /* convert to C string because it's easier to mess with C strings */
    cInputStr = Utils::getNewCString(inputStr);

    /* first get the alphabet - all FSM input will have one of these */
    this->alphaParser = new AlphabetParser(cInputStr);
    this->alphabet = alphaParser->getNewAlphabet();

    /* now parse the finite state machine (FSM) description */
    this->parse(cInputStr);

    delete[] cInputStr;
}

/* ////////////////////////////////////////////////////////////////////////// */
FSMInputParser::~FSMInputParser(void)
{
    delete this->alphaParser;
    delete this->alphabet;
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
stateParse(char *stateStart, char **end)
{
    /* start passed the start keyword */
    char *cptr = (stateStart + strlen(STATES_START_KEYWORD));
    /* end of the state input */
    char *stateEnd = strstr(stateStart, STATES_END_KEYWORD);
    bool haveState = false;
    int stateLen = 0;

    if (NULL == stateEnd ||
        !Utils::strictlyCStr(stateStart, stateEnd, strlen(STATES_END_KEYWORD))) {
        string eStr = "state end not found... cannot continue";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    while ('\0' != *cptr) {
        /* skip all the white space and get starting position */
        cptr += strspn(cptr, SLAP_WHITESPACE);
        /* find extent of word */
        stateLen = strcspn(cptr, SLAP_WHITESPACE);
        /* done parsing */
        if (cptr == stateEnd) {
            /* make sure we have at least one alphabet symbol */
            if (!haveState) {
                string eStr = "no states found... cannot continue";
                throw SLAPException(SLAP_WHERE, eStr);
            }
            /* capture end of state parse */
            *end = cptr + strlen(STATES_END_KEYWORD);
            break;
        }
        /* XXX add addition to to set */
        cout << string(cptr, stateLen) << endl;
        haveState = true;
#if 0
        /* make sure this symbol already isn't in our set */
        if (!this->alphabet.insert(string(cptr, symLength)).second) {
            string eStr = "duplicate symbol \"" + string(cptr, symLength) +
                          "\" found. cannot continue...";
            throw SLAPException(SLAP_WHERE, eStr);
        }
        else {
            haveAlpha = true;
        }
#endif
        cptr += stateLen;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
initialStateParse(char *stateStart, char **end)
{
    /* start passed the start keyword */
    char *cptr = (stateStart + strlen(INITIAL_STATE_START_KEYWORD));
    /* end of the state input */
    char *stateEnd = strstr(stateStart, ACCEPT_STATE_START_KEYWORD);
    bool haveState = false;
    int stateLen = 0;

    if (NULL == stateEnd ||
        !Utils::strictlyCStr(stateStart, stateEnd,
                             strlen(ACCEPT_STATE_START_KEYWORD))) {
        string eStr = "initial state end not found... cannot continue";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    while ('\0' != *cptr) {
        /* skip all the white space and get starting position */
        cptr += strspn(cptr, SLAP_WHITESPACE);
        /* find extent of word */
        stateLen = strcspn(cptr, SLAP_WHITESPACE);
        if (haveState) {
            if (ACCEPT_STATE_START_KEYWORD !=
                string(cptr, strlen(ACCEPT_STATE_START_KEYWORD))) {
                string eStr = "multiple start states detected. "
                              "cannot continue.";
                throw SLAPException(SLAP_WHERE, eStr);
            }
            /* done parsing */
            *end = cptr + strlen(ACCEPT_STATE_START_KEYWORD);
            break;
        }
        /* XXX add addition to to set */
        cout << "start: " << string(cptr, stateLen) << endl;
        haveState = true;
#if 0
        /* make sure this symbol already isn't in our set */
        if (!this->alphabet.insert(string(cptr, symLength)).second) {
            string eStr = "duplicate symbol \"" + string(cptr, symLength) +
                          "\" found. cannot continue...";
            throw SLAPException(SLAP_WHERE, eStr);
        }
        else {
            haveAlpha = true;
        }
#endif
        cptr += stateLen;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
void
FSMInputParser::parse(char *cInputStr)
{
    FSMType type = UNKNOWN;
    char *pos = cInputStr;

    /* XXX add start end balance checking thing here */

    type = determineFSMTypeFromInput(cInputStr);
    if (UNKNOWN == type) {
        string eStr = "cannot determine finite state machine type. "
                      "cannot continue.";
        throw SLAPException(SLAP_WHERE, eStr);
    }

    /* /// parse states /// */
    if (NULL == (pos = Utils::getListStart(pos,
                                           (char *)STATES_START_KEYWORD,
                                           (char *)STATES_END_KEYWORD))) {
        string eStr = "cannot find \'" STATES_START_KEYWORD "\' begin. "
                      "cannot continue.";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    stateParse(pos, &pos);

    /* /// parse initial state */
    if (NULL == (pos =
        Utils::getListStart(pos,
                            (char *)INITIAL_STATE_START_KEYWORD,
                            (char *)ACCEPT_STATE_START_KEYWORD))) {
        string eStr = "cannot find \'" INITIAL_STATE_START_KEYWORD"\' begin. "
                      "cannot continue.";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    initialStateParse(pos, &pos);
}
