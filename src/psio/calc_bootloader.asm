; --- SCSA-16 Calculator Bootloader Assembly (1000 * 10) ---

; Data Segment - MUST match the emulator's expectations (0xE000 and 0xE002)
.ORG 0xE000
.WORD 1000  ; VAL_A data (0x03E8) at 0xE000
.WORD 10    ; VAL_B data (0x000A) at 0xE002

; Code Segment - The actual Bootloader
.ORG 0x0100 ; Bootloader start address
LDA R0, 0xE000  ; R0 <- 1000
MUL R0, 0xE002  ; R0 <- 1000 * 10 = 10000
STA R0, 0xFFFE  ; Store result to output port
HLT             ; Halt
