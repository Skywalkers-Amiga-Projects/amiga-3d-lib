/* ____________________________________________________________________________
  | File: timer.c - part of Amiga 3D Demo.                                     |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/

#include "timer.h"

#include <exec/types.h>
// VBCC warning 307: member <M> does not have natural alignment
#pragma dontwarn 307
#include <hardware/cia.h>
#include <hardware/custom.h>
#pragma popwarn
#include <hardware/dmabits.h>
#include <hardware/intbits.h>

// #define _HAVE_BYTE_TYPE_
#include "base_types.h"

extern struct Custom custom;

void busy_wait_mouse_click(void) {
  // VBCC warning 81: only 0 should be cast to pointer
  #pragma dontwarn 81
  struct CIA *ciaa = (struct CIA *)0xBFE001;
  #pragma popwarn

  // Wait for left mouse button click
  while (ciaa->ciapra & CIAF_GAMEPORT0);
}

INT16 busy_wait_frames(UINT16 t) {
  UINT16 frame = 0;
  while (frame < t) {
    if (custom.vhposr > (0xFE << 8)) {
      custom.color[0] = 0x0F0;
      while (custom.vhposr & 0xFF00);
      while (custom.vhposr <= (0x08 << 8));
      custom.color[0] = 0x000;
      frame++;
    }
  }
  return 0;
}

void busy_wait_screen(void) {
  while (custom.vhposr <= (0xFF << 8));
  while (custom.vhposr <= (0x08 << 8));
}

void busy_wait_blitter(void) {
  while (custom.dmaconr & DMAF_BLTDONE);
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
