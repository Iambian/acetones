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



FONTFGCOLOR .EQU $FF ;$29
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
vram_palette        .equ $D03200        ;32 bytes. PPU palette RAM (bg, spr)
vram_cache          .equ $D03220        ;32 bytes. Used for prior frame compare
core_memory_ix      .equ $D03280        ;IX range -128 to -64 already in use.
scrap_mapping       .equ $D03300        ;256 bytes. maps to nowhere important.
saferam1            .equ $D03400        ;1024 bytes available. Define later.
main_ram            .equ $D03800        ;$0800 bytes of main system memory

ZEROPAGE            .equ main_ram+(0*256)
STACKBASE           .equ main_ram+(1*256)

NAMETABLE1          .equ $D04000
NAMETABLE2          .equ $D04400
NAMETABLE3          .equ $D04800    ;These two don't exist on the NES but
NAMETABLE4          .equ $D04C00    ;are present here in case mappers.
OBJECT_ATTRIB_MEM   .equ $D05000    ;256 BYTES

EVENT_STACK_L       .equ $D10000
EVENT_STACK_S       .equ EVENT_STACK_L & $00FFFF

BASETABLE           .equ $D08000        ;variable size. 256*BASETABLE_STRIDE
BASETABLE_STRIDE    .equ 12
PAGETABLE_OFFSET    .equ 0
PAGETABLE           .equ BASETABLE+PAGETABLE_OFFSET
JUMPTABLE_OFFSET    .equ 4
JUMPTABLE           .equ BASETABLE+JUMPTABLE_OFFSET
;USETABLE_OFFSET     .equ 8
;USETABLE            .equ BASETABLE+USETABLE_OFFSET


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
ppu_smc_gate        .equ chr_rom_base_file+1    ;3b write address to ungate PPU
ppu_smc_gate_ports  .equ ppu_smc_gate+3         ;1b smc target to direct register write
ppu_smc_gate_stack  .equ ppu_smc_gate_ports+1   ;1b smc target to event stack write
chr_rom_cachefrom_A .equ ppu_smc_gate_stack+1   ;3b. source of cache in bank 1
chr_rom_cachefrom_B .equ chr_rom_cachefrom_A+3  ;3b. source of cache in bank 2
timings_base        .equ chr_rom_cachefrom_B+3  ;3b. Current pointer to timings

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

ppuScreenBuf1       .equ vRam                       ;+$000000
ppuScreenBuf2       .equ ppuScreenBuf1+(256*224)    ;+$00E000
chr_rom_1_cache     .equ ppuScreenBuf2+(256*224)    ;+$01C000
chr_rom_2_cache     .equ chr_rom_1_cache+(256*64)   ;+$020000
;EOM: $25800


;
;
;
;
#define WREG_CONST %01010101
#if WREG_CONST != %01010101
    .error "WREG_CONST must equal %01010101" 
#endif

