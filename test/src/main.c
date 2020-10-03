/* ____________________________________________________________________________
  | File: main.c - part of Amiga 3D Demo.                                      |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/

/* C standard includes */
#include <stdio.h>

/* Program includes */
#include "3d_graphics.h"
#include "blitter.h"
#include "copper.h"
#include "dmaman.h"
#include "intrman.h"
#include "mem_areas.h"
#include "timer.h"

int main(void) {
  printf("A small 3D demo...\nLet's start!!\n");

  /* Allocate memory */
  if (setup_chiparea() == ERR_NO_CHIPMEM) {
    printf("Error: cannot allocate enough chipmem\n");
    return -1;
  }
  if (setup_fastarea() == ERR_NO_FASTMEM) {
    printf("Error: cannot allocate enough fastmem\n");
    return -1;
  }

  /* Setup DMA, interrupts and copper */
  store_interrupts();
  store_dma();
  store_copper();

  busy_wait_screen();
  disable_interrupts();
  disable_dma();

  /* Show graphics */
  setup_graphics();

  // Test horizontal line left to right
  int x1 = 100, y1 = 100;
  int x2 = 150, y2 = 150;
  // drawline(x1,y1 , x2,y2);

  // Test 1st quadrant
  x1 = 100, y1 = 100;
  for (int y2 = 100; y2 >= 50; y2 -= 10) {
    int x2 = 150;
    drawline(x1, y1, x2, y2);
  }
  for (int x2 = 100; x2 <= 150; x2 += 10) {
    int y2 = 50;
    drawline(x1, y1, x2, y2);
  }

  // Test 2nd quadrant
  x1 = 100, y1 = 100;
  for (int y2 = 100; y2 >= 50; y2 -= 10) {
    int x2 = 50;
    drawline(x1, y1, x2, y2);
  }
  for (int x2 = 100; x2 >= 50; x2 -= 10) {
    int y2 = 50;
    drawline(x1, y1, x2, y2);
  }

  // Test 3rd quadrant
  x1 = 100, y1 = 100;
  for (int y2 = 100; y2 <= 150; y2 += 10) {
    int x2 = 50;
    drawline(x1, y1, x2, y2);
  }
  for (int x2 = 100; x2 >= 50; x2 -= 10) {
    int y2 = 150;
    drawline(x1, y1, x2, y2);
  }

  // Test 4th quadrant
  x1 = 100, y1 = 100;
  for (int y2 = 100; y2 <= 150; y2 += 10) {
    int x2 = 150;
    drawline(x1, y1, x2, y2);
  }
  for (int x2 = 100; x2 <= 150; x2 += 10) {
    int y2 = 150;
    drawline(x1, y1, x2, y2);
  }

  /* Wait for end */
  // busy_wait_frames(120);
  busy_wait_mouse_click();

  /* Restore DMA, interrupts and copper */
  busy_wait_screen();
  disable_dma();
  restore_copper();
  restore_dma();
  restore_interrupts();

  free_memareas();
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
