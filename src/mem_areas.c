/* ____________________________________________________________________________
  | File: mem_areas.c - part of Amiga 3D Demo.                                 |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/

#include <exec/memory.h>
#include <proto/exec.h>

#include "base_types.h"
#include "mem_areas.h"

chipmem_content *chip_area;
fastmem_content *fast_area;

UINT32 setup_chiparea(void){
    chip_area = AllocMem(CHIPMEM_SIZE, MEMF_CHIP|MEMF_CLEAR);
    return (UINT32)chip_area;
}

UINT32 setup_fastarea(void){
    fast_area = AllocMem(FASTMEM_SIZE, /*MEMF_ANY*/MEMF_PUBLIC|MEMF_CLEAR);
    return (UINT32)fast_area;
}

void free_memareas(){
    if(chip_area != NULL){
        FreeMem(chip_area, CHIPMEM_SIZE);
    }
    if(fast_area != NULL){
        FreeMem(fast_area, FASTMEM_SIZE);
    }
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
