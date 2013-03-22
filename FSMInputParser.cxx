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

#include "Constants.hxx"
#include "Utils.hxx"
#include "SLAPException.hxx"
#include "FSMInputParser.hxx"
#include "State.hxx"

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
#define ACCEPT_STATE_END_KEYWORD    "end;"
#define TRANSITIONS_START_KEYWORD   "transitions"
#define TRANSITIONS_END_KEYWORD     "end;"
#define TRANSITION_ARROW            "-->"

/* number of "parts" within a single transition specification */
#define NUM_TRANSITION_PARTS 4

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
/* private utility functions */
/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */

/* ////////////////////////////////////////////////////////////////////////// */
static FSMInputParser::FSMType
determineFSMTypeFromInput(char *input)
{
    map<FSMInputParser::FSMType, string> validTypes;
    /* populate with valid types */
    validTypes[FSMInputParser::DFA] = "dfa";
    validTypes[FSMInputParser::NFA] = "nfa";
    char *typeStart = NULL;
    map<FSMInputParser::FSMType, string>::iterator it;

    for (it = validTypes.begin(); it != validTypes.end(); ++it) {
        if (NULL != (typeStart = strstr(input, it->second.c_str()))) {
            /* now make sure that we are just dealing with a valid keyword */
            if (Utils::strictlyCStr(input, typeStart,
                strlen(it->second.c_str()))) {
                return it->first;
            }
        }
    }
    return FSMInputParser::UNKNOWN;
}

