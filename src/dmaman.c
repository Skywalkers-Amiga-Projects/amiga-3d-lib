/* ____________________________________________________________________________
  | File: dmaman.c - part of Amiga 3D Demo.                                    |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/

#include <exec/types.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>

#include "dmaman.h"

extern struct Custom custom;

static UWORD saved_dma;

int store_dma(void){
    // Save active DMA
    UWORD saved_dma = custom.dmaconr;
    return 0;
}

int restore_dma(void){
    // Restore DMA settings
    custom.dmacon =  DMAF_SETCLR|DMAF_MASTER|DMAF_ALL | saved_dma;
    return 0;
}

int disable_dma(void){
    // disable DMA
    custom.dmacon =  DMAF_ALL;

    return 0;
}

int enable_dma(DMAFLAGS dma_flags){
    custom.dmacon =  DMAF_SETCLR|DMAF_MASTER | dma_flags;
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
