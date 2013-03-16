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

#include "Constants.hxx"
#include "SLAPException.hxx"
#include "AlphabetSymbol.hxx"
#include "AlphabetParser.hxx"
#include "FSMInputParser.hxx"
#include "FSMTransition.hxx"
#include "State.hxx"
#include "FiniteStateMachine.hxx"
#include "NFA.hxx"
#include "DFA.hxx"
#include "NFAToDFAConverter.hxx"
#include "UserInputStringParser.hxx"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
static bool
nfa2dfa(const string &nfaInPath,
        const AlphabetString &input)
{
    FSMInputParser *inputParser = NULL;
    AlphabetParser *alphaParser = NULL;
    NFA *fsm = NULL;
    bool accepts = false;

    cout <<
    "##########################################################################"
    << endl << "### nfa2dfa: " << nfaInPath << endl
    << "### input: " << endl;
    for (AlphabetString::const_iterator elem = input.begin();
         input.end() != elem;
         ++elem) {
        cout << elem->str() << endl;
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
        fsm->verbose(false);
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
static void
usage(void)
{
    cout << endl << "usage:" << endl;
    cout << "nfa2dfa NFASpecification InputsFile" << endl << endl;
    cout << "nfa2dfa reads an NFA specification file and input strings and "
            "returns whether or not the inputs found within InputsFile are "
            "accepted by the machine." << endl;
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
int
main(int argc, char **argv)
{
    string nfaSpec, inputsFile;

    if (3 != argc) {
        usage();
        return EXIT_FAILURE;
    }
    else {
        nfaSpec = string(argv[1]);
        inputsFile = string(argv[2]);
    }
    try {
        UserInputStringParser inputParser(inputsFile);
        AlphabetStrings inputs = inputParser.getInputs();
        for (AlphabetStrings::const_iterator alphaString = inputs.begin();
             inputs.end() != alphaString;
             ++alphaString) {
            cout << "CCCCCCCCCCCCCCCCCCCCCC" << endl;
            nfa2dfa(nfaSpec, *alphaString);
        }
    }
    catch (SLAPException &e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
