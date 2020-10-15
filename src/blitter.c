/* ____________________________________________________________________________
  | File: blitter.c - part of Amiga 3D Demo.                                   |
  | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>                |
  | Copyright: (C) 2018 skywalker. All rights reserved.                        |
  |____________________________________________________________________________|
*/

#include "blitter.h"

// VBCC warning 306: padding bytes behind member <M>
#pragma dontwarn 306
// VBCC warning 307: member <M> does not have natural alignment
#pragma dontwarn 307
#include <hardware/blit.h>
#pragma popwarn
#pragma popwarn
// VBCC warning 307: member <M> does not have natural alignment
#pragma dontwarn 307
#include <hardware/custom.h>
#pragma popwarn
#include <hardware/dmabits.h>

#include <stdlib.h>

#include "base_types.h"

extern struct Custom    custom;

void blt_clear(UBYTE *bitplane) {
  blt_fill(BLT_CLEAR, bitplane);
}

void blt_fill(UWORD pattern, UBYTE *bitplane) {
  // busy_wait_blitter();
  while (custom.dmaconr & DMAF_BLTDONE);

  custom.bltcon0 = 0x0102;  // Use C as fill source (Logic Function 2: !A!BC)
  custom.bltcon1 = 0x0000;

  custom.bltafwm = 0x0000;
  custom.bltalwm = 0x0000;

  custom.bltapt = NULL;
  custom.bltbpt = NULL;
  custom.bltcpt = NULL;
  // VBCC warning 167: pointer cast may cause alignment problems
  #pragma dontwarn 167
  custom.bltdpt = (APTR)bitplane;
  #pragma popwarn

  custom.bltamod = 0x0000;
  custom.bltbmod = 0x0000;
  custom.bltcmod = 0x0000;
  custom.bltdmod = 0x0000;

  custom.bltadat = 0x0000;
  custom.bltbdat = 0x0000;
  custom.bltcdat = pattern;  // Set fill pattern
  // custom.bltddat = 0x0000;

  // Start blit operation
  custom.bltsize = 0x3214;
}

void simpleline(__reg("d0") UBYTE* bplpt);
void line(__reg("d0") UINT32 x1, __reg("d1") UINT32 y1,
          __reg("d2") UINT32 x2, __reg("d3") UINT32 y2,
          __reg("a0") UBYTE* color);

void blt_line(int x1, int y1, int x2, int y2, UBYTE *bitplane) {
  // busy_wait_blitter();
  while (custom.dmaconr & DMAF_BLTDONE);

  // blt_line2(x1, y1, x2, y2);

  UINT start_index = x1 / 8 + (y1 * (320 / 8));
  // simpleline(chip_area->bit_plane0/* + start_index*/);
  line(x1, y1, x2, y2, bitplane + start_index);
}

