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
#include "SLAPException.hxx"
#include "AlphabetParser.hxx"

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <errno.h>
#include <string.h>

#define ALPHABET_START_KEYWORD "alphabet"
#define ALPHABET_END_KEYWORD   "end"

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
AlphabetParser::~AlphabetParser(void)
{
    ;
}

/* ////////////////////////////////////////////////////////////////////////// */
/* it is assumed that the position into the iterator is set at the beginning
 * of the alphabet specification. */
AlphabetParser::AlphabetParser(char *input)
{
    char *alphaBegin = NULL;
    char *alphaEnd = NULL;
    char *cptr = NULL;
    int symLength = 0;
    bool haveAlpha = false;

    /* find beginning of alphabet */
    alphaBegin = strstr(input, ALPHABET_START_KEYWORD);
    if (NULL == alphaBegin) {
        string eStr = "cannot find beginning of alphabet specification. "
                      "cannot continue input parsing...";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    /* move passed the start keyword */
    alphaBegin += strlen(ALPHABET_START_KEYWORD);
    /* now that we know the start of the alphabet, find the end */
    alphaEnd = strstr(alphaBegin, ALPHABET_END_KEYWORD);
    if (NULL == alphaEnd) {
        string eStr = "cannot find end of alphabet specification. "
                      "cannot continue input parsing...";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    cptr = alphaBegin;
    /* now we can get to parsing the thing... */
    while ('\0' != *cptr) {
        /* skip all the white space and get starting position */
        cptr += strspn(cptr, SLAP_WHITESPACE);
        /* find extent of word */
        symLength = strcspn(cptr, SLAP_WHITESPACE);
        /* done parsing */
        if (cptr >= alphaEnd) {
            if (!haveAlpha) {
                string eStr = "no alphabet symbols found... cannot continue";
                throw SLAPException(SLAP_WHERE, eStr);
            }
            break;
        }
        /* if not dealing with a new alphabet symbol, bail */
        if ('\'' != *cptr) {
            string eStr = "invalid alphabet format. cannot continue...";
            throw SLAPException(SLAP_WHERE, eStr);
        }
        /* make sure this symbol already isn't in our set */
        if (!this->alphabet.insert(string(cptr, symLength)).second) {
            string eStr = "duplicate symbol \"" + string(cptr, symLength) +
                          "\" found. cannot continue...";
            throw SLAPException(SLAP_WHERE, eStr);
        }
        else {
            haveAlpha = true;
        }
        cptr += symLength;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
Alphabet *
AlphabetParser::getNewAlphabet(void)
{
    /* caller is responsible for cleanup */
    return new Alphabet(this->alphabet);
}
