/* ____________________________________________________________________________
  | File: intrman.c - part of Amiga 3D Demo.                                   |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/

#include <exec/types.h>
#include <hardware/custom.h>
#include <hardware/intbits.h>

#include "intrman.h"

extern struct Custom custom;

static UWORD saved_interrupts = 0;

int store_interrupts(void){
    // Save active interrupts
    UWORD saved_interrupts = custom.intenar;

    return 0;
}

int restore_interrupts(void){
    // Restore interrupts
    custom.intena = INTF_SETCLR|INTF_INTEN|INTF_EXTER|INTF_VERTB|INTF_COPER|INTF_PORTS | saved_interrupts;

    return 0;
}

int disable_interrupts(void){
    // Disable all interrupts.
    custom.intena = INTF_INTEN|INTF_EXTER|INTF_DSKSYNC|INTF_RBF|INTF_AUD3|INTF_AUD2|INTF_AUD1|INTF_AUD0|
                    INTF_BLIT|INTF_VERTB|INTF_COPER|INTF_PORTS|INTF_SOFTINT|INTF_DSKBLK|INTF_TBE;
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
