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
#include "HopcroftDFAMinimizer.hxx"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
static bool
dfa2dfa(const string &dfaInPath,
        const AlphabetString &input,
        bool verbose)
{
    FSMInputParser *fsmParser = NULL;
    AlphabetParser *alphaParser = NULL;
    DFA *fsm, minfsm;
    bool accepts = false;

    cout << endl <<
    "##########################################################################"
        << endl <<
    "##########################################################################"
    << endl;
    cout << "# dfa2dfa - input file: " << dfaInPath << endl
        << "# input: " << endl;
    for (unsigned int i = 0; i < input.size(); ++i) {
        cout << input[i];
    }
    cout << endl <<
    "##########################################################################"
        << endl <<
    "##########################################################################"
    << endl;

    try {
        /* parse the alphabet */
        alphaParser = new AlphabetParser(dfaInPath);
        alphaParser->parse();

        /* parse the re spec */
        fsmParser = new FSMInputParser(dfaInPath,
                                       alphaParser->getAlphabet());
        fsmParser->parse();

        cout << "# alphabet:" << endl;
        fsmParser->echoAlphabet();
        cout << "# alphabet end" << endl;

        fsm = new DFA(alphaParser->getAlphabet(),
                      fsmParser->getTransitionTable(),
                      fsmParser->getAllStates(),
                      fsmParser->getStartState(),
                      fsmParser->getAcceptStates());
        fsm->verbose(verbose);
        cout << "# dfa accepts input: " << fsm->accepts(input) << endl;

        cout << "# now minimizing dfa..." << endl;
        minfsm = HopcroftDFAMinimizer::minimize(*fsm, verbose);
        cout << "# min dfa accepts input: " << minfsm.accepts(input)
             << endl;

    }
    catch (SLAPException &e) {
        cerr << e.what() << endl;
        return false;
    }
    delete fsmParser;
    delete alphaParser;
    delete fsm;
    return accepts;
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
usage(void)
{
    cout << endl << "usage:" << endl;
    cout << "dfa2dfa [-v] DFASpecification InputFile" << endl << endl;
    cout << "dfa2dfa reads a DFA specification file and an input string and "
            "returns whether or not the input found within InputFile is "
            "accepted by the machine." << endl;
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
int
main(int argc, char **argv)
{
    string nfaSpec, inputsFile;
    bool verboseMode = false;

    if (3 != argc && 4 != argc) {
        usage();
        return EXIT_FAILURE;
    }
    else if (3 == argc) {
        nfaSpec = string(argv[1]);
        inputsFile = string(argv[2]);
    }
    else {
        if ("-v" != string(argv[1])) {
            usage();
            return EXIT_FAILURE;
        }
        nfaSpec = string(argv[2]);
        inputsFile = string(argv[3]);
        verboseMode = true;
    }
    try {
        UserInputStringParser inputParser(inputsFile);
        AlphabetString inputs = inputParser.getInputs();
        AlphabetSymbol alphaSym  = *inputs.begin();
        string symStr = alphaSym.str();
        AlphabetString parts;
        for (unsigned long c = 0; c < symStr.length(); ++c) {
            char *cp = &symStr[c];
            parts.push_back(AlphabetSymbol(string(cp, 1)));
        }
        dfa2dfa(nfaSpec, parts, verboseMode);
    }
    catch (SLAPException &e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
