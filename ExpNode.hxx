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

#ifndef EXPNODE_INCLUDED
#define EXPNODE_INCLUDED

#include <string>

#define EXPNODE_NONE 0
#define EXPNODE_BOP  1
#define EXPNODE_UOP  2
#define EXPNODE_SYM  3

class ExpNode {
public:
    int type;
    /* id */
    std::string id;
    /* left child */
    ExpNode *l;
    /* right child */
    ExpNode *r;

    ExpNode(std::string id, int type = EXPNODE_NONE);

    ExpNode(ExpNode *l, std::string op, ExpNode *r);

    ~ExpNode(void);

    static void echoTree(const ExpNode *root);

protected:

private:
    ExpNode(void);

    static void echoNode(const ExpNode *root);
};

#endif
