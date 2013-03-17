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

#include "RegExpInputParser.hxx"
#include "Constants.hxx"
#include "Utils.hxx"
#include "SLAPException.hxx"
#include "ExpNode.hxx"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <locale>
#include <map>

#include <errno.h>
#include <string.h>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
RegExpInputParser::RegExpInputParser(const std::string &fileToParse,
                                     const AlphabetString &alpha)
{
    string inputStr;
    ifstream file(fileToParse.c_str());

    this->beVerbose = false;

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
void
RegExpInputParser::echoAlphabet(void)
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

/* ////////////////////////////////////////////////////////////////////////// */
void
RegExpInputParser::verbose(bool beVerbose)
{
    this->beVerbose = beVerbose;
}

/* ////////////////////////////////////////////////////////////////////////// */
ExpNode *
RegExpInputParser::parse(char *input)
{
    char *cptr = input;
    int wordLen = 0;
    string wordStr;

    if ('\0' == *cptr) {
        return NULL;
    }
    /* skip all the white space and get starting position */
    cptr += strspn(cptr, SLAP_WHITESPACE);
    /* find extent of word */
    wordLen = strcspn(cptr, SLAP_WHITESPACE);
    wordStr = string(cptr, wordLen);
    if (this->beVerbose) {
        cout << "reading: " << wordStr << endl;
    }
    return parse(cptr);
}

/* ////////////////////////////////////////////////////////////////////////// */
void
RegExpInputParser::parse(void)
{
    ExpNode *root = NULL;             
    char *inputCopy = Utils::getNewCString(string(this->cInputStr));

    root = parse(inputCopy);

    delete[] inputCopy;
}
