;  ___________________________________________________________________________
; | File: line.s - part of Amiga 3D Demo.                                     |
; | Author: skywalker aka J.Karlsson <j.karlsson@retrocoder.se>               |
; | Copyright (C) 2018 skywalker. All rights reserved.                        |
; |___________________________________________________________________________|

        include 'hardware.i'
_custom equ $dff000
_ciaa   equ $bfe001

        xdef    _line
_line:
        movem.l d0-d7/a0-a6,-(sp)

;;    x1 = 100; y1 = 100;
;;    x2 = 150; y2 = 150;
;;        move.l  #100, d0
;;        move.l  #100, d1

;;        move.l  #150, d2
;;        move.l  #150, d3
;;    UINT16 ashift = x1%15;
;;    custom.bltcon0 = (ashift << 12) | 0x0BCA; // USEA | USEC | USED = 0xB;
        move.l  d0, d4
        divu    #15, d4
        andi.l  #$000F0000, d4  ; Get the remainder
        lsr.l   #4, d4          ; Shift remainder 4 bits down into bits 12 to 15.
        ori.l   #$BCA, d4
        move.w  d4, _custom+bltcon0

;;    INT16 dx = x2 - x1;
;;    INT16 dy = y2 - y1;
;;    UINT16 adx = abs(dx);
;;    UINT16 ady = abs(dy);
        move.l  d2, d4
        sub.l   d0, d4          ; d4 = dx = x2 - x1
        move.l  d4, d6
        bpl     dx_pos
        neg.l   d6              ; d6 = abs(dx)
dx_pos:
        move.l  d3, d5
        sub.l   d1, d5          ; d5 = dy = y2 - y1
        move.l  d5, d7
        bpl     dy_pos
        neg.l   d7              ; d7 = abs(dy)
dy_pos:

;;    int octant = 0;
;;    if(dx >= 0 && dy < 0){ /* Quadrant 0, i.e. octant 0 or 1 */
;;        octant = (adx >= ady)?OCTANT0:OCTANT1;
;;    } else if(dx < 0 && dy < 0){ /* Quadrant 1, i.e. octant 2 or 3 */ 
;;        octant = (adx < ady)?OCTANT2:OCTANT3;
;;    } else if(dx < 0 && dy >= 0){ /* Quandrant 2, i.e. octant 4 or 5 */
;;        octant = (adx >= ady)?OCTANT4:OCTANT5;
;;    } else if (dx >= 0 && dy >= 0){ /* Quadrant 3, i.e. octant 6 or 7 */
;;        octant = (adx < ady)?OCTANT6:OCTANT7;
        ;;    }
OCT0 equ 24
OCT1 equ 4
OCT2 equ 12
OCT3 equ 28
OCT4 equ 20
OCT5 equ 8
OCT6 equ 0
OCT7 equ 16
        cmpi    #0, d4
        bge     dx_ge_zero
        cmpi    #0, d5
        bge     dx_lt_zero_dy_ge_zero
        cmp     d6, d7
        bge     ady_gt_adx1
        move.w  #OCT3, d2
        bra     OCT_END
ady_gt_adx1:
        move.w  #OCT2, d2
        bra     OCT_END

dx_lt_zero_dy_ge_zero:
        cmp     d6, d7
        bge     ady_gt_adx2
        move.w  #OCT4, d2
        bra     OCT_END
ady_gt_adx2:
        move.w  #OCT5, d2
        bra     OCT_END
        
dx_ge_zero:
        cmpi    #0, d5
        bge     dx_ge_zero_dy_ge_zero
        cmp     d6, d7
        bge     ady_gt_adx3
        move.w  #OCT0, d2
        bra     OCT_END
ady_gt_adx3:
        move.w  #OCT1, d2
        bra     OCT_END

dx_ge_zero_dy_ge_zero:
        cmp     d6, d7
        bge     ady_gt_adx4
        move    #OCT7, d2
        bra     OCT_END
ady_gt_adx4:
        move    #OCT6, d2

OCT_END:
;;    dx = MAX(adx, ady);
;;    dy = MIN(adx, ady);
        cmp.l   d6, d7
        bge     max_dy
        move.l  d6, d4
        move.l  d7, d5
        bra     max_end
max_dy:
        move.l  d7, d4
        move.l  d6, d5
max_end:

