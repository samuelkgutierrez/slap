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

#define LINE_BUF_SIZE          4096
#define ALPHABET_START_KEYWORD "alphabet"
#define ALPHABET_END_KEYWORD   "end"

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
AlphabetParser::~AlphabetParser(void)
{
    ;
}

/* ////////////////////////////////////////////////////////////////////////// */
AlphabetParser::AlphabetParser(const string &fileToParse)
{
    FILE *filep = NULL;
    /* line buffer pointer */
    char *lineBufp = new char[LINE_BUF_SIZE];
    /* we mess with lineBufp, so stash it so we can clean up later */
    char *saveLineBufp = lineBufp;
    /* flag indicating whether or not we have seen the start keyword */
    bool alphabetStart = false;
    /* flag indicating whether or not we have seen some alphabet symbols */
    bool haveSomeAlpha = false;
    /* flag indicating whether or not we have seen the end keyword */
    bool alphabetEnd = false;
    /* exit code */
    int ec = SUCCESS;
    int wordEnd = 0;

    if (NULL == (filep = fopen(fileToParse.c_str(), "r+"))) {
        int err = errno;
        cerr << "cannot open " << fileToParse << ". " << strerror(err) << endl;
        throw FAILURE_IO;
    }
    /* can't hurt */
    fill_n(lineBufp, LINE_BUF_SIZE, '\0');
    /* parse the thing */
    while (NULL != fgets(lineBufp, LINE_BUF_SIZE - 1, filep)) {
        /* skip all the white space and get starting position */
        lineBufp += strspn(lineBufp, WHITESPACE);
        /* find extent of word */
        wordEnd = strcspn(lineBufp, WHITESPACE);
        /* cap the string we have */
        *(lineBufp + wordEnd) = '\0';
        /* if we are at the end of the buffer, just skip */
        if ('\0' == *lineBufp) {
            continue;
        }
        /* have we seen the "alphabet" keyword? */
        if (!alphabetStart) {
            /* this better be the start keyword */
            if (0 != strcasecmp(ALPHABET_START_KEYWORD, lineBufp)) {
                ec = FAILURE_INVLD_FILE_FORMAT;
                break;
            }
            alphabetStart = true;
            continue;
        }
        /* grab all the input that we can from the current buffer */
        while ('\0' != *lineBufp) {
            /* skip all the white space and get starting position */
            lineBufp += strspn(lineBufp, WHITESPACE);
            /* find extent of word */
            wordEnd = strcspn(lineBufp, WHITESPACE);
            /* cap the string we have */
            *(lineBufp + wordEnd) = '\0';
            /* if we are at the end of the buffer, just skip */
            if ('\0' == *lineBufp) {
                continue;
            }
            /* are we ending? */
            if (0 == strcasecmp(ALPHABET_END_KEYWORD, lineBufp)) {
                if (!haveSomeAlpha) {
                    cerr << "end detected with empty alphabet..." << endl;
                    ec = FAILURE_INVLD_FILE_FORMAT;
                    goto out;
                }
                /* all is well... */
                else {
                    alphabetEnd = true;
                    break;
                }
            }
            /* if not dealing with a new alphabet symbol, bail */
            if ('\'' != *lineBufp) {
                ec = FAILURE_INVLD_FILE_FORMAT;
                goto out;
            }
            /* or are we dealing with a new alphabet symbol? */
            haveSomeAlpha = true;
            /* make sure this symbol already isn't in our set */
            if (!this->alphabet.insert(string(lineBufp,
                                       strlen(lineBufp))).second) {
                cerr << "duplicate symbol \""
                     << string(lineBufp, strlen(lineBufp)) << "\" found..."
                     << endl;
                ec = FAILURE_INVLD_FILE_FORMAT;
                goto out;
            }
            /* +1 to skip over string cap */
            lineBufp += (wordEnd + 1);
        }
    }
    /* did we succeed in finding all the pieces that are required? */
    if (!alphabetEnd) {
        ec = FAILURE_INVLD_FILE_FORMAT;
    }

out:
    (void)fclose(filep);
    delete[] saveLineBufp;

    if (SUCCESS != ec) {
        cerr << "invalid alphabet format detected in: " << fileToParse
             << ". cannot continue..." << endl;
        throw ec;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
Alphabet *
AlphabetParser::getNewAlphabet(void)
{
    /* caller is responsible for cleanup */
    return new Alphabet(this->alphabet);
}
