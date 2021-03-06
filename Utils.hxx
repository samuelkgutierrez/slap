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

#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#include <string>
#include <fstream>

class Utils {
private:
    Utils(void);
    ~Utils(void);

protected:

public:
    static char *getNewCString(const std::string &str);

    static bool strictlyCStr(char *inStart,
                             char *strBegin,
                             int strLen);

    static char *getListStart(char *inStart,
                              char *beginKeyword,
                              char *endKeyword);

    static std::string int2string(int i);

    static std::string bufferFile(std::ifstream &fin);

    static std::string specialTokToInternalTok(const std::string &tok);

    static std::string internalTokToSpecialTok(const std::string &tok);

    static bool specialTok(const std::string &tok);
};

#endif
