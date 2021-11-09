#  ____________________________________________________________________________
# | File: makefile - Amiga 3D Demo                                             |
# | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
# | Copyright: (C) 2018 skywalker. All rights reserved.                        |
# |____________________________________________________________________________|

# Target configuration
# Target system: Amiga 500 (AmigaOS 1.3 - version 34.5)
# VBCC compiler config file
VCCONF=+config/kick13

# Tools
CC=vc $(VCCONF)
AS=vasmm68k_mot
LD=vc $(VCCONF)
JOIN=cat
RM=rm -rf

# Directory structure
SRC=src/
INC=inc/
BLD=build/
OBJ=$(BLD)obj/
BIN=$(BLD)bin/

# Flags
CPPFLAGS=-I$(INC) -DNO_PRAGMAS

CFLAGS=
ASFLAGS=-quiet -m68000 -phxass -align -chklabels -wfail -x -Fhunk -kick1hunks

# Source files (anything in the src directory)
SOURCES=$(wildcard $(SRC)*)

# Binary files (lib/objs)
LIB=3d_graphics.lib
OBJS=$(subst $(SRC),$(OBJ),$(addsuffix .o,$(basename $(SOURCES))))

.phony: all clean $(LIB)
all: $(LIB)

clean:
	$(RM) $(OBJ)* $(BIN)*

$(LIB): $(BIN)$(LIB)

$(BIN)$(LIB): $(OBJS)
	$(JOIN) $^ > $@

$(OBJ)%.o: $(SRC)%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ)%.o: $(SRC)%.s
	$(AS) $(ASFLAGS) $(CPPFLAGS) -o $@ $<

#  ____________________________________________________________________________
# | License:                                                                   |
# | This program is free software: you can redistribute it and/or modify       |
# | it under the terms of the GNU General Public License version 2 as          |
# | published by the Free Software Foundation.                                 |
# |                                                                            |
# | This program is distributed in the hope that it will be useful,            |
# | but WITHOUT ANY WARRANTY; without even the implied warranty of             |
# | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              |
# | GNU General Public License for more details.                               |
# |                                                                            |
# | You should have received a copy of the GNU General Public License          |
# | along with this program. If not, see <http://www.gnu.org/licenses/>        |
# | or write to the Free Software Foundation, Inc.,                            |
# | 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                |
# |____________________________________________________________________________|
