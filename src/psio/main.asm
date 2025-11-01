// SCSA - Primary System I/O (PS/IO) Boot Code
// Loads system status and jumps to user space.

START:
    LDA 8     ; 0010 1000 - Load System Ready Flag (RAM[8]) into ACC
    OUT 0     ; 1011 0000 - Output ACC (System Ready Status) to LED
    LDA 9     ; 0010 1001 - Load Jump Control Bit (RAM[9])
    JMP 15    ; 1000 1111 - Jump to User Program Start (RAM[15] / 0xF)

USER_START:
    ; User code begins here (after successful jump)
    HLT 0
