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

NAMETABLE1          .equ BASETABLE+(512*BASETABLE_STRIDE)
NAMETABLE2          .equ NAMETABLE1 + 1024

;saferam1 1024 bytes storage
;
curScreenBuf        .equ saferam1           ;3b
linestart           .equ curScreenBuf+3     ;1b
inputNumFiles       .equ linestart+1        ;1b
basefile_sig        .equ inputNumFiles+1    ;3b pointer to 16b hash
missing_files       .equ basefile_sig+3     ;1b
total_rom_size      .equ missing_files+1    ;3b
prg_rom_size        .equ total_rom_size+3   ;3b
chr_rom_size        .equ prg_rom_size+3     ;3b
submapper           .equ chr_rom_size+3     ;1b. iNES2 thing. Not often used.
ines_flag_6         .equ submapper+1        ;1b. Flags that a mapper may need.
mapper_id           .equ ines_flag_6+1      ;3b. Convenience load for 12 bits
mapper_adr_base     .equ mapper_id+1        ;3b. Used to load mapper
trainer_base        .equ mapper_adr_base+3  ;3b. If exist, starts here
prg_rom_base        .equ mapper_adr_base+3  ;3b. Adjusted in case of trainer.
chr_rom_base        .equ prg_rom_base+3     ;3b. Located somewhere.
chr_rom_base_file   .equ chr_rom_base+3     ;1b. Nonzero if not in first file.

SIZEOF_INTEGRITY_HASH    .equ 16
MAXPRGROM_HINIB     .equ 1
MAXCHRROM_HINIB     .equ 3
NUMBER_OF_MAPPERS   .equ 1
MAPPER_STRIDE       .equ 9

MAXNUMFILES         .equ 1                  ;set to 32 later. that's like 1.5MB
FILETABLE_STRIDE    .equ 8
FILETABLE           .equ chr_rom_base_file+3   ;(MAXNUMFILES*FILETABLE_STRIDE)b
FILE_SIZE_OFFSET    .equ 0
FILE_ADR_OFFSET     .equ 2
FILE_SIG_ADR_OFFSET .equ 5



;
;

;Undefine this to speed up core process by losing debug state
#define COREDEBUG

;core memory
#define REGA 0
#define REGX 1
#define REGY 2
#define REGSP 3
#define REGSR 4
#define REGPC 5
#define CYCLES_REMAIN 8
#define CYCLES_CURRENT 11
#define CURRENT_OPCODES 14
;flag IDs
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