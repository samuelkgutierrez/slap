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

#ifndef ALPHABET_SYMBOL_INCLUDED
#define ALPHABET_SYMBOL_INCLUDED

#include <string>
#include <ostream>
#include <vector>

#define AlphabetString std::vector<AlphabetSymbol>

class AlphabetSymbol {
private:
    std::string symbol;

protected:

public:
    AlphabetSymbol(void);
    /* copy constructor */
    AlphabetSymbol(const AlphabetSymbol &other);
    /* construct AlphabetSymbol from string */
    AlphabetSymbol(const std::string &strSymbol);
    /* get string representation of AlphabetSymbol instance */
    std::string str(void) const;
    /* returns an epsilon symbol */
    static AlphabetSymbol epsilon(void);
    /* == */
    friend bool operator==(const AlphabetSymbol &s1,
                           const AlphabetSymbol &s2);
    /* < */
    friend bool operator<(const AlphabetSymbol &s1,
                          const AlphabetSymbol &s2);
    /* << */
    friend std::ostream &operator<<(std::ostream &out,
                                    const AlphabetSymbol &sym);
};

#endif
