#  ____________________________________________________________________________
# | File: makefile - Amiga 3D Demo                                             |
# | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
# | Copyright: (C) 2020 skywalker. All rights reserved.                        |
# |____________________________________________________________________________|

# Target configuration
# Target system: Amiga 500 (AmigaOS 1.3 - version 34.5)
# VBCC compiler config file
VCCONF=+../config/kick13

# Installation directory
INSTDIR=/d/OneDrive/Documents/retro/Amiga/drives/wb1_3/user/bin

# Tools
CC=vc $(VCCONF)
LD=vc $(VCCONF)
MAKE=make
RM=rm -rf
COPY=cp

# Directory structure
SRC=src/
BLD=build/
OBJ=$(BLD)obj/
BIN=$(BLD)bin/
LIBPATH=../build/bin/

# Flags
CPPFLAGS=-I$(INC) -I../inc -DNO_PRAGMAS
CFLAGS=
LDFLAGS=-L$(LIBPATH)

# Source files (anything in the src directory)
SOURCES=$(wildcard $(SRC)*)

# Binary files (exe/objs/libs)
PROG=3d_demo
LIB=3d_graphics.lib
OBJS=$(subst $(SRC),$(OBJ),$(addsuffix .o,$(basename $(SOURCES))))
LIBS=-lamiga -l$(basename $(LIB))

.phony: all install clean $(PROG) $(LIB)
all: $(PROG) install

install: $(PROG)
	$(COPY) $(BIN)$(PROG) $(INSTDIR)

clean:
	$(RM) $(OBJ)* $(BIN)*
	$(MAKE) clean -C../

$(PROG): $(BIN)$(PROG)

$(BIN)$(PROG): $(OBJS) | $(LIB)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

$(LIB): $(LIBPATH)$(LIB)

$(LIBPATH)$(LIB):
	$(MAKE) -C ../

$(OBJ)%.o: $(SRC)%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

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
