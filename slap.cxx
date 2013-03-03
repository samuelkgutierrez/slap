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

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

#include <unistd.h>

#include "Constants.hxx"
#include "Utils.hxx"
#include "AlphabetParser.hxx"

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
static void
usage(void)
{
    cout << "usage" << endl;
}

/* ////////////////////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////////////////// */
int
main(int argc, char **argv)
{
    AlphabetParser *alphaParser = NULL;

    try {
        alphaParser = new AlphabetParser("./tests/simple.alpha");
    }
    catch (int err) {
        cerr << "slap exception: " << Constants::rc2String(err) << endl;
        return EXIT_FAILURE;
    }

    delete alphaParser;

    return EXIT_SUCCESS;
}
