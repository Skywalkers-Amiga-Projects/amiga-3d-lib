/* ____________________________________________________________________________
  | File: 3d_graphics.c - part of Amiga 3D Demo.                               |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/

#include "3d_graphics.h"

// VBCC warning 307: member <M> does not have natural alignment
#pragma dontwarn 307
#include <hardware/custom.h>
#pragma popwarn

#include "blitter.h"
#include "copper.h"
#include "dmaman.h"

#define HIGH_ADDRESS(addr) ((USHORT)0xFFFF & (((ULONG)(addr)) >> 0x10))
#define LOW_ADDRESS(addr)  ((USHORT)0xFFFF & (ULONG)(addr))

extern struct Custom    custom;

void setup_graphics(UBYTE  *bitplanes[],
                    UINT8   bpl_count,
                    USHORT *copper_list,
                    UINT16 copls_maxlen) {
  setup_blitter();
  setup_bitplanes(bitplanes, bpl_count, copper_list, copls_maxlen);
}

void setup_bitplanes(UBYTE  *bitplanes[],
                     UINT8  bpl_count,
                     USHORT *copper_list,
                     UINT16 copls_maxlen) {
  // Init screen display

  // Set screen size - row and column are 9 bit values counting low-res
  // (320*256) pixels. Display size = 0x108 - 0x40 = 0xC8  = 200 rows,
  //                                  0x1C1 - 0x81 = 0x140 = 320 columns
  // Display window start at row 0x40 and column 0x81.
  custom.diwstrt = 0x4081;
  // Display window ends at row 0x108 and column 0x1C1.
  custom.diwstop = 0x08C1;

  // Set DMA display timing
  // What does it mean?? 0x81 / 2 - 8 = 0x38
  custom.ddfstrt = 0x38;
  custom.ddfstop = 0xD0;

  // Clear screen
  blt_clear(bitplanes[0]);

  // Set even and odd bitplane modulo
  custom.bpl1mod = 0x0;
  custom.bpl2mod = 0x0;

  // Set new copper list
  init_copper_list(copper_list, copls_maxlen);

  // Set bitplane addresses
  copper_list[1] = HIGH_ADDRESS(bitplanes[0]);
  copper_list[3] = LOW_ADDRESS(bitplanes[0]);

  // Enable copper and screen DMA
  enable_dma(DF_COPPER | DF_RASTER);
}

void setup_blitter(void) {
  // Enable blitter DMA
  enable_dma(DF_BLITTER);
}

void plot(UINT16 x, UINT16 y, UINT16 z, UBYTE *bitplane) {
  UINT32 index = y * (320 / 8) + (x / 8);
  // VBCC warning 166: cast to narrow type may cause loss of precision
  #pragma dontwarn 166
  UINT8  bit = 1 << (x & 7);
  #pragma popwarn
  bitplane[index] |= bit;
}

void drawline(int x1, int y1, int x2, int y2, UBYTE *bitplane) {
  blt_line(x1, y1, x2, y2, bitplane);
}

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
