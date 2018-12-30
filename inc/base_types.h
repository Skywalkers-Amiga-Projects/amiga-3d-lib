/* ____________________________________________________________________________
  | File: base_types.h                                                         |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/
#ifndef BASE_TYPES_H
#define BASE_TYPES_H

/* Integer types */
typedef          char            INT8;
typedef unsigned char           UINT8;
typedef          short           INT16;
typedef unsigned short          UINT16;
typedef          long            INT32;
typedef unsigned long           UINT32;
typedef          int             INT;
typedef unsigned int            UINT;

/* Float types */
typedef float                    FLOAT32;
typedef double                   FLOAT64;

/* Other types */
#ifdef _HAVE_BYTE_TYPE_
    typedef          char            BYTE;
    typedef unsigned char           UBYTE;
#endif
//typedef          short           WORD;
//typedef unsigned short          UWORD;
//typedef          long           DWORD;
//typedef unsigned long          UDWORD;

/* Macros */
#define MIN(x,y)            (((x)<(y))?(x):(y))
#define MAX(x,y)            (((x)<(y))?(x):(y))

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
