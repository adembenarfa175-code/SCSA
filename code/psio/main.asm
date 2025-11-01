; file: code/psio/main.asm
; -----------------------------------------------------------------
; PSI/O (Please System I/O) - 1-Bit Computer Boot Code
; This code runs immediately after Power-On Reset (POR).
; -----------------------------------------------------------------

; ADDRESS: 0000
START:  LDA 1000  ; 0001 (LDA Opcode) + 1000 (RAM Address for Initial Data)
        ; ^^^ Load a single bit from RAM[8] into Accumulator (A).

; ADDRESS: 0001
        OUT 0000  ; 0010 (OUT Opcode) + 0000 (Not used, or specifies default Output Port)
        ; ^^^ Display the loaded bit on the I/O LED to signal system readiness.

; ADDRESS: 0010
        LDA 1001  ; Load a control bit from RAM[9] (e.g., if bit=1, jump to user code).
        
; ADDRESS: 0011
        JMP 1111  ; 1111 (JMP Opcode) + 1111 (Jump to RAM[15] - Start of User Program)
        ; ^^^ Jumps to the entry point of the main User Program.

; ADDRESS: 0100
        HLT 0000  ; 0000 (HLT Opcode) - Stop the processor if the jump failed.

