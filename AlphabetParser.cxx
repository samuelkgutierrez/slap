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
#include "AlphabetParser.hxx"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>

#include <string.h>
#include <ctype.h>
#include <errno.h>

#define ALPHABET_START_KEYWORD "alphabet"
#define ALPHABET_END_KEYWORD   "end"

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
AlphabetParser::AlphabetParser(const string &fileToParse)
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
    /* convert to C string because it's easier to mess with C strings */
    this->cInputStr = Utils::getNewCString(inputStr);
}

/* ////////////////////////////////////////////////////////////////////////// */
AlphabetParser::~AlphabetParser(void)
{
    delete[] this->cInputStr;
}

/* ////////////////////////////////////////////////////////////////////////// */
/* this should be called AFTER parse */
Alphabet *
AlphabetParser::getNewAlphabet(void)
{
    /* caller is responsible for cleanup */
    return new Alphabet(this->alphabet);
}

/* ////////////////////////////////////////////////////////////////////////// */
void
AlphabetParser::parse(void)
{
    char *alphaBegin = NULL;
    char *alphaEnd = NULL;
    char *cptr = NULL;
    int symLength = 0;
    bool haveAlpha = false;
    char *input = this->cInputStr;

    /* find beginning of alphabet */
    alphaBegin = Utils::getListStart(input,
                                     (char *)ALPHABET_START_KEYWORD,
                                     (char *)ALPHABET_END_KEYWORD);
    if (NULL == alphaBegin) {
        string eStr = "cannot find beginning of alphabet specification. "
                      "cannot continue input parsing...";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    /* move passed the start keyword */
    alphaBegin += strlen(ALPHABET_START_KEYWORD);
    /* now that we know the start of the alphabet, find the end */
    alphaEnd = strstr(alphaBegin, ALPHABET_END_KEYWORD);
    if (NULL == alphaEnd ||
        !Utils::strictlyCStr(input, alphaEnd, strlen(ALPHABET_END_KEYWORD))) {
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
        if (cptr == alphaEnd) {
            /* make sure we have at least one alphabet symbol */
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
