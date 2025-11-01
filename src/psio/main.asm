; SCSA-16 PSI/O Firmware - Located at 0xF000
; This routine initializes the CPU and jumps to the user program.

.ORG 0xF000     ; Set the fixed starting address for the firmware

; 1. System Initialization (e.g., Stack Pointer, not implemented fully yet)
; LDI R1, 0xFFFC  ; Initialize Stack Pointer R1 to 0xFFFC (High RAM)

; 2. JUMP TO USER PROGRAM
JMP 0x0010      ; Jump to the user program start address

; 3. Firmware Utilities (Future expansion area for I/O calls)
; ...

.ORG 0x0010     ; User Program starts here
; User Program Section: Test 1000 + 2000 = 3000
LDA R0, 0xE000  ; Load 1000
ADD R0, 0xE002  ; Add 2000
STA R0, 0xFFFE  ; Store 3000 to Output
HLT             ; Halt execution
