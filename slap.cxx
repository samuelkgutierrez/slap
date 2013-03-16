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

#include "SLAPException.hxx"
#include "AlphabetParser.hxx"
#include "FSMInputParser.hxx"
#include "FSMTransition.hxx"
#include "State.hxx"
#include "FiniteStateMachine.hxx"
#include "NFA.hxx"
#include "DFA.hxx"
#include "NFAToDFAConverter.hxx"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
bool
dfaXListBool(const string &dfaInPath,
             const string &inputPath)
{
    FSMInputParser *inputParser = NULL;
    AlphabetParser *alphaParser = NULL;
    DFA *fsm = NULL;
    FiniteStateMachine *minFSM = NULL;
    bool accepts = false;
    /* XXX TODO read from file */
    AlphabetString input;
    AlphabetSymbol a("a");
    AlphabetSymbol b("b");
    AlphabetSymbol c("c");

    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    cout <<
    "##########################################################################"
    << endl << "### dfaXListBool: " << dfaInPath << endl
    << "### input: " << endl;
    for (unsigned int i = 0; i < input.size(); ++i) {
        cout << input[i];
    }
    cout << endl <<
    "##########################################################################"
    << endl;

    try {
        alphaParser = new AlphabetParser(dfaInPath);
        alphaParser->parse();

        inputParser = new FSMInputParser(dfaInPath,
                                         alphaParser->getAlphabet());
        inputParser->parse();

        fsm = new DFA(alphaParser->getAlphabet(),
                      inputParser->getTransitionTable(),
                      inputParser->getAllStates(),
                      inputParser->getStartState(),
                      inputParser->getAcceptStates());
        fsm->verbose(true);
        cout << "ORIG" << endl;
        accepts = fsm->accepts(input);
        cout << "accepts: " << accepts << endl;
        minFSM = fsm->minimize();
        minFSM->verbose(true);
        accepts = minFSM->accepts(input);
        cout << "accepts: " << accepts << endl;
    }
    catch (SLAPException &e) {
        cerr << e.what() << endl;
        return false;
    }

    delete inputParser;
    delete alphaParser;
    delete fsm;
    return accepts;
}

/* ////////////////////////////////////////////////////////////////////////// */
bool
nfaXListBool(const string &nfaInPath,
             const string &inputPath)
{
    FSMInputParser *inputParser = NULL;
    AlphabetParser *alphaParser = NULL;
    NFA *fsm = NULL;
    bool accepts = false;
    /* XXX TODO read from file */
    AlphabetString input;
    AlphabetSymbol a("a");
    AlphabetSymbol b("b");
    AlphabetSymbol c("c");

    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    input.push_back(a);
    cout <<
    "##########################################################################"
    << endl << "### nfaXListBool: " << nfaInPath << endl
    << "### input: " << endl;
    for (unsigned int i = 0; i < input.size(); ++i) {
        cout << input[i];
    }
    cout << endl <<
    "##########################################################################"
    << endl;

    try {
        alphaParser = new AlphabetParser(nfaInPath);
        alphaParser->parse();

        inputParser = new FSMInputParser(nfaInPath,
                                         alphaParser->getAlphabet());
        inputParser->parse();

        fsm = new NFA(alphaParser->getAlphabet(),
                      inputParser->getTransitionTable(),
                      inputParser->getAllStates(),
                      inputParser->getStartState(),
                      inputParser->getAcceptStates());
        fsm->verbose(true);
        cout << "ORIG" << endl;
        accepts = fsm->accepts(input);
        cout << "accepts: " << accepts << endl;
    }
    catch (SLAPException &e) {
        cerr << e.what() << endl;
        return false;
    }

    delete inputParser;
    delete alphaParser;
    delete fsm;
    return accepts;
}

/* ////////////////////////////////////////////////////////////////////////// */
static bool
nfa2dfa(const string &nfaInPath,
        const string &inputPath)
{
    FSMInputParser *inputParser = NULL;
    AlphabetParser *alphaParser = NULL;
    NFA *fsm = NULL;
    bool accepts = false;
    /* XXX TODO read from file */
    AlphabetString input;
    AlphabetSymbol a("a");
    AlphabetSymbol b("b");
    AlphabetSymbol c("c");

    input.push_back(a);
    input.push_back(a);
    input.push_back(b);
    input.push_back(a);
    input.push_back(b);
    input.push_back(b);
    input.push_back(b);
    input.push_back(b);
    input.push_back(a);

    cout <<
    "##########################################################################"
    << endl << "### nfaXListBool: " << nfaInPath << endl
    << "### input: " << endl;
    for (unsigned int i = 0; i < input.size(); ++i) {
        cout << input[i];
    }
    cout << endl <<
    "##########################################################################"
    << endl;

    try {
        DFA dfa;
        NFAToDFAConverter converter;
        alphaParser = new AlphabetParser(nfaInPath);
        alphaParser->parse();

        inputParser = new FSMInputParser(nfaInPath,
                                         alphaParser->getAlphabet());
        inputParser->parse();

        fsm = new NFA(alphaParser->getAlphabet(),
                      inputParser->getTransitionTable(),
                      inputParser->getAllStates(),
                      inputParser->getStartState(),
                      inputParser->getAcceptStates());
        fsm->verbose(true);
        accepts = fsm->accepts(input);
        cout << "accepts: " << accepts << endl;

    }
    catch (SLAPException &e) {
        cerr << e.what() << endl;
        return false;
    }

    delete inputParser;
    delete alphaParser;
    delete fsm;
    return accepts;
}

/* ////////////////////////////////////////////////////////////////////////// */
#if 0
static void
usage(void)
{
    cout << "usage" << endl;
}
#endif

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
int
main(int argc, char **argv)
{
    bool accepts = false;
#if 0
    accepts = dfaXListBool("./tests/dfa1.txt", "./tests/dfa1-test-input.txt");
    cout << "accepts: " << accepts << endl;

    accepts = nfaXListBool("./tests/nfa2.txt", "./tests/dfa1-test-input.txt");
    cout << "accepts: " << accepts << endl;
#endif

    cout <<
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
        <<endl;
    accepts = nfa2dfa("./tests/nfa1.txt", "asdf");
    return EXIT_SUCCESS;
}
