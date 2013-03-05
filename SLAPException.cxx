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

#include <string>
#include <cstdio>

#include <string.h>

#include "SLAPException.hxx"

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
SLAPException::SLAPException(const char *fileName,
                             int lineNo,
                             const string &errMsg)
{
    char numBuf[16];

    memset(numBuf, '\0', sizeof(numBuf));
    snprintf(numBuf, sizeof(numBuf) - 1, " (%d): ", lineNo);
    whatString.append(fileName).append(numBuf).append(errMsg);
}

/* ////////////////////////////////////////////////////////////////////////// */
const char *
SLAPException::what(void) const throw()
{
    return this->whatString.c_str();
}
