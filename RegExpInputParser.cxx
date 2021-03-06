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
#include <cctype>
#include <sstream>

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
    char *textCopy = NULL; 
    char *alphaBegin = NULL, *alphaEnd;
    string startStr, endStr, reStr;
    int startStrLen = 0, endStrLen = 0;
    char *saveAB = NULL, *saveAE = NULL;

    if (NULL == fileText) {
        throw SLAPException(SLAP_WHERE, "fileText NULL!");
    }
    /* make a copy of the text because we are going to modify it */
    textCopy = Utils::getNewCString(string(fileText));

    /* find beginning of alphabet */
    alphaBegin = Utils::getListStart(textCopy,
                                     (char *)ALPHABET_START_KEYWORD,
                                     (char *)ALPHABET_END_KEYWORD);
    *alphaBegin = '\0';
    /* move passed the start keyword */
    alphaBegin += strlen(ALPHABET_START_KEYWORD);
    /* now that we know the start of the alphabet, find the end */
    alphaEnd = strstr(alphaBegin, ALPHABET_END_KEYWORD);
    alphaEnd += strlen(ALPHABET_END_KEYWORD);
    *alphaEnd = '\0';
    alphaEnd += 1;

    saveAB = textCopy;
    /* capture the start string */
    while ('\0' != *saveAB) {
        startStrLen += 1;
        ++saveAB;
    }
    startStr = string(textCopy, startStrLen - 1);

    /* now the end string */
    saveAE = alphaEnd;
    while ('\0' != *alphaEnd) {
        endStrLen += 1;
        ++alphaEnd;
    }
    endStr = string(saveAE, endStrLen);

    reStr = startStr + endStr;

    size_t startpos = reStr.find_first_not_of(SLAP_WHITESPACE);
    if (string::npos != startpos) {
        reStr = reStr.substr(startpos);
    }
    size_t endpos = reStr.find_last_not_of(SLAP_WHITESPACE);
    if (string::npos != endpos) {
        reStr = reStr.substr(0, endpos + 1);
    }

    cout << "# re: [" << reStr << "]" << endl;

    delete[] textCopy;
    
    return Utils::getNewCString(reStr);
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
    this->reTree = NULL;
}

RegExpInputParser::RegExpInputParser(const AlphabetString &alpha,
                                     const std::string &reDesc)
{
    /* /// setup private member containers /// */
    /* first get the alphabet - all FSM input will have one of these */
    this->beVerbose = false;
    this->alphabet = alpha;
    /* convert to C string because it's easier to mess with C strings */
    this->cInputStr = NULL;
    this->cRegExpStr = Utils::getNewCString(reDesc);
    this->reTree = NULL;
}

/* ////////////////////////////////////////////////////////////////////////// */
RegExpInputParser::~RegExpInputParser(void)
{
    delete this->reTree;
    delete[] this->cRegExpStr;
    if (this->cInputStr) delete[] this->cInputStr;
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
RegExpInputParser::parse(queue<string> &tokQ)
{
    ExpNode *node = NULL;
    string s = tokQ.front();
    tokQ.pop();

    if (this->beVerbose) {
        cout << "   R reading: " << s << endl;
    }
    if (OP_UNION == s || OP_CONCAT == s) {
        return new ExpNode(parse(tokQ), s, parse(tokQ));
    }
    else if (OP_STAR == s) {
        return new ExpNode(parse(tokQ), s, NULL);
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
        if (OP_CONCAT == root->id) {
            return NFA::getNFAConcat(reTreeToNFA(root->l),
                                     reTreeToNFA(root->r));
        }
        else if (OP_UNION == root->id) {
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
queue<string>
RegExpInputParser::cStrToTokQ(char *cStr)
{
    char *cptr = cStr;
    int slen = 0;
    string s;
    queue<string> tokQ;

    while ('\0' != *cptr) {
        /* skip all the white space and get starting position */
        cptr += strspn(cptr, SLAP_WHITESPACE);
        /* find extent of word */
        slen = strcspn(cptr, SLAP_WHITESPACE);
        if ('\'' == *cptr) {
            s = string(cptr + 1, slen - 1);
            if ("" == s) {
                s = string(" ");
            }
            if (this->alphabet.end() ==
                find(this->alphabet.begin(),
                     this->alphabet.end(),
                     AlphabetSymbol(s))) {
                string eStr = "invalid alphabet symbol found during re parse. "
                              "cannot continue. culprit: [" + s + "]";
                throw SLAPException(SLAP_WHERE, eStr);
            }
        }
        /* must be an op */
        else if (Utils::specialTok(string(cptr, slen))) {
            s = Utils::specialTokToInternalTok(string(cptr, slen));
        }
        /* if not, bail if not empty */
        else if ("" != string(cptr, slen)) {
            string eStr = "cannot parse. invalid operation found: " +
                          string(cptr, slen);
            throw SLAPException(SLAP_WHERE, eStr);
        }
        tokQ.push(s);
        cptr += slen;
    }
    return tokQ;
}

/* ////////////////////////////////////////////////////////////////////////// */
void
RegExpInputParser::parse(void)
{
    ExpNode *root = NULL;
    char *regExpCStr = Utils::getNewCString(string(this->cRegExpStr));
    char *psave = regExpCStr;
    queue<string> tokQ = cStrToTokQ(regExpCStr);

    if (this->beVerbose) {
        cout << "   R walking the parse tree" << endl;
    }
    root = parse(tokQ);
    if (this->beVerbose) {
        cout << endl << "   R done walking the parse tree" << endl;
        cout << "# re after tree walk: " << endl << "# ";
        ExpNode::echoTree(root);
        cout << endl << "# done re after tree walk" << endl;
    }

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
