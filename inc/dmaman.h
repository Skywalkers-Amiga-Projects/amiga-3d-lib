/* ____________________________________________________________________________
  | File: dmaman.c - part of Amiga 3D Demo.                                    |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/
#ifndef DMAMAN_H
#define DMAMAN_H

#include <hardware/dmabits.h>

#include "base_types.h"

typedef enum {
  DF_AUDIO0  = DMAF_AUD0,
  DF_AUDIO1  = DMAF_AUD1,
  DF_AUDIO2  = DMAF_AUD2,
  DF_AUDIO3  = DMAF_AUD3,
  DF_AUDIO   = DMAF_AUDIO,
  DF_DISK    = DMAF_DISK,
  DF_SPRITE  = DMAF_SPRITE,
  DF_BLITTER = DMAF_BLITTER,
  DF_COPPER  = DMAF_COPPER,
  DF_RASTER  = DMAF_RASTER
} DMAFLAGS;

int store_dma(void);
int restore_dma(void);
int disable_dma(void);
int enable_dma(DMAFLAGS);

#endif
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
