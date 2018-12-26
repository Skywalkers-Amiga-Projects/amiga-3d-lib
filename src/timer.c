/* ____________________________________________________________________________
  | File: timer.c - part of Amiga 3D Demo.                                      |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/

#include <hardware/custom.h>
#include "base_types.h"
#include "timer.h"

extern struct Custom custom;

INT16 busy_wait_frames(UINT16 t){
    UINT16 frame = 0;
    while(frame < t){
        if(custom.vhposr > (0xF7<<8)){
            custom.color[0] =  0x0F0;
            while(custom.vhposr&0xFF00);
            custom.color[0] =  0x000;
            frame++;
        }
    }
    return 0;
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
