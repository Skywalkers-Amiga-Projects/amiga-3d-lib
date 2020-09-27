/* ____________________________________________________________________________
  | File: copper.c - part of Amiga 3D Demo.                                    |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/

#include "copper.h"

#include <exec/libraries.h>
#include <exec/types.h>
#include <graphics/gfx.h>
#include <graphics/gfxbase.h>
#include <hardware/custom.h>
#include <proto/exec.h>

#include "dmaman.h"
#include "mem_areas.h"

extern struct Custom    custom;
extern chipmem_content *chip_area;

static struct copinit *saved_copperlist;

int store_copper(void) {
  // Save current copper list
  struct Library *gfx_lib = OpenLibrary(GFXNAME, LIBRARY_VERSION);
  struct GfxBase *gfx_base = (struct GfxBase *)gfx_lib;
  saved_copperlist = gfx_base->copinit;
  if (gfx_lib != NULL) {
    CloseLibrary(gfx_lib);
  }

  return 0;
}

int restore_copper(void) {
  // Restore copper list
  custom.cop1lc = (ULONG)saved_copperlist;

  return 0;
}

typedef enum {
  COP_MEM = 0,
  COP_WAIT = 1,
  CL_END = 0xFFFF
} COPPER_INSTRUCTION;

typedef enum {
  CL_BLACK = 0x0000,
  CL_WHITE = 0x0FFF
} COLOR;

typedef enum {
  CL_BPLCON0 = 0x100,  // Bitplane control register (misc. control bits)
  CL_BPLCON1 = 0x102,  // Bitplane control register (horizontal scroll control)
  CL_BPLCON2 = 0x104,  // Bitplane control register (video priority control)
} COPPER_ADDRESS;

int init_copper_list() {
  USHORT copper_list_data[] = {
    0x00E0,     0x0000,                      // Set bitplane addresses
    0x00E2,     0x0000, CL_BPLCON0, 0x1000,  // x bitplane(s) on
    CL_BPLCON1, 0x0000, CL_BPLCON2, 0x0000,
    CL_END,     0xFFFE  // Copper list end.
  };
  for (int i = 0; i < MAX_COPPER_LIST_LEN; i += 2) {
    USHORT cop_inst = copper_list_data[i];
    USHORT cop_data = copper_list_data[i + 1];
    chip_area->copperlist[i] = cop_inst;
    chip_area->copperlist[i + 1] = cop_data;
    if (cop_inst == 0xFFFF) break;
  }
  custom.cop1lc = (ULONG)chip_area->copperlist;

  return 0;
}

/*_____________________________________________________________________________
 | License:                                                                    |
 | This program is free software: you can redistribute it and/or modify        |
 | it under the terms of the GNU General Public License version 2 as           |
 | published by the Free Software Foundation.                                  |
 |                                                                             |
 | This program is distributed in the hope that it will be useful,             |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of              |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               |
 | GNU General Public License for more details.                                |
 |                                                                             |
 | You should have received a copy of the GNU General Public License           |
 | along with this program. If not, see <http://www.gnu.org/licenses/>         |
 | or write to the Free Software Foundation, Inc.,                             |
 | 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 |
 |_____________________________________________________________________________|
*/
