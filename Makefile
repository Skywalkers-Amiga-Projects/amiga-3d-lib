#  ____________________________________________________________________________
# | File: makefile - Amiga 3D Demo                                             |
# | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
# | Copyright: (C) 2018 skywalker. All rights reserved.                        |
# |____________________________________________________________________________|

# Target configuration (uncomment applicable config part).
# Target system: Amiga 500 (AmigaOS 1.3 - version 34.5)
# VBCC compiler config file (default vc.cfg)
#VCCONF=+vc.cfg

# Detect OS
OS := $(shell uname -s)

# Installation directory
INSTDIR=$(HOME)/Documents/retro/Amiga/drives/wb1_3/user/bin

# Tools
CC=vc $(VCCONF)
AS=vc $(VCCONF)
LD=vc $(VCCONF)
RM=rm -rf
MKDIR=mkdir -p
COPY=cp
ADF=xdftool
ifeq ($(OS), Linux)
     UAE=fs-uae
else
     UAE=winuae
endif

# Directory structure
SRC=src/
INC=inc/
LIB=lib/
BLD=build/
OBJ=$(BLD)obj/
BIN=$(BLD)bin/

# Disble warnings
# warning 163: #pragma used
NOWARN+=-dontwarn=163
# warning 314: trailing garbage in preprocessing directive
#TODO: Clean up
# warning 113: only 0 should be assigned to pointer
#TODO: Move HW pointers to assembler.
# warning 307: member <xyz> does not have natural alignment
NOWARN+=-dontwarn=307
# warning 168: no declaration of global variable <start> before definition
#NOWARN+=-dontwarn=168
# warning 166: cast to narrow type may cause loss of precision
NOWARN+=-dontwarn=166
# warning 214: suspicious format string
#NOWARN+=-dontwarn=214
# warning 65: in function "<func>": var <var> was never used
NOWARN+=-dontwarn=65

# Flags
CPPFLAGS=-I$(INC)
CFLAGS=$(NOWARN)
ASFLAGS=
LDFLAGS=-lamiga

# Source files (anything in the src directory)
SOURCES=$(wildcard $(SRC)*)

# Binary files (exe/objs/libs)
PROG=3d_demo
OBJS=$(subst $(SRC),$(OBJ),$(addsuffix .o,$(basename $(SOURCES))))
LIBS=

.phony: all install clean $(PROG)
all: $(PROG) install

install: $(PROG)
	$(COPY) $(BIN)$(PROG) $(INSTDIR)

clean:
	$(RM) $(BIN) $(OBJ) *~ $(SRC)*~ $(INC)*~

$(PROG): $(BIN)$(PROG)

$(BIN)$(PROG): $(OBJS) | $(BIN)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJ)%.o: $(SRC)%.c | $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ)%.o: $(SRC)%.s | $(OBJ)
	$(AS) $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ)%.o: $(SRC)%.S | $(OBJ)
	$(AS) $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ)%.o: $(SRC)%.asm | $(OBJ)
	$(AS) $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ)%.o: $(SRC)%.a | $(OBJ)
	$(AS) $(ASFLAGS) $(CPPFLAGS) -c -o $@ $<

# Create directory structure
$(OBJ):
	$(MKDIR) $(OBJ)

$(BIN):
	$(MKDIR) $(BIN)

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
