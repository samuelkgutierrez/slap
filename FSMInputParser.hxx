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

#ifndef FSM_INPUT_PARSER_INCLUDED
#define FSM_INPUT_PARSER_INCLUDED

#include "Alphabet.hxx"
#include "State.hxx"
#include "FSMTransition.hxx"

#include <string>

class FSMInputParser {
public:
    /* finite state machine types */
    enum FSMType {
        DFA,
        NFA,
        UNKNOWN
    };
    FSMInputParser(const std::string &fileToParse,
                   const AlphabetString &alpha);

    ~FSMInputParser(void);

    void parse(void);

    FSMTransitionTable getTransitionTable(void);

    StateSet getAllStates(void);

    StateSet getAcceptStates(void);

    State getStartState(void);
    void echoAlphabet(void);

private:
    /* buffer that holds input text */
    char *cInputStr;
    /* the alphabet generated by an alphabet parse */
    AlphabetString alphabet;
    /* set of all states */
    StateSet stateSet;
    /* set of all accept states */
    StateSet acceptStateSet;
    /* the start state. should only be one item in here */
    StateSet initStateSet;
    /* table of transitions (multimap) */
    FSMTransitionTable transitionTable;

    FSMInputParser(void);

    void parse(char *cInputStr);

    char *parseStates(char *startPos);

    char *parseInitState(char *startPos);

    char *parseAcceptStates(char *startPos);

    char *parseTransitions(char *startPos, FSMType fsmType);

protected:

};

#endif
