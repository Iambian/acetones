.nolist
#ifndef PROJECTDEFINES
#define PROJECTDEFINES
#include "ti84pce.inc"
;
;
;

;Undefine this to speed up core process by losing debug state
;#define COREDEBUG

DEBUG_PRINT_START_CYCLE .equ 0
DEBUG_STEP_START_CYCLE  .equ 30000 ;26485



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
vram_palette        .equ $D03200        ;64 bytes of color. It looks like itself
core_memory_ix      .equ $D03280        ;IX range -128 to -64 already in use.
scrap_mapping       .equ $D03300        ;256 bytes. maps to nowhere important.
saferam1            .equ $D03400        ;1024 bytes available. Define later.
main_ram            .equ $D03800        ;$0800 bytes of main system memory

ZEROPAGE            .equ main_ram+(0*256)
STACKBASE           .equ main_ram+(1*256)

BASETABLE           .equ $D04000        ;variable size. 256*BASETABLE_STRIDE
BASETABLE_STRIDE    .equ 12
PAGETABLE_OFFSET    .equ 0
PAGETABLE           .equ BASETABLE+PAGETABLE_OFFSET
JUMPTABLE_OFFSET    .equ 4
JUMPTABLE           .equ BASETABLE+JUMPTABLE_OFFSET
USETABLE_OFFSET     .equ 8
USETABLE            .equ BASETABLE+USETABLE_OFFSET

NAMETABLE1          .equ BASETABLE+(512*BASETABLE_STRIDE)
NAMETABLE2          .equ NAMETABLE1 + 1024

;
;saferam1 1024 bytes storage
;
nametable00         .equ saferam1           ;4b Do not move these vars
nametable01         .equ nametable00+4      ;4b
nametable10         .equ nametable01+4      ;4b
nametable11         .equ nametable10+4      ;4b
nametable00_attrib  .equ nametable11+4      ;4b
nametable01_attrib  .equ nametable00_attrib+4   ;4b
nametable10_attrib  .equ nametable01_attrib+4   ;4b
nametable11_attrib  .equ nametable10_attrib+4   ;4b
chr_rom_tiles       .equ nametable11_attrib+4   ;4b
chr_rom_sprites     .equ chr_rom_tiles+4    ;4b
curScreenBuf        .equ chr_rom_sprites+4  ;4b
linestart           .equ curScreenBuf+3     ;1b
chr_rom_curbank     .equ linestart+1        ;3b Base used for PPU stuff.
inputNumFiles       .equ chr_rom_curbank+3  ;1b
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
ppu_smc_gate        .equ chr_rom_base_file+1 ;3b write address to ungate PPU
timings_base        .equ ppu_smc_gate+3    ;3b. Current pointer to timings

SIZEOF_INTEGRITY_HASH    .equ 16
MAXPRGROM_HINIB     .equ 1
MAXCHRROM_HINIB     .equ 3
NUMBER_OF_MAPPERS   .equ 1
MAPPER_STRIDE       .equ 9

MAXNUMFILES         .equ 1                  ;set to 32 later. that's like 1.5MB
FILETABLE_STRIDE    .equ 8
FILETABLE           .equ timings_base+3   ;(MAXNUMFILES*FILETABLE_STRIDE)b
FILE_SIZE_OFFSET    .equ 0
FILE_ADR_OFFSET     .equ 2
FILE_SIG_ADR_OFFSET .equ 5

int_table_len       .equ FILETABLE+((MAXNUMFILES+1)*FILETABLE_STRIDE)
INT_TABLE           .equ int_table_len+1
INT_TABLE_MAXLEN    .equ 8
INT_TABLE_STRIDE    .equ 6
INT_ADDR_OFFSET     .equ 0
INT_CYCLE_OFFSET    .equ 3



;
;
;
;

;PPU default locations. Some are not used
#define PPU_CTRL -32
#define PPU_MASK -31
#define PPU_STATUS -30
#define PPU_OAMADDR -29
#define PPU_OAMDATA -28
#define PPU_SCROLL -27
#define PPU_ADDR -26
#define PPU_DATA -25
;PPU internal stuff
#define PPU_SUBCYCLE -15    ;0, 1, or 2.
#define PPU_V -14           ;USED TO STORE VRAM ADDRESS (PPU ADDRESS SPACE)
#define PPU_W -12           ;INTERNAL W REGISTER FOR HIGH/LOW BYTE PORT WRITES
#define PPU_FRAMECOUNT -11  ;USED FOR EVEN/ODD
#define PPU_NAMETABLE_PTR -10   ;Pointer cache
#define PPU_VCOUNT -7
#define PPU_HCOUNT -6
#define PPU_TCOUNT -5
#define PPU_XSCROLL -4  ;Should be constructed from PPU_SCROLL
#define PPU_YSCROLL -3  ;
#define APU_KEYPAD -2   ;First to last: A,B,SE,ST,UP,DO,LE,RI. 01234567.
#define APU_KEYLATCH -1
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
#define CORE_STACK_BASE 17
#define CORE_ERROR_LEVEL 20
#define SCRAP_COUNTER 21
#define PORT_WRITE_TIMESTAMP 24
#define TEMP_MEMORY 27
#define NMI_TRIGGERED 30 
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