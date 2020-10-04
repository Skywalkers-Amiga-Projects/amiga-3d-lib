/* ____________________________________________________________________________
  | File: blitter.c - part of Amiga 3D Demo.                                   |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/
#ifndef BLIITER_H
#define BLIITER_H
#include <exec/types.h>

#define BLT_CLEAR (0x0000)

void blt_clear(UBYTE *bitplane);
void blt_fill(UWORD pattern, UBYTE *bitplane);

void blt_line(int x1, int y1, int x2, int y2, UBYTE *bitplane);
void blt_line2(int x1, int y1, int x2, int y2, UBYTE *bitplane);

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