;PPU default locations. Some are not used
#define DEBUGCONTROL ($40-$80)
#define PPU_CTRL -32
#define PPU_MASK -31
#define PPU_STATUS -30
#define PPU_OAMADDR -29
#define PPU_OAMDATA -28
#define PPU_SCROLL -27
#define PPU_ADDR -26
#define PPU_DATA -25
;PPU internal stuff
#define PPU_W_RESET -20     ;Write WREG_CONST on status read, for event stack.
#define PPU_ATTRIBUTE_CACHE -19
#define PPU_XSCROLL_FINE -18
#define PPU_YSCROLL_FINE -17
#define PPU_XSCROLL_ROUGH -16   ;PPU_XSCROLL with fine adjustments shifted out.
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
#define PREV_INSTR 31
;flag IDs
#define SF_NEG 7
#define SF_OVER 6 
#define SF_RESERVED 5 
#define SF_BRK 4
#define SF_DEC 3
#define SF_INT 2
#define SF_ZERO 1
#define SF_CARRY 0
;
;
;Named PPU registers
PPU_CTRL_ADR                .equ 0
PPU_MASK_ADR                .equ 1
PPU_STATUS_ADR              .equ 2
PPU_OAMADDR_ADR             .equ 3
PPU_OAMDATA_ADR             .equ 4
PPU_SCROLL_ADR              .equ 5
PPU_ADDR_ADR                .equ 6
PPU_DATA_ADR                .equ 7
;Named APU registers
APU_PULSE1_TIMER_ADR        .equ $00
APU_PULSE1_LENCOUNT_ADR     .equ $01
APU_PULSE1_ENVELOPE_ADR     .equ $02
APU_PULSE1_SWEEP_ADR        .equ $03
APU_PULSE2_TIMER_ADR        .equ $04
APU_PULSE2_LENCOUNT_ADR     .equ $05
APU_PULSE2_ENVELOPE_ADR     .equ $06
APU_PULSE2_SWEEP_ADR        .equ $07
APU_TRIANGLE_TIMER_ADR      .equ $08
APU_TRIANGLE_UNUSED         .equ $09
APU_TRIANGLE_LENCOUNT_ADR   .equ $0A
APU_TRIANGLE_LINCOUNT_ADR   .equ $0B
APU_NOISE_TIMER_ADR         .equ $0C
APU_NOISE_LENCOUNT_ADR      .equ $0D
APU_NOISE_ENVELOPE_ADR      .equ $0E
APU_NOISE_LFSR_ADR          .equ $0F
APU_DMC_TIMER_ADR           .equ $10
APU_DMC_MEMORYREADER_ADR    .equ $11
APU_DMC_SAMPLEBUFFER_ADR    .equ $12
APU_DMC_OUTPUTUNIT_ADR      .equ $13
APU_OAMDMA_ADR              .equ $14    ;Should be listed with PPU registers.
APU_STATUS_ADR              .equ $15
APU_UNKNOWN2_ADR            .equ $16
APU_FRAMECOUNTER_ADR        .equ $17
;Named I/O devices. Shares memory space with APU and even overlaps one port.
IO_CONTROLLER1_ADR          .equ $16    ;read-and-write.
IO_CONTROLLER2_ADR          .equ $17    ;read-only. Writes are for framecounter
;PPU_CTRL flag bits
PPU_CTRL_BASE_NAMETABLE     .equ 0
PPU_CTRL_VRAM_INCREMENT     .equ 2
PPU_CTRL_BASE_SPRITETABLE   .equ 3
PPU_CTRL_BASE_BGTABLE       .equ 4
PPU_CTRL_SPRITE_SIZE        .equ 5
PPU_CTRL_MASTER_SLAVE_SELECT .equ 6
PPU_CTRL_VBLANK_NMI_ENABLE  .equ 7
;PPU_MASK flag bits
PPU_MASK_GREYSCALE          .equ 0
PPU_MASK_SHOW_LEFT_BG       .equ 1
PPU_MASK_SHOW_LEFT_SPRITES  .equ 2
PPU_MASK_RENDER_BG          .equ 3
PPU_MASK_RENDER_SPRITES     .equ 4
PPU_MASK_EMPHASIS_RED       .equ 5
PPU_MASK_EMPHASIS_GREEN     .equ 6
PPU_MASK_EMPHASIS_BLUE      .equ 7
;PPU_STATUS flag bigs
PPU_STATUS_VBLANK_TRIGGERED .equ 7
PPU_STATUS_SPRITE_0_HIT     .equ 6
PPU_STATUS_SPRITE_OVERFLOW  .equ 5  ;not implemented. Too bugged.



;OBJECT ATTRIBUTE TABLE OFFSET BYTES AND BITS
OAM_YPOS        .equ 0
OAM_INDEX       .equ 1
OAM_ATTRIBUTES  .equ 2
OAM_XPOS        .equ 3
OAM_PALETTE     .equ 0
OAM_PRIORITY    .equ 5
OAM_HFLIP       .equ 6
OAM_VFLIP       .equ 7


#macro ADDCYC(X)
    #if (x < 1)
        nop
    #elif (X = 1)
        inc iy
    #else
        lea iy,iy+X
    #endif
#endmacro

;`start` is the address right before the JR instruction, as you would label it.
;`target` is the address where the JR instruction needs to jump to.
#define JR_OPERAND(start, target) ((target-start)-2)+0




#endif
.list