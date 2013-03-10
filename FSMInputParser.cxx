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
// XXX RM ME V
#include "AlphabetParser.hxx"
#include "Utils.hxx"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <locale>
#include <vector>

#include <errno.h>
#include <string.h>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
static bool
isParseType(const string &type)
{
#if 0
    return type == "dfa" ||
           type == "nfa" ||
           type == "alphabet";
#endif
    return type == "alphabet";
}

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
FSMInputParser::FSMInputParser(const string &fileToParse)
{
    char *cInputStr = NULL;
    string inputStr;
    ifstream file(fileToParse.c_str());
    AlphabetParser *alphaParser = NULL;

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
    /* first get the alphabet */
    alphaParser = new AlphabetParser(cInputStr);

#if 0
    for (strvecIter = fileBuffer->begin();
         strvecIter != fileBuffer->end();
         ++strvecIter) {
        lineBuf = Utils::getNewCString(*strvecIter);
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
            /* XXX rm me */
            continue;
            string eStr = "cannot use " + fileToParse +
                          ". invalid parse type: \'" +
                          lineBuf + "\'\n";
            throw SLAPException(SLAP_WHERE, eStr);
        }
        else {
            AlphabetParser *tmp = new AlphabetParser(strvecIter, fileBuffer->end());
        }

        delete[] lineBuf;
    }
#endif
    file.close();
}
