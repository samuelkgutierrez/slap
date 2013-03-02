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

SHELL  = /bin/sh

.SUFFIXES:
.SUFFIXES: .cxx .hxx .o
.PHONY: clean all

TARGET = slap

CXX = g++
CXXFLAGS = -Wall -g -O0

SOURCES = $(wildcard *.cxx)
OBJS = $(SOURCES:.cxx=.o)

RM = /bin/rm -rf

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

clean:
	$(RM) *.o $(TARGET) $(TARGET).dSYM

%.o: %.cxx
	$(CXX) -c $(CXXFLAGS) $< -o $@
