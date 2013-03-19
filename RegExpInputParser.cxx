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
/* ////////////////////////////////////////////////////////////////////////// */
/* private utility functions */
/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */

/* ////////////////////////////////////////////////////////////////////////// */
static char *
getRegExpCStr(char *fileText)
{
    char *cptr = fileText, *dptr = NULL;
    int inputLen = 0;

    /* skip all the white space and get starting position */
    cptr += strspn(cptr, SLAP_WHITESPACE);
    dptr = strcasestr(cptr, ALPHABET_START_KEYWORD);
    /* cap, so we ignore the alphabet spec */
    *dptr = '\0';
    dptr = cptr;
    dptr += strcspn(dptr, SLAP_EOL);
    *dptr = '\0';
    cout << "# re: [" << string(cptr) << "]" << endl;

    return Utils::getNewCString(string(cptr));
}

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
    this->cRegExpStr = getRegExpCStr(this->cInputStr);
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
/* XXX add some input checks here */
ExpNode *
RegExpInputParser::parse(char *input)
{
    char *cptr = input;
    int slen = 0;
    string s;
    ExpNode *node = NULL;

    /* skip all the white space and get starting position */
    cptr += strspn(cptr, SLAP_WHITESPACE);
    /* find extent of word */
    slen = strcspn(cptr, SLAP_WHITESPACE);
    if ('\'' == *cptr) {
        s = string(cptr + 1, slen - 1);
        if ("" == s && ' ' == *(cptr + 1)) {
            s = string(" ");
        }
        if (this->alphabet.end() ==
            find(this->alphabet.begin(), this->alphabet.end(), AlphabetSymbol(s))) {
            string eStr = "invalid alphabet symbol found during re parse. "
                          "cannot continue. culprit: [" + s + "]";
            throw SLAPException(SLAP_WHERE, eStr);
        }
    }
    else {
        s = string(cptr, slen);
    }
    cptr += slen;
    if (this->beVerbose) {
        cout << "   R reading: " << s << endl;
    }
    if ("|" == s || "+" == s) {
        return new ExpNode(parse(cptr), s, parse(cptr + 1));
    }
    else if ("*" == s) {
        return new ExpNode(parse(cptr), s, NULL);
    }
    else {
        return new ExpNode(s, EXPNODE_SYM);
    }
    return node;
}

/* ////////////////////////////////////////////////////////////////////////// */
NFA
RegExpInputParser::reTreeToNFA(ExpNode *root)
{
    if (NULL == root) {
        string eStr = "unexpected node state. cannot continue.";
        throw SLAPException(SLAP_WHERE, eStr);
    }
    if (root->type == EXPNODE_SYM) {
        return NFA::getSimpleNFA(root->id, this->beVerbose);
    }
    else if (root->type == EXPNODE_UOP) {
        return NFA::getKleeneNFA(reTreeToNFA(root->l));
    }
    else if (root->type == EXPNODE_BOP) {
        if ("+" == root->id) {
            return NFA::getNFAConcat(reTreeToNFA(root->l),
                                     reTreeToNFA(root->r));
        }
        else if ("|" == root->id) {
            return NFA::getNFAUnion(reTreeToNFA(root->l),
                                    reTreeToNFA(root->r));
        }
        else {
            string eStr = "unknown op type in reTreeToNFA. cannot continue.";
            throw SLAPException(SLAP_WHERE, eStr);
        }
    }
    else {
        string eStr = "unknown exp type in reTreeToNFA. cannot continue.";
        throw SLAPException(SLAP_WHERE, eStr);
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
void
RegExpInputParser::parse(void)
{
    ExpNode *root = NULL;             
    char *regExpCStr = Utils::getNewCString(string(this->cRegExpStr));
    char *psave = regExpCStr;

    if (this->beVerbose) {
        cout << "   R walking the parse tree" << endl;
    }
    root = parse(regExpCStr);
    if (this->beVerbose) {
        cout << "   R done walking the parse tree" << endl;
    }
    cout << "# re after tree walk: ";
    ExpNode::echoTree(root);
    cout << endl;

    this->reTree = root;

    delete[] psave;
}

/* ////////////////////////////////////////////////////////////////////////// */
NFA
RegExpInputParser::getNFA(void)
{
    NFA nfa;

    if (this->beVerbose) {
        cout << "   R building an NFA from re tree" << endl;
    }
    nfa = this->reTreeToNFA(this->reTree);
    if (this->beVerbose) {
        cout << "   R done building an NFA from re tree" << endl;
    }

    return nfa;
}
