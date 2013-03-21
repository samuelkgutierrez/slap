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

#include "LexDescParser.hxx"
#include "Constants.hxx"
#include "SLAPException.hxx"
#include "Utils.hxx"
#include "AlphabetSymbol.hxx"
#include "AlphabetParser.hxx"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>

#include <string.h>
#include <ctype.h>
#include <errno.h>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
LexDescParser::LexDescParser(const string &fileToParse)
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
    this->beVerbose = false;
    this->fileToParse = fileToParse;
    this->cInputStr = Utils::getNewCString(inputStr);
}

/* ////////////////////////////////////////////////////////////////////////// */
void
LexDescParser::verbose(bool beVerbose)
{
    this->beVerbose = beVerbose;
}

/* ////////////////////////////////////////////////////////////////////////// */
LexDescParser::~LexDescParser(void)
{
    delete[] this->cInputStr;
}

/* ////////////////////////////////////////////////////////////////////////// */
void
LexDescParser::parseAlphabet(void)
{
    AlphabetParser alphaParser(this->fileToParse);
    alphaParser.parse();
    this->alphabet = alphaParser.getAlphabet();
}

/* ////////////////////////////////////////////////////////////////////////// */
void
LexDescParser::parse(void)
{
    this->parseAlphabet();
    if (this->beVerbose) {
        cout << "   L alphabet" << endl;
        for (AlphabetString::const_iterator a = this->alphabet.begin();
             this->alphabet.end() != a;
             ++a) {
            cout << "     " << *a << endl;
        }
        cout << "   L alphabet" << endl;
    }
}