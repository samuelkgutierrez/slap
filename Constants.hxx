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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define SLAP_WHITESPACE " \t\n\v\f\r"
#define SLAP_EOL        "\n\f\r"
#define SLAP_WHERE      __FILE__, __LINE__

#define ALPHABET_START_KEYWORD "alphabet"
#define ALPHABET_END_KEYWORD   "end"

#define OP_STAR   "__0xSLAP__STAR__"
#define OP_CONCAT "__0xSLAP__CONCAT__"
#define OP_UNION  "__0xSLAP__UNION__"

#endif
