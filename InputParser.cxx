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
#include "InputParser.hxx"
#include "SLAPException.hxx"
#include "Utils.hxx"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <locale>

#include <errno.h>
#include <string.h>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
static bool
isParseType(const string &type)
{
    return type == "dfa" ||
           type == "nfa" ||
           type == "alphabet";
}

/* ////////////////////////////////////////////////////////////////////////// */
InputParser::InputParser(const string &fileToParse)
{
    ifstream file(fileToParse.c_str());
    string line;
    char *lineBuf = NULL;
    char *lineBufp = NULL;
    int wordEnd = 0;

    if (!file.is_open()) {
        int err = errno;
        string eStr = "cannot open " + fileToParse +
                      ". " + strerror(err) + ".\n";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    while (file.good()) {
        getline(file, line);
        lineBuf = Utils::getCString(line);
        lineBufp = lineBuf;
        /* skip all the white space and get starting position */
        lineBufp += strspn(lineBufp, SLAP_WHITESPACE);
        /* find extent of word */
        wordEnd = strcspn(lineBufp, SLAP_WHITESPACE);
        /* cap the string we have */
        *(lineBufp + wordEnd) = '\0';
        /* if we are at the end of the buffer, just skip */
        if ('\0' == *lineBufp) {
            delete[] lineBuf;
            continue;
        }
        /* this better be a valid parse type */
        if (!isParseType(lineBufp)) {
            string eStr = "cannot use " + fileToParse +
                          ". invalid parse type: \'" +
                          lineBuf + "\'\n";
            throw SLAPException(SLAP_WHERE, eStr);
        }
        else {

        }

        delete[] lineBuf;
    }
    file.close();
}
