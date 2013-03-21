# Copyright (c) 2013 Samuel K. Gutierrez All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

EXTRA_DIST = \
AUTHORS README COPYRIGHT autogen tests

bin_PROGRAMS = \
slap \
dfa2dfa \
nfa2dfa \
re2nfa

base_src = \
Constants.hxx \
Alphabet.cxx Alphabet.hxx \
AlphabetParser.cxx AlphabetParser.hxx \
AlphabetSymbol.cxx AlphabetSymbol.hxx \
DFA.cxx DFA.hxx \
FSMInputParser.cxx FSMInputParser.hxx \
FSMTransition.cxx FSMTransition.hxx \
FiniteStateMachine.cxx FiniteStateMachine.hxx \
HopcroftDFAMinimizer.cxx HopcroftDFAMinimizer.hxx \
LexDescParser.cxx LexDescParser.hxx \
LexDesc.cxx LexDesc.hxx \
NFA.cxx NFA.hxx \
NFAToDFAConverter.cxx NFAToDFAConverter.hxx \
RegExpInputParser.cxx RegExpInputParser.hxx \
ExpNode.cxx ExpNode.hxx \
SLAPException.cxx SLAPException.hxx \
State.cxx State.hxx \
Utils.cxx Utils.hxx \
UserInputStringParser.cxx UserInputStringParser.hxx

dfa2dfa_SOURCES = \
${base_src} \
dfa2dfa.cxx

nfa2dfa_SOURCES = \
${base_src} \
nfa2dfa.cxx

re2nfa_SOURCES = \
${base_src} \
re2nfa.cxx

slap_SOURCES = \
${base_src} \
slap.cxx
