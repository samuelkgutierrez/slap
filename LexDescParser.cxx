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
#include <stack>

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
static stack<string>
getNewTypeClassStack(void)
{
    stack<string> classes;
    classes.push("relevant");
    classes.push("irrelevant");
    classes.push("discard");
    return classes;
}

/* ////////////////////////////////////////////////////////////////////////// */
void
LexDescParser::parseClasses(void)
{
    char *fStartSav = this->cInputStr;
    char *cptr = NULL, *dptr = NULL, *cend = NULL;
    string classID, reStr, classRel;
    int wordl = 0;
    stack<string> classes;
    cptr = fStartSav;

    while (true) {
        classes = getNewTypeClassStack();
        /* find beginning of class */
        cptr = Utils::getListStart(cptr,
                                   (char *)CLASS_START_KEYWORD,
                                   (char *)CLASS_END_KEYWORD);
        /* done */
        if (NULL == cptr) {
            break;
        }
        /* find end of class */
        cend = strstr(cptr, CLASS_END_KEYWORD);
        /* start working from beginning */
        /* skip keyword */
        cptr += strlen(CLASS_START_KEYWORD);
        /* each white space */
        cptr += strspn(cptr, SLAP_WHITESPACE);
        /* find extent of word */
        wordl = strcspn(cptr, SLAP_WHITESPACE);
        /* store class id */
        classID = string(cptr, wordl);
        /* now skip over id is and whitespace */
        cptr += strlen(classID.c_str());
        cptr += strspn(cptr, SLAP_WHITESPACE);
        cptr += strlen("is");
        cptr += strspn(cptr, SLAP_WHITESPACE);
        /* now grab re */
        while (!classes.empty()) {
            string curClass = classes.top();
            classes.pop();
            if (NULL == (dptr = strstr(cptr, curClass.c_str()))) {
                continue;
            }
            /* make sure that it is within out bounds */
            else if (dptr > cend) {
                continue;
            }
            /* we found it! */
            else {
                reStr = string(cptr, dptr - cptr);
                classRel = curClass;
                if (beVerbose) {
                    cout << "   L found new class" << endl;
                    cout << "   L class id: " << classID << endl;
                    cout << "   L class desc: " << reStr << endl;
                    cout << "   L class semantic relevance: " << classRel
                         << endl;
                    cout << "   L end found new class" << endl;
                }
                break;
            }
        }
        cptr = cend + strlen(CLASS_END_KEYWORD);
    }
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
        cout << "   L end alphabet" << endl;
    }
    this->parseClasses();
}