;;    INT32 apt = (4*dy) - (2*dx);
        move.l  d5, d6
        asl.l   #2, d6       ; d6 = 4*dy
        move.l  d4, d7
        asl.l   #1, d7       ; d7 = 2*dx
        sub.l   d7, d6       ; d6 = (4*dy) - (2*dx)
;;    UINT16 sign = (apt < 0)?SIGNFLAG:0;
;;    custom.bltcon1 = sign | octant | LINEMODE;
;;        move.w #OCT1, d2
        ori.l     #1, d2
        move.w d2, _custom+bltcon1
;;    custom.bltapt = (APTR)(UINT32)apt;
        move.l  d6, _custom+bltapt

;;    custom.bltamod = 4*(dy-dx);
;;    custom.bltbmod = 4*dy;
        move.l  d5, d6
        sub.l   d4, d6
        asl.l   #2, d6          ; d6 = 4*(dy-dx)
        move.w  d6, _custom+bltamod
        move.l  d5, d6
        asl.l   #2, d6          ; d6 = 4*dy
        move.w  d6, _custom+bltbmod

;;    custom.bltadat = 0x8000;
;;    custom.bltbpt = NULL; /* Unused */
        move.w #$8000, _custom+bltadat
        move.l #0, _custom+bltbpt

;;    custom.bltafwm = 0xFFFF;
;;    custom.bltalwm = 0xFFFF;
        move.w #$FFFF, _custom+bltafwm
        move.w #$FFFF, _custom+bltalwm

;;    custom.bltbdat = 0xFFFF;
        move.w #$FFFF, _custom+bltbdat

;;    UINT start_index = x1/8 + (y1*(320/8));
;;    custom.bltcpt = (APTR)(chip_area->bit_plane0 + start_index);
;;    custom.bltdpt = (APTR)(chip_area->bit_plane0 + start_index);
        move.l a0, _custom+bltcpt
        move.l a0, _custom+bltdpt

;;    custom.bltcmod = 0x0028;
;;    custom.bltdmod = 0x0028;
        move.w #$0028, _custom+bltcmod
        move.w #$0028, _custom+bltdmod

;;    custom.bltsize = ((dx+1) << 6) | 0x02
        move.l  d4, d6
        addq.l  #1, d6
        asl.l   #6, d6
        ori.l   #$02, d6
        move.w  d6, _custom+bltsize

;;waitmouse:
;;        cmpi.w  #$FF00, _custom+vhposr
;;        bls     end_wait       ; vhposr <= $FF00 (unsigned)
;;        move.w a0, _custom+color
;;
;;wait_raster:
;;        cmpi.w  #$0800, _custom+vhposr
;;        ble     wait_raster    ; vhposr <= $0800 (signed)
        move.w #$0000, _custom+color
;;
;;end_wait:
;;        btst #6,_ciaa+ciapra
;;        bne waitmouse

        movem.l (sp)+, d0-d7/a0-a6 ;
        rts
        end

;;;---------------------------------------------------------------
        movem.l d0-d7/a0-a6,-(sp)

        move.w  #$5555, (a0)
        ;; Input:  d0=x1 d1=y1 d2=x2 d3=y2 d4=width a0=aptr
        move.w  #100, d0
        move.w  #100, d1
        move.w  #150, d2
        move.w  #150, d3

        ;; Preliminary setup:
        ;;   The line goes from (x1,y1) to (x2,y2).
        ;;   dx = max(abs(x2 - x1), abs(y2 - y1)) ;
        ;;   dy = min(abs(x2 - x1), abs(y2 - y1)) ;
        move.l  d2, d4          ; d4 = dx = x2 - x1
        sub.l   d0, d4
        bgt     pos_dx
        neg     d4              ; dx = abs(dx)
pos_dx:
        move.l  d3, d5          ; d5 = dy = y2 - y1
        sub.l   d1, d5
        bgt     pos_dy
        neg     d5              ; dy = abs(dy)
pos_dy:
        cmp.l   d4, d5          ; dy - dx
        blt     reg_ok
        exg     d4, d5          ; dx < dy => swap(dx, dy)