/* ////////////////////////////////////////////////////////////////////////// */
/* *start and *end will always be valid if no exception is encountered */
static void
parseStart(string id,
           char *startKeyword,
           char *endKeyword,
           char *listStart,
           char **start,
           char **end)
{
    char *cptr = NULL, *listEnd = NULL;
    /* get start of list */
    if (NULL == (cptr = Utils::getListStart(listStart,
                                            startKeyword,
                                            endKeyword))) {
        string eStr = "cannot find \'" + string(startKeyword) +
                      "\' begin/end. cannot continue.";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    /* make sure we have a valid end */
    if (NULL == (listEnd = strstr(cptr, endKeyword)) ||
        !Utils::strictlyCStr(listStart, listEnd, strlen(endKeyword))) {
        string eStr = id + " end not found... cannot continue";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    /* if we are here, then all is well */
    *start = cptr;
    *end = listEnd;
}

/* ////////////////////////////////////////////////////////////////////////// */
static char *
parseDFAInputList(FSMInputParser::FSMType fsmType,
                  char *start,
                  const AlphabetString &alphabet,
                  AlphabetString &resultList)
{
    /* we are dealing with: 'alphaSym1 'alphaSym2 '... alphaSymN */
    char *cptr = start;
    int wordLen = 0;
    string inputStr;
    AlphabetSymbol tmpAlphaSym;

    /* skip all the white space and get starting position */
    cptr += strspn(cptr, SLAP_WHITESPACE);
    /* find extent of word */
    wordLen = strcspn(cptr, SLAP_WHITESPACE);
    /* create string */
    inputStr = string(cptr, wordLen);
    /* remove the ' */
    inputStr.erase(inputStr.begin(), inputStr.begin() + 1);
    /* create the symbol */
    tmpAlphaSym = AlphabetSymbol(inputStr);
    /* sanity */
    if ('\'' != *cptr) {
        string eStr = "invalid input detected during transition parse."
            " expected \' at symbol start, but found " +
            string(cptr, 1) + ". cannot continue.";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    if ("" == inputStr) {
        inputStr = " ";
        tmpAlphaSym = AlphabetSymbol(inputStr);
    }
    /* else make sure it is a valid symbol */
    if (alphabet.end() == find(alphabet.begin(), alphabet.end(), tmpAlphaSym)) {
        string eStr = "invalid symbol detected during transition parse."
            " \'" + inputStr+ "\' is not a valid symbol. "
            "cannot continue.";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    /* else, good to go */
    resultList.push_back(tmpAlphaSym);
    return cptr;
}

/* ////////////////////////////////////////////////////////////////////////// */
static char *
parseNFAInputList(FSMInputParser::FSMType fsmType,
               char *start,
               const AlphabetString &alphabet,
               AlphabetString &resultList)
{
    /* we are dealing with: 'alphaSym1 'alphaSym2 '... alphaSymN */
    char *cptr = start;
    int wordLen = 0;
    string inputStr;
    AlphabetSymbol tmpAlphaSym;

    while ('\0' != *cptr) {
        /* skip all the white space and get starting position */
        cptr += strspn(cptr, SLAP_WHITESPACE);
        /* find extent of word */
        wordLen = strcspn(cptr, SLAP_WHITESPACE);
        /* create the string */
        inputStr = string(cptr, wordLen);
        /* remove the ' */
        inputStr.erase(inputStr.begin(), inputStr.begin() + 1);
        /* create the symbol */
        tmpAlphaSym = AlphabetSymbol(inputStr);
        /* done! */
        if ('\'' != *cptr) {
            /* rewind, so the caller can start at the next part of the parse */
            if (resultList.empty()) {
                resultList.push_back(AlphabetSymbol::epsilon());
            }
            return (char *)cptr - wordLen;
        }
        /* else new input symbol */
        /* else make sure it is a valid symbol */
        if (alphabet.end() ==
                find(alphabet.begin(), alphabet.end(), tmpAlphaSym)) {
            string eStr = "invalid symbol detected during transition parse."
                " \'" + inputStr+ "\' is not a valid symbol. "
                "cannot continue.";
            throw SLAPException(SLAP_WHERE, eStr);
        }
        /* else, good to go */
        resultList.push_back(tmpAlphaSym);
        cptr += wordLen;
    }
    return cptr;
}

/* ////////////////////////////////////////////////////////////////////////// */
static char *
parseSingleTransition(FSMInputParser::FSMType fsmType,
                      char *start,
                      const AlphabetString &alphabet,
                      const StateSet &states,
                      FSMTransitionTable &transTab)
{
    char *cptr = start;
    /* C string length of item */
    int wordLen = 0;
    /* transition part number */
    int part = 0;
    /* states */
    State state1, state2;
    /* input symbols */
    AlphabetString inputs;

    /* general transition form                      */
    /* 0      1                          2   3      */
    /* state1 'alphaSymA 'alphaSymB '... --> state2 */

    while ('\0' != *cptr && part < NUM_TRANSITION_PARTS) {
        /* skip all the white space and get starting position */
        cptr += strspn(cptr, SLAP_WHITESPACE);
        /* find extent of word */
        wordLen = strcspn(cptr, SLAP_WHITESPACE);
        string sym = string(cptr, wordLen);
        /* parsing state1 or state2 */
        if (0 == part || 3 == part) {
            State tmpState(sym);
            /* is this a valid state? */
            if (states.end() == states.find(tmpState.str())) {
                string eStr = "invalid state detected during transition parse."
                              " \'" + sym + "\' is not a valid state. "
                              "cannot continue.";
                throw SLAPException(SLAP_WHERE, eStr);
            }
            /* else, it is valid */
            if (0 == part) {
                state1 = tmpState;
            }
            else {
                state2 = tmpState;
            }
        }
        /* parsing 'alphaSym list */
        else if (1 == part) {
            if (FSMInputParser::DFA == fsmType) {
                cptr = parseDFAInputList(fsmType, cptr, alphabet, inputs);
            }
            else if (FSMInputParser::NFA == fsmType) {
                cptr = parseNFAInputList(fsmType, cptr, alphabet, inputs);
            }
            else {
                string eStr = "invalid finite state machine type during "
                              "transition parse. cannot continue.";
                throw SLAPException(SLAP_WHERE, eStr);
            }
        }
        /* parsing --> */
        else if (2 == part) {
            if (TRANSITION_ARROW != sym) {
                string eStr = "invalid symbol detected during transition parse."
                              " expected \'" TRANSITION_ARROW "\' but got \'" +
                              sym + "\' cannot continue.";
                throw SLAPException(SLAP_WHERE, eStr);
            }
            /* else, we are cool */
        }
        /* move passed parsed input */
        cptr += wordLen;
        /* update part */
        ++part;
    }
    /* last bit of sanity to make sure that we got all the parts */
    if (NUM_TRANSITION_PARTS != part) {
        string eStr = "incomplete parse detected during transition parse. "
                      "cannot continue.";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    if (FSMInputParser::DFA == fsmType && 1 != inputs.size()) {
        string eStr = "input inconsistency. dfa type detected, but found " +
                      Utils::int2string((int)inputs.size()) + " transitions " +
                      "to a state.";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    /* at this point, we should have a complete and valid transition, so add
     * them to the multimap of transitions that we are maintaining */
    for (AlphabetString::iterator aIt = inputs.begin();
         aIt != inputs.end();
         ++aIt) {
        transTab.insert(make_pair(state1, FSMTransition(*aIt, state2)));
    }

    return cptr;
}

/* ////////////////////////////////////////////////////////////////////////// */
/**
 * parses transition lists of the form: transition t1 t2 ... end end and returns
 * end position.
 */
static char *
transitionsParse(FSMInputParser::FSMType fsmType,
                 string id,
                 char *startKeyword,
                 char *endKeyword,
                 char *listStart,
                 const AlphabetString &alphabet,
                 const StateSet &states,
                 FSMTransitionTable &transitionTable)
{
    /* char pointer */
    char *cptr = NULL;
    /* end of the state input */
    char *listEnd = NULL;
    /* # items inserted */
    int nItems = 0;

    /* start parse */
    parseStart(id, startKeyword, endKeyword, listStart, &cptr, &listEnd);
    /* move passed the start keyword before we start parsing */
    cptr += strlen(startKeyword);
    /* start parsing loop */
    while ('\0' != *cptr) {
        /* skip all the white space and get starting position */
        cptr += strspn(cptr, SLAP_WHITESPACE);
        /* done parsing */
        if (cptr == listEnd) {
            /* make sure we have at least one item */
            if (0 == nItems) {
                string eStr = "no " + id + "s found... cannot continue";
                throw SLAPException(SLAP_WHERE, eStr);
            }
            /* capture end of state parse */
            return (char *)(cptr + strlen(endKeyword));
        }
        /* now start parsing a single transition */
        cptr = parseSingleTransition(fsmType,
                                     cptr,
                                     alphabet,
                                     states,
                                     transitionTable);
        /* if we are here, then no exceptions were encountered in parse */
        ++nItems;
    }
    /* if we are here, then something weird happened */
    return (char *)NULL;
}

/* ////////////////////////////////////////////////////////////////////////// */
/**
 * parses lists in the form: start i1 i2 ... in end and returns end position.
 */
static char *
listParse(string id,
          char *startKeyword,
          char *endKeyword,
          char *listStart,
          StateSet &targetSet,
          int insertLimit)
{
    /* char pointer */
    char *cptr = NULL;
    /* end of the state input */
    char *listEnd = NULL;
    /* C string length of item */
    int wordLen = 0;
    /* # items inserted */
    int nItems = 0;

    /* start parse */
    parseStart(id, startKeyword, endKeyword, listStart, &cptr, &listEnd);
    /* move passed the start keyword before we start parsing */
    cptr += strlen(startKeyword);
    /* start parsing loop */
    while ('\0' != *cptr) {
        /* skip all the white space and get starting position */
        cptr += strspn(cptr, SLAP_WHITESPACE);
        /* find extent of word */
        wordLen = strcspn(cptr, SLAP_WHITESPACE);
        /* done parsing */
        if (cptr == listEnd) {
            /* make sure we have at least one item */
            if (0 == nItems) {
                string eStr = "no " + id + "s found... cannot continue";
                throw SLAPException(SLAP_WHERE, eStr);
            }
            /* capture end of state parse */
            return (char *)(cptr + strlen(endKeyword));
        }
        /* make sure this item already isn't in our set */
        if (!targetSet.insert(State(string(cptr, wordLen))).second) {
            string eStr = "duplicate " + id + " \"" + string(cptr, wordLen) +
                          "\" found. cannot continue...";
            throw SLAPException(SLAP_WHERE, eStr);
        }
        /* insert successful */
        else {
            ++nItems;
            if (-1 != insertLimit && nItems > insertLimit) {
                string maxIStr = Utils::int2string(insertLimit);
                string eStr = "more than " + maxIStr + " item " +
                              "found in " + id + " parse. cannot continue.";
                throw SLAPException(SLAP_WHERE, eStr);
            }
        }
        cptr += wordLen;
    }
    /* if we are here, then something weird happened */
    return (char *)NULL;
}

/* ////////////////////////////////////////////////////////////////////////// */
FSMInputParser::FSMInputParser(const string &fileToParse,
                               const AlphabetString &alpha)
{
    string inputStr;
    ifstream file(fileToParse.c_str());

    /* problem opening the file */
    if (!file.is_open()) {
        int err = errno;
        string eStr = "cannot open " + fileToParse +
                      ". " + strerror(err) + ".\n";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    /* buffer the file */
    inputStr = Utils::bufferFile(file);
    /* close the file */
    file.close();

    /* /// setup private member containers /// */
    /* first get the alphabet - all FSM input will have one of these */
    this->alphabet = alpha;
    /* convert to C string because it's easier to mess with C strings */
    this->cInputStr = Utils::getNewCString(inputStr);
}

/* ////////////////////////////////////////////////////////////////////////// */
FSMInputParser::~FSMInputParser(void)
{
    delete[] this->cInputStr;
}

/* ////////////////////////////////////////////////////////////////////////// */
void
FSMInputParser::parse(void)
{
    /* now parse the finite state machine (FSM) description */
    this->parse(this->cInputStr);
}

/* ////////////////////////////////////////////////////////////////////////// */
char *
FSMInputParser::parseStates(char *startPos)
{

    return listParse("state",
                     (char *)STATES_START_KEYWORD,
                     (char *)STATES_END_KEYWORD,
                     startPos,
                     this->stateSet,
                     -1);
}

/* ////////////////////////////////////////////////////////////////////////// */
char *
FSMInputParser::parseInitState(char *startPos)
{
    char *pos = listParse("initial",
                          (char *)INITIAL_STATE_START_KEYWORD,
                          (char *)ACCEPT_STATE_START_KEYWORD,
                          startPos,
                          this->initStateSet,
                          1);
    /* need to rewind a bit before we return because the end of this list is the
     * start of accept and listParse returns a pos at the end of the end keyword
     */
    return (char *)(pos - strlen(ACCEPT_STATE_START_KEYWORD));
}

/* ////////////////////////////////////////////////////////////////////////// */
char *
FSMInputParser::parseAcceptStates(char *startPos)
{
    return listParse("accept",
                     (char *)ACCEPT_STATE_START_KEYWORD,
                     (char *)ACCEPT_STATE_END_KEYWORD,
                     startPos,
                     this->acceptStateSet,
                     -1);
}

/* ////////////////////////////////////////////////////////////////////////// */
char *
FSMInputParser::parseTransitions(char *startPos,
                                 FSMType fsmType)
{
    return transitionsParse(fsmType,
                            "transition",
                            (char *)TRANSITIONS_START_KEYWORD,
                            (char *)TRANSITIONS_END_KEYWORD,
                            startPos,
                            this->alphabet,
                            this->stateSet,
                            this->transitionTable);
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
    pos = this->parseStates(pos);
    /* /// parse initial state /// */
    pos = this->parseInitState(pos);
    /* /// parse accept states /// */
    pos = this->parseAcceptStates(pos);
    /* /// parse transitions /// */
    pos = this->parseTransitions(pos, type);

    /* XXX add check for EOF and make sure no garbage is left */
}

/* ////////////////////////////////////////////////////////////////////////// */
FSMTransitionTable
FSMInputParser::getTransitionTable(void)
{
    return this->transitionTable;
}

/* ////////////////////////////////////////////////////////////////////////// */
StateSet
FSMInputParser::getAcceptStates(void)
{
    return this->acceptStateSet;
}

/* ////////////////////////////////////////////////////////////////////////// */
StateSet
FSMInputParser::getAllStates(void)
{
    return this->stateSet;
}

/* ////////////////////////////////////////////////////////////////////////// */
State
FSMInputParser::getStartState(void)
{
    /* one last bit of sanity. make sure that we only have one start state. */
    if (1 != this->initStateSet.size()) {
        string eStr = "more than one start state detected while returning "
                      "start state. cannot continue.";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    /* else, return the one item in the set */
    return *this->initStateSet.begin();
}

/* ////////////////////////////////////////////////////////////////////////// */
void
FSMInputParser::echoAlphabet(void)
{
    for (AlphabetString::const_iterator a = this->alphabet.begin();
         this->alphabet.end() != a;
         ++a) {
        if (" " == a->str()) {
            cout << "# _" << endl;
        }
        else {
            cout << "# " << *a  << endl;
        }
    }
}
