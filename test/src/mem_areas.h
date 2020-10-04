/* ____________________________________________________________________________
  | File: mem_areas.h - part of Amiga 3D Demo.                                 |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/
#ifndef MEM_AREAS_H
#define MEM_AREAS_H
/* To access memeory areas add the following in each file that need access.
 * extern chipmem_content *chip_area;
 * extern fastmem_content *fast_area;
 */
#include "exec/types.h"

// #define _HAVE_BYTE_TYPE_
#include "base_types.h"

#define HIGH_ADDRESS(addr) ((USHORT)0xFFFF & (((ULONG)(addr)) >> 0x10))
#define LOW_ADDRESS(addr)  ((USHORT)0xFFFF & (ULONG)(addr))

#define SCREEN_WIDTH  (320)
#define SCREEN_HEIGHT (200)
#define BITPLANE_SZ(w, h) ((w) * (h) / 8)
#define MAX_COPPER_LIST_LEN (1024)

#define CHIPMEM_SIZE (100 * 1024)
#define FASTMEM_SIZE (100 * 1024)

#define ERR_NO_CHIPMEM (0)
#define ERR_NO_FASTMEM (0)

/* Contents of chip memory area */
// VBCC warning 307: member <M> does not have natural alignment
#pragma dontwarn 307
typedef struct {
  UBYTE  bit_plane0[BITPLANE_SZ(SCREEN_WIDTH, SCREEN_HEIGHT)];
  USHORT copperlist[MAX_COPPER_LIST_LEN];
} chipmem_content;
#pragma popwarn

/* Contents of fast memory area */
typedef struct {
  unsigned char data[1024];
} fastmem_content;

UINT32 setup_chiparea(void);
UINT32 setup_fastarea(void);
void   free_memareas(void);

#endif
/*___________________________________________________________________________
 | License:                                                                  |
 | This program is free software: you can redistribute it and/or modify      |
 | it under the terms of the GNU General Public License version 2 as         |
 | published by the Free Software Foundation.                                |
 |                                                                           |
 | This program is distributed in the hope that it will be useful,           |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of            |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             |
 | GNU General Public License for more details.                              |
 |                                                                           |
 | You should have received a copy of the GNU General Public License         |
 | along with this program. If not, see <http://www.gnu.org/licenses/>       |
 | or write to the Free Software Foundation, Inc.,                           |
 | 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               |
 |___________________________________________________________________________|
*/
