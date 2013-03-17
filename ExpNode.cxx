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

#include "ExpNode.hxx"

#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
ExpNode::ExpNode(void)
{
    this->l = this->r = this->exp = NULL;
}

/* ////////////////////////////////////////////////////////////////////////// */
ExpNode::ExpNode(string id)
{
    this->id = id;
    this->l = this->r = this->exp = NULL;
}

/* ////////////////////////////////////////////////////////////////////////// */
ExpNode::~ExpNode(void)
{
    if (this->l) delete this->l;
    if (this->r) delete this->r;
    if (this->exp) delete this->exp;
}

/* ////////////////////////////////////////////////////////////////////////// */
void
ExpNode::echoNode(const ExpNode *root)
{
    cout << root->id;
}

/* ////////////////////////////////////////////////////////////////////////// */
void
ExpNode::echoTree(const ExpNode *root)
{
    if (NULL == root) {
        return;
    }
    echoTree(root->l);
    echoNode(root);
    echoTree(root->r);
}
