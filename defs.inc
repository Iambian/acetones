.nolist
#ifndef PROJECTDEFINES
#define PROJECTDEFINES
#include "ti84pce.inc"
;
;
;

FONTFGCOLOR .EQU $FF
FONTBGCOLOR .EQU $00

screenBuf1      .equ vRam
screenBuf2      .equ vRam+(320*240)
;DO NOT MOVE THESE.
xPos                .equ $D031F6
yPos                .equ $D031F7
slopPos             .equ $D031F8        ;extra byte here
debugram            .equ $D031F9        ;FA and FB occupied too.
save_spl            .equ $D031FC        ;FD and FE occupied too.
save_mbase          .equ $D031FF        ;
core_memory_ix      .equ $D03280        ;occupies D03200 to D032FF
scrap_mapping       .equ $D03300        ;256 bytes. maps to nowhere important.
saferam1            .equ $D03400        ;1024 bytes available. Define later.
main_ram            .equ $D03800        ;$0800 bytes of main system memory

ZEROPAGE            .equ main_ram+(0*256)
STACKBASE           .equ main_ram+(1*256)

BASETABLE           .equ $D04000        ;variable size. 256*BASETABLE_STRIDE
BASETABLE_STRIDE    .equ 8
PAGETABLE_OFFSET    .equ 0
PAGETABLE           .equ BASETABLE+PAGETABLE_OFFSET
JUMPTABLE_OFFSET    .equ 4
JUMPTABLE           .equ BASETABLE+JUMPTABLE_OFFSET

NAMETABLE1          .equ BASETABLE+(256*BASETABLE_STRIDE)
NAMETABLE2          .equ NAMETABLE1 + 1024

;saferam1 1024 bytes storage
;
curScreenBuf        .equ saferam1   ;3b
;
;





#define REGA 0
#define REGX 1
#define REGY 2
#define REGSP 3
#define REGSR 4
#define REGPC 5
#define CYCLES_REMAIN ix+8
#define SF_NEG 7
#define SF_OVER 6 
#define SF_RESERVED 5 
#define SF_BRK 4
#define SF_DEC 3
#define SF_INT 2
#define SF_ZERO 1
#define SF_CARRY 0

#macro ADDCYC(X)
    #if (x < 1)
        nop
    #elif (X = 1)
        inc iy
    #else
        lea iy,iy+X
    #endif
#endmacro

;
;
;
#endif
.list