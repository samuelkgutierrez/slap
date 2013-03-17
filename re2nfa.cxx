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
#include "RegExpInputParser.hxx"
#include "FSMTransition.hxx"
#include "State.hxx"
#include "FiniteStateMachine.hxx"
#include "NFA.hxx"
#include "DFA.hxx"
#include "UserInputStringParser.hxx"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
static bool
re2nfa(const string &nfaInPath,
        const AlphabetString &input,
        bool verbose)
{
    RegExpInputParser *reParser = NULL;
    AlphabetParser *alphaParser = NULL;
    NFA *fsm = NULL;
    bool accepts = false;

    cout << endl <<
    "##########################################################################"
        << endl <<
    "##########################################################################"
    << endl;
    cout << "# re2nfa - input file: " << nfaInPath << endl
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
        alphaParser = new AlphabetParser(nfaInPath);
        alphaParser->parse();

        /* parse the re spec */
        reParser = new RegExpInputParser(nfaInPath,
                                            alphaParser->getAlphabet());
        reParser->verbose(verbose);
        reParser->parse();

        cout << "# alphabet:" << endl;
        reParser->echoAlphabet();
        cout << "# alphabet end" << endl;

        cout << "# starting re --> nfa conversion..." << endl;
        NFA nfa = reParser->getNFA();
        accepts = nfa.accepts(input);
        cout << "re accepts input: " << accepts << endl;
    }
    catch (SLAPException &e) {
        cerr << e.what() << endl;
        return false;
    }
    delete reParser;
    delete alphaParser;
    delete fsm;
    return accepts;
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
usage(void)
{
    cout << endl << "usage:" << endl;
    cout << "re2nfa [-v] RESpecification InputsFile" << endl << endl;
    cout << "re2nfa reads an RE specification file and input strings and "
            "returns whether or not the inputs found within InputsFile are "
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
        re2nfa(nfaSpec, parts, verboseMode);
    }
    catch (SLAPException &e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