void blt_line2(int x1, int y1, int x2, int y2, UBYTE *bitplane) {
  x1 = 100;
  y1 = 100;
  x2 = 150;
  y2 = 150;
  // busy_wait_blitter();
  while (custom.dmaconr & DMAF_BLTDONE);

  /* The SRCA, SRCC, and DEST bits of BLTCON0  should be set to one, and the
     SRCB flag should be set to zero. */
  /* The A  shift  value should be set to the x
     coordinate of the first point (x1) modulo 15. */
  // VBCC warning 166: cast to narrow type may cause loss of precision
  #pragma dontwarn 166
  UINT16 ashift = x1 % 15;
  custom.bltcon0 = (ashift << 12) | 0x0BCA;  // USEA | USEC | USED = 0xB;
  #pragma popwarn
  /* The logic function remains.  The C  DMA channel  represents the original
     source, the A channel the bit to set in the line, and the B channel the
     pattern to draw.  Thus, to draw a line, the function AB | !A is the most
     common.
     To draw the line using exclusive-or mode, so it can be easily erased by
     drawing it again, the function AB!C | !AC can be used.

    BIT   A  B  C  -> D
     0    0  0  0     0
     1    0  0  1     1
     2    0  1  0     0
     3    0  1  1     1 = !AC (Pixel already set, src = C)
     4    1  0  0     0
     5    1  0  1     0
     6    1  1  0     1
     7    1  1  1     1 = AB (Set line pixel, line = A, texture pattern = B)
    --------------------
     !AC | AB = CA
  */

  /* The OVFLAG should be cleared. */
  /* If only a single bit per horizontal row is desired, the ONEDOT bit of
     BLTCON1 should be set; otherwise it should be cleared.*/
  // VBCC warning 166: cast to narrow type may cause loss of precision
  #pragma dontwarn 166
  INT16  dx = x2 - x1;
  INT16  dy = y2 - y1;
  UINT16 adx = abs(dx);
  UINT16 ady = abs(dy);
  #pragma popwarn

  int octant = 0;
  if (dx >= 0 && dy < 0) { /* Quadrant 0, i.e. octant 0 or 1 */
    octant = (adx >= ady) ? OCTANT0 : OCTANT1;
  } else if (dx < 0 && dy < 0) { /* Quadrant 1, i.e. octant 2 or 3 */
    octant = (adx < ady) ? OCTANT2 : OCTANT3;
  } else if (dx < 0 && dy >= 0) { /* Quandrant 2, i.e. octant 4 or 5 */
    octant = (adx >= ady) ? OCTANT4 : OCTANT5;
  } else if (dx >= 0 && dy >= 0) { /* Quadrant 3, i.e. octant 6 or 7 */
    octant = (adx < ady) ? OCTANT6 : OCTANT7;
  }

  // VBCC warning 166: cast to narrow type may cause loss of precision
  #pragma dontwarn 166
  dx = MAX(adx, ady);
  dy = MIN(adx, ady);
  #pragma popwarn
  /* Set the A  pointer register to 4 * dy - 2 * dx. If this value
     is negative, we set the sign bit (SIGNFLAG in  BLTCON1 ), otherwise we
     clear it. */
  /* The B shift value should be set to the bit number at which to start
     the line texture (zero means the last significant bit.)*/
  INT32  apt = (4 * dy) - (2 * dx);
  // VBCC warning 166: cast to narrow type may cause loss of precision
  #pragma dontwarn 166
  UINT16 sign = (apt < 0) ? SIGNFLAG : 0;
  #pragma popwarn
  custom.bltcon1 = sign | octant | LINEMODE;
  custom.bltapt = (APTR)(UINT32)apt;

  /* Set the A modulo register  to 4 * (dy - dx) and the B
     modulo register  to 4 * dy. */
  custom.bltamod = 4 * (dy - dx);
  custom.bltbmod = 4 * dy;

  /* The A  data register  should be preloaded with $8000. */
  custom.bltadat = 0x8000;
  custom.bltbpt = NULL; /* Unused */

  /* Both word  masks should be set to $FFFF. */
  custom.bltafwm = 0xFFFF;
  custom.bltalwm = 0xFFFF;

  /* The B  data register  should be initialized with the line texture pattern,
     if any, or $FFFF for a solid line. The B shift value should be set to
     the bit number at which to start the line texture (zero means the last
     significant bit.)*/
  custom.bltbdat = 0xFFFF;

  /* The C and D pointer registers should be initialized to the word
     containing the first pixel of the line; the C and D modulo registers
     should be set to the width of the bitplane in bytes. */
  UINT start_index = x1 / 8 + (y1 * (320 / 8));
  // VBCC warning 167: pointer cast may cause alignment problems
  #pragma dontwarn 167
  custom.bltcpt = (APTR)(bitplane + start_index);
  custom.bltdpt = (APTR)(bitplane + start_index);
  #pragma popwarn
  custom.bltcmod = 0x0028;
  custom.bltdmod = 0x0028;

  // custom.bltcdat = 0x0000;

  // Start blit operation
  /* Set the blit height to the length of the line, which is dx + 1. The
     width must be set to two for all line drawing. */
  // VBCC warning 166: cast to narrow type may cause loss of precision
  #pragma dontwarn 166
  custom.bltsize = ((dx + 1) << 6) | 0x02;
  #pragma popwarn
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

#if 0
void blt_line3(int x1, int y1, int x2, int y2){
    busy_wait_blitter();

    x1 = 100;
    y1 = 100;
    x2 = 100;
    y2 = 150;

    /* Preliminary setup:
       The line goes from (x1,y1) to (x2,y2).

       dx = max(abs(x2 - x1), abs(y2 - y1)) ;
       dy = min(abs(x2 - x1), abs(y2 - y1)) ;
    */
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    dx = (dx > dy)?dx:dy;
    dy = (dx < dy)?dx:dy;

    /* Register setup: */
    /*
      BLTADAT  = $8000
      BLTBDAT  = line texture pattern ($FFFF for a solid line)
    */
    custom.bltadat = 0x8000;
    custom.bltbdat = 0xFFFF;

    /*
      BLTAFWM  = $FFFF
      BLTALWM  = $FFFF
    */
    custom.bltafwm = 0xFFFF;
    custom.bltalwm = 0xFFFF;

    /*
      BLTAMOD  = 4 * (dy - dx)
      BLTBMOD  = 4 * dy
      BLTCMOD  = width of the bitplane in bytes
      BLTDMOD  = width of the bitplane in bytes
    */
    custom.bltamod = 4*(dy-dx);
    custom.bltbmod = 4*dy;
    custom.bltcmod = 320/8;
    custom.bltdmod = 320/8;

    /*
      BLTAPT   = (4 * dy) - (2 * dx)
      BLTBPT   = unused
      BLTCPT   = word containing the first pixel of the line
      BLTDPT   = word containing the first pixel of the line
    */
    custom.bltapt = (4*dy) - (2*dx);
    custom.bltbpt = NULL; /* Unused */
    UINT start_index = x1 + (y1*(320/8));
    custom.bltcpt = chip_area->bit_plane0 + start_index;
    custom.bltdpt = chip_area->bit_plane0 + start_index;

    /*
      BLTCON0  bits 15-12 = x1 modulo 15
      BLTCON0  bits SRCA, SRCC, and SRCD = 1
      BLTCON0  bit  SRCB = 0
          If exclusive-or line mode:
              then BLTCON0  LF control byte  = AB!C + !AC
              else BLTCON0  LF control byte  = AB + !AC
    */
    custom.bltcon0 = ((x1%16) << 12) | 0x0BCA;

    /*
      BLTCON1  bit  LINEMODE  = 1
      BLTCON1  bit  OVFLAG  = 0
      BLTCON1  bits 4-2 =  octant number  from table
      BLTCON1  bits 15-12 = start bit for line texture (0 = last significant
      If (((4 * dy) - (2 * dx)) < 0):                bit)
          then  BLTCON1  bit  SIGNFLAG  = 1
          else  BLTCON1  bit  SIGNFLAG  = 0
      If one pixel/row:
          then  BLTCON1  bit  ONEDOT  = 1
          else  BLTCON1  bit  ONEDOT  = 0
    */
    UINT16 sign = ( ((4*dy) - (2*dx)) < 0 ) ? /*SIGNFLAG*/ (1<<6) : 0;
    int octant =  0 << 2;
    custom.bltcon1 = sign | octant | LINEMODE;

    /*
      BLTSIZE  bits 15-6 = dx + 1
      BLTSIZE  bits  5-0 = 2
    */
    custom.bltsize = ((dx+1) << 6) | 0x02;
}
#endif
/*


Preliminary setup:

   The line goes from (x1,y1) to (x2,y2).

   dx = max(abs(x2 - x1), abs(y2 - y1)) ;
   dy = min(abs(x2 - x1), abs(y2 - y1)) ;

Register setup:

   BLTADAT  = $8000
   BLTBDAT  = line texture pattern ($FFFF for a solid line)

   BLTAFWM  = $FFFF
   BLTALWM  = $FFFF

   BLTAMOD  = 4 * (dy - dx)
   BLTBMOD  = 4 * dy
   BLTCMOD  = width of the bitplane in bytes
   BLTDMOD  = width of the bitplane in bytes

   BLTAPT   = (4 * dy) - (2 * dx)
   BLTBPT   = unused
   BLTCPT   = word containing the first pixel of the line
   BLTDPT   = word containing the first pixel of the line

   BLTCON0  bits 15-12 = x1 modulo 15
   BLTCON0  bits SRCA, SRCC, and SRCD = 1
   BLTCON0  bit  SRCB = 0
            If exclusive-or line mode:                  _   _
                     then BLTCON0  LF control byte  = ABC + AC
                                                           _
                     else BLTCON0  LF control byte  = AB + AC

   BLTCON1  bit  LINEMODE  = 1
   BLTCON1  bit  OVFLAG  = 0
   BLTCON1  bits 4-2 =  octant number  from table
   BLTCON1  bits 15-12 = start bit for line texture (0 = last significant
            If (((4 * dy) - (2 * dx)) < 0):                bit)
                     then  BLTCON1  bit  SIGNFLAG  = 1
                     else  BLTCON1  bit  SIGNFLAG  = 0
            If one pixel/row:
                     then  BLTCON1  bit  ONEDOT  = 1
                     else  BLTCON1  bit  ONEDOT  = 0

   BLTSIZE  bits 15-6 = dx + 1
   BLTSIZE  bits  5-0 = 2








;
; simpleline.asm
;
;   This example uses the line draw mode of the blitter
;   to draw a line.  The line is drawn with no pattern
;   and a simple `or' blit into a single bitplane.
;   (Link with amiga.lib)
;
;   Input:  d0=x1 d1=y1 d2=x2 d3=y2 d4=width a0=aptr
;
        include 'exec/types.i'
        include 'hardware/custom.i'
        include 'hardware/blit.i'
        include 'hardware/dmabits.i'

        include 'hardware/hw_examples.i'
;
        xref    _custom
;
        xdef    simpleline
;
;   Our entry point.
;
simpleline:
        lea     _custom,a1      ; snarf up the custom address register

        dx =
        sub.w   d0,d2           ; calculate dx
        bmi     xneg            ; if negative, octant is one of [3,4,5,6]
        sub.w   d1,d3           ; calculate dy   ''   is one of [1,2,7,8]
        bmi     yneg            ; if negative, octant is one of [7,8]
        cmp.w   d3,d2           ; cmp |dx|,|dy|  ''   is one of [1,2]
        bmi     ygtx            ; if y>x, octant is 2
        moveq.l #OCTANT1+LINEMODE,d5    ; otherwise octant is 1
        bra     lineagain       ; go to the common section
ygtx:
        exg     d2,d3           ; X must be greater than Y
        moveq.l #OCTANT2+LINEMODE,d5    ; we are in octant 2
        bra     lineagain       ; and common again.
yneg:
        neg.w   d3              ; calculate abs(dy)
        cmp.w   d3,d2           ; cmp |dx|,|dy|, octant is [7,8]
        bmi     ynygtx          ; if y>x, octant is 7
        moveq.l #OCTANT8+LINEMODE,d5    ; otherwise octant is 8
        bra     lineagain
ynygtx:
        exg     d2,d3           ; X must be greater than Y
        moveq.l #OCTANT7+LINEMODE,d5    ; we are in octant 7
        bra     lineagain
xneg:
        neg.w   d2              ; dx was negative! octant is [3,4,5,6]
        sub.w   d1,d3           ; we calculate dy
        bmi     xyneg           ; if negative, octant is one of [5,6]
        cmp.w   d3,d2           ; otherwise it's one of [3,4]
        bmi     xnygtx          ; if y>x, octant is 3
        moveq.l #OCTANT4+LINEMODE,d5    ; otherwise it's 4
        bra     lineagain
xnygtx:
        exg     d2,d3           ; X must be greater than Y
        moveq.l #OCTANT3+LINEMODE,d5    ; we are in octant 3
        bra     lineagain
xyneg:
        neg.w   d3              ; y was negative, in one of [5,6]
        cmp.w   d3,d2           ; is y>x?
        bmi     xynygtx         ; if so, octant is 6
        moveq.l #OCTANT5+LINEMODE,d5    ; otherwise, octant is 5
        bra     lineagain
xynygtx:
        exg     d2,d3           ; X must be greater than Y
        moveq.l #OCTANT6+LINEMODE,d5    ; we are in octant 6

lineagain:
        mulu.w  d4,d1           ; Calculate y1 * width
        ror.l   #4,d0           ; move upper four bits into hi word
        add.w   d0,d0           ; multiply by 2
        add.l   d1,a0           ; ptr += (x1 >> 3)
        add.w   d0,a0           ; ptr += y1 * width
        swap    d0              ; get the four bits of x1
        or.w    #$BFA,d0        ; or with USEA, USEC, USED, F=A+C
        lsl.w   #2,d3           ; Y = 4 * Y
        add.w   d2,d2           ; X = 2 * X
        move.w  d2,d1           ; set up size word
        lsl.w   #5,d1           ; shift five left
        add.w   #$42,d1         ; and add 1 to height, 2 to width

        btst    #DMAB_BLTDONE-8,DMACONR(a1)     ; safety check
waitblit:
        btst    #DMAB_BLTDONE-8,DMACONR(a1)     ; wait for blitter
        bne     waitblit

        move.w  d3,BLTBMOD(a1)  ; B mod = 4 * Y
        sub.w   d2,d3
        ext.l   d3
        move.l  d3,BLTAPT(a1)   ; A ptr = 4 * Y - 2 * X
        bpl     lineover        ; if negative,
        or.w    #SIGNFLAG,d5    ; set sign bit in con1
lineover:
        move.w  d0,BLTCON0(a1)  ; write control registers
        move.w  d5,BLTCON1(a1)
        move.w  d4,BLTCMOD(a1)  ; C mod = bitplane width
        move.w  d4,BLTDMOD(a1)  ; D mod = bitplane width
        sub.w   d2,d3
        move.w  d3,BLTAMOD(a1)  ; A mod = 4 * Y - 4 * X
        move.w  #$8000,BLTADAT(a1)      ; A data = 0x8000
        moveq.l #-1,d5          ; Set masks to all ones
        move.l  d5,BLTAFWM(a1)  ; we can hit both masks at once
        move.l  a0,BLTCPT(a1)   ; Pointer to first pixel to set
        move.l  a0,BLTDPT(a1)
        move.w  d1,BLTSIZE(a1)  ; Start blit
        rts                     ; and return, blit still in progress.
        end

*/
