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

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <errno.h>
#include <string.h>

#include "Constants.hxx"
#include "Utils.hxx"
#include "AlphabetParser.hxx"

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
AlphabetParser::~AlphabetParser(void)
{
}

/* ////////////////////////////////////////////////////////////////////////// */
AlphabetParser::AlphabetParser(const string &fileToParse)
{
    FILE *filep = NULL;
    char lineBuf[4096];
    char *lineBufp = NULL;
    int startPos = 0;
    bool startRead = false;

    if (NULL == (filep = fopen(fileToParse.c_str(), "r+"))) {
        int err = errno;
        cerr << "cannot open " << fileToParse << ". " << strerror(err) << endl;
        throw FAILURE_IO;
    }
    while (NULL != fgets(lineBuf, sizeof(lineBuf) - 1, filep)) {
        lineBufp = lineBuf;
        /* skip all the white space and get starting position */
        startPos = strspn(lineBuf, WHITESPACE);
        lineBufp += startPos;
    }

    fclose(filep);
}
