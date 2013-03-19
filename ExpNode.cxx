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
#include "Constants.hxx"
#include "SLAPException.hxx"

#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

/* ////////////////////////////////////////////////////////////////////////// */
ExpNode::ExpNode(void)
{
    this->type = EXPNODE_NONE;
    this->l = this->r = NULL;
}

/* ////////////////////////////////////////////////////////////////////////// */
ExpNode::ExpNode(string id, int type)
{
    this->type = type;
    this->id = id;
    this->l = this->r = NULL;
}

/* ////////////////////////////////////////////////////////////////////////// */
ExpNode::ExpNode(ExpNode *l, std::string op, ExpNode *r)
{
    this->l = l;
    this->id = op;
    this->r = r;
    if (NULL != this->r) {
        this->type = EXPNODE_BOP;
    }
    else {
        this->type = EXPNODE_UOP;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
ExpNode::~ExpNode(void)
{
    if (this->l) delete this->l;
    if (this->r) delete this->r;
}

/* ////////////////////////////////////////////////////////////////////////// */
void
ExpNode::echoNode(const ExpNode *root)
{
    if (NULL == root) {
        return;
    }
    else if (root->type == EXPNODE_UOP) {
        cout << "(";
        echoNode(root->l);
        cout << root->id << ")";
    }
    else if (root->type == EXPNODE_BOP) {
        cout << "(";
        echoNode(root->l);
        cout << " " << root->id << " ";
        echoNode(root->r);
        cout << ")";
    }
    else if (root->type == EXPNODE_SYM) {
        if (" " == root->id) {
            cout << "_";
        }
        else {
            cout << root->id;
        }
    }
    else {
        string eStr = "unknown exp type in echoNode. cannot continue.";
        throw SLAPException(SLAP_WHERE, eStr);
    }
}

/* ////////////////////////////////////////////////////////////////////////// */
void
ExpNode::echoTree(const ExpNode *root)
{
    if (NULL == root) {
        throw SLAPException(SLAP_WHERE,
                            "unexpected node state. cannot continue.");
        return;
    }
    echoNode(root);
}
