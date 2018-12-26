/* ____________________________________________________________________________
  | File: 3d_graphics.c - part of Amiga 3D Demo.                               |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/

#include <hardware/custom.h>

#include "3d_graphics.h"
#include "mem_areas.h"
#include "dmaman.h"
#include "copman.h"

extern struct Custom custom;
extern chipmem_content *chip_area;

void setup_graphics(void){
  setup_bitplanes();
}

void setup_bitplanes(void){
    // Init screen display

    // Set screen size - row and column are 9 bit values counting low-res (320*256) pixels.
    // Display size = 0x108 - 0x40 = 0xC8 = 200 rows, 0x1C1 - 0x81 = 0x140 = 320 columns
    custom.diwstrt = 0x4081; // Display window start at row 0x40 and column 0x81.
    custom.diwstop = 0x08C1; // Display window ends at row 0x108 and column 0x1C1.

    // Set DMA display timing
    // What does it mean?? 0x81 / 2 - 8 = 0x38
    custom.ddfstrt = 0x38;
    custom.ddfstop = 0xD0;

    // Fill screen with pattern
    for(int i = 0; i < BITPLANE_SZ(SCREEN_WIDTH,SCREEN_HEIGHT); i++){
	chip_area->bit_plane0[i] =  0xA5;
    }

    // Set even and odd bitplane modulo
    custom.bpl1mod = 0x0;
    custom.bpl2mod = 0x0;

    // Set new copper list
    init_copper_list();

    // Set bitplane addresses
    chip_area->copperlist[1] = HIGH_ADDRESS(chip_area->bit_plane0);
    chip_area->copperlist[3] = LOW_ADDRESS(chip_area->bit_plane0);

    // Enable copper and screen DMA
    enable_dma(DF_COPPER|DF_RASTER);
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