reg_ok:
        ;; Register setup:

        ;;    BLTADAT  = $8000
        ;;    BLTBDAT  = line texture pattern ($FFFF for a solid line)
        move.w  #$8000, _custom+bltadat
        move.w  #$FFFF, _custom+bltbdat

        ;;    BLTAFWM  = $FFFF
        ;;    BLTALWM  = $FFFF
        move.w  #$FFFF, _custom+bltafwm
        move.w  #$FFFF, _custom+bltalwm

        ;;    BLTAMOD  = 4 * (dy - dx)
        ;;    BLTBMOD  = 4 * dy
        ;;    BLTCMOD  = width of the bitplane in bytes
        ;;    BLTDMOD  = width of the bitplane in bytes
        move.l  d5, d6
        sub.l   d4, d6
        asl.l   #2, d6          ; d6 = 4*(dy-dx)
        move.w  d6, _custom+bltamod

        move.l  d5, d6
        asl.l   #2, d6          ; d6 = 4*dy
        move.w  d6, _custom+bltbmod

        move.l  #$28, d6        ; d6 = 320 / 8 = $28
        move.w  d6, _custom+bltcmod
        move.w  d6, _custom+bltdmod
        
        ;;    BLTAPT   = (4 * dy) - (2 * dx)
        ;;    BLTBPT   = unused
        ;;    BLTCPT   = word containing the first pixel of the line
        ;;    BLTDPT   = word containing the first pixel of the line
        move.l  d5, d6
        asl.l   #2, d6          ; d6 = 4*dy
        move.l  d4, d7
        asl.l   #1, d7          ; d7 = 2*dx
        sub.l   d7, d6          ; d6 = (4*dy)-(2*dx)
        move.l  d6, _custom+bltapt

        move.l  #0, _custom+bltbpt ; unused

        move.l  a0, _custom+bltcpt
        move.l  a0, _custom+bltdpt

        ;;    BLTCON0  bits 15-12 = x1 modulo 15
        ;;    BLTCON0  bits SRCA, SRCC, and SRCD = 1
        ;;    BLTCON0  bit  SRCB = 0
        ;;      If exclusive-or line mode:
        ;;          then BLTCON0  LF control byte  = AB!C + !AC
        ;;          else BLTCON0  LF control byte  = AB +!AC
        move.w  #$0BFA, _custom+bltcon0

        ;;    BLTCON1  bit  LINEMODE  = 1
        ;;    BLTCON1  bit  OVFLAG  = 0
        ;;    BLTCON1  bits 4-2 =  octant number  from table
        ;;    BLTCON1  bits 15-12 = start bit for line texture (0 = last significant bit)
        ;;      If (((4 * dy) - (2 * dx)) < 0):                
        ;;          then  BLTCON1  bit  SIGNFLAG  = 1
        ;;          else  BLTCON1  bit  SIGNFLAG  = 0
        ;;      If one pixel/row:
        ;;          then  BLTCON1  bit  ONEDOT  = 1
        ;;          else  BLTCON1  bit  ONEDOT  = 0
        move.l  #6, d6          ; d6 = octant
        lsl.l   #2, d6
        addq    #1, d6
        move.w  d6, _custom+bltcon0

        ;;   BLTSIZE  bits 15-6 = dx + 1
        ;;   BLTSIZE  bits  5-0 = 2
        addq    #1, d4          ; d4 = dx+1
        lsl.l   #6, d4
        or.l    #2, d4
        move.w  d4, _custom+bltsize

waitmouse:
        cmpi.w  #$FF00, _custom+vhposr
        bls     end_wait       ; vhposr <= $FF00 (unsigned)
        move.w a0, _custom+color

wait_raster:
        cmpi.w  #$0800, _custom+vhposr
        ble     wait_raster    ; vhposr <= $0800 (signed)
        move.w #$0000, _custom+color

end_wait:
        btst #6,_ciaa+ciapra
        bne waitmouse

exit:
        movem.l (sp)+, d0-d7/a0-a6 ;
        rts
        end
        
;  ___________________________________________________________________________
; | License:                                                                  |
; | This program is free software: you can redistribute it and/or modify      |
; | it under the terms of the GNU General Public License version 2 as         |
; | published by the Free Software Foundation.                                |
; |                                                                           |
; | This program is distributed in the hope that it will be useful,           |
; | but WITHOUT ANY WARRANTY; without even the implied warranty of            |
; | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             |
; | GNU General Public License for more details.                              |
; |                                                                           |
; | You should have received a copy of the GNU General Public License         |
; | along with this program. If not, see <http://www.gnu.org/licenses/>       |
; | or write to the Free Software Foundation, Inc.,                           |
; | 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               |
; |___________________________________________________________________________|
