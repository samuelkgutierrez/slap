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
#include "UserInputStringParser.hxx"
#include "LexDesc.hxx"
#include "LexDescParser.hxx"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
static void
echoHeader(void)
{
    cout <<
    "c        .__" << endl <<
    "c   _____|  | _____  ______" << endl <<
    "c  /  ___/  | \\__  \\ \\____ \\" << endl <<
    "c  \\___ \\|  |__/ __ \\|  |_> >" << endl <<
    "c /____  >____(____  /   __/" << endl <<
    "c      \\/          \\/|__|" << endl;
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
usage(void)
{
    cout << endl << "usage:" << endl;
    cout << "slap [-v] LexDesc FileToParse" << endl << endl;
    cout << "slap reads a lexical specification file and an input file "
            "and parses its input." << endl;
}

/* ////////////////////////////////////////////////////////////////////////// */
static void
parse(bool verboseMode,
      const string &lexDesc,
      const AlphabetString &input)
{
    LexDescParser ldParser(lexDesc);
    ldParser.verbose(verboseMode);
    ldParser.parse();
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
int
main(int argc, char **argv)
{
    string lexDesc, targetFile;
    bool verboseMode = false;

    if (3 != argc && 4 != argc) {
        usage();
        return EXIT_FAILURE;
    }
    else if (3 == argc) {
        lexDesc = string(argv[1]);
        targetFile = string(argv[2]);
    }
    else {
        if ("-v" != string(argv[1])) {
            usage();
            return EXIT_FAILURE;
        }
        lexDesc = string(argv[2]);
        targetFile = string(argv[3]);
        verboseMode = true;
    }
    try {
        echoHeader();
        UserInputStringParser inputParser(targetFile);
        AlphabetString input = inputParser.getInput();
        parse(verboseMode, lexDesc, input);
    }
    catch (SLAPException &e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
