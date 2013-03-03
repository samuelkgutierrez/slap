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

#ifndef CONSTANTS_INCLUDED
#define CONSTANTS_INCLUDED

#define SUCCESS                   0
#define FAILURE                   1
#define FAILURE_IO                2
#define FAILURE_INVLD_FILE_FORMAT 3

#define WHITESPACE " \t\n\v\f\r"

class Constants {
private:
    Constants(void);
    ~Constants(void);
public:
    static std::string rc2String(int rc);
};

#endif