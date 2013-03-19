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

#include "Utils.hxx"
#include "Constants.hxx"
#include "SLAPException.hxx"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>

#include <string.h>
#include <ctype.h>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
char *
Utils::getNewCString(const string &str)
{
    char *lineBuf = new char[str.size() + 1];

    lineBuf[str.size()] = '\0';
    memmove(lineBuf, str.c_str(), str.size());
    /* caller is responsible for calling delete[] */
    return lineBuf;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
Utils::strictlyCStr(char *inStart,
                    char *strBegin,
                    int strLen)
{
    bool startOK = strBegin == inStart || isspace(*(strBegin - 1));

    if (startOK) {
        /* now check end */
        for (int len = 0; len < strLen; ++len) {
            if ('\0' == *(strBegin + len)) {
                return false;
            }
        }
        return '\0' == *(strBegin + strLen) || isspace(*(strBegin + strLen));
    }
    else {
        return false;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
char *
Utils::getListStart(char *inStart,
                    char *beginKeyword,
                    char *endKeyword)
{
    char *cptr = inStart;

    while (NULL != (cptr = strstr(cptr, beginKeyword))) {
        /* close, but not what we are looking for... skip */
        if (!Utils::strictlyCStr(inStart, cptr, strlen(beginKeyword))) {
            /* move pointer passed occurrence */
            cptr += strlen(beginKeyword);
            continue;
        }
        /* the keyword was found, but we couldn't find the end */
        if (NULL == strstr(cptr, endKeyword)) {
            return (char *)NULL;
        }
        /* else, good to go */
        return cptr;
    }
    return (char *)NULL;
}

/* ////////////////////////////////////////////////////////////////////////// */
string
Utils::int2string(int i)
{
    char tmpBuf[32];

    fill_n(tmpBuf, sizeof(tmpBuf), '\0');
    snprintf(tmpBuf, sizeof(tmpBuf) - 1, "%d", i);
    return string(tmpBuf);
}

/* ////////////////////////////////////////////////////////////////////////// */
string
Utils::bufferFile(ifstream &fin)
{
    string input, line;

    while (fin.good()) {
        getline(fin, line);
        input += (line + "\n");
    }
    return input;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
Utils::specialTok(const string &tok)
{
    return "*" == tok || "+" == tok || "|" == tok;
}

/* ////////////////////////////////////////////////////////////////////////// */
string
Utils::specialTokToInternalTok(const string &tok)
{
    if ("*" == tok) return OP_STAR;
    if ("+" == tok) return OP_CONCAT;
    if ("|" == tok) return OP_UNION;
    throw SLAPException(SLAP_WHERE, "cannot convert given token");
}

/* ////////////////////////////////////////////////////////////////////////// */
string
Utils::internalTokToSpecialTok(const std::string &tok)
{
    if (OP_STAR == tok) return "*";
    if (OP_CONCAT == tok) return "+";
    if (OP_UNION == tok) return "|";
    throw SLAPException(SLAP_WHERE, "cannot convert given token");
}
