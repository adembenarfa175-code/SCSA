// file: ~/scsa/src/compiler/scsa-calculator.c
// This file is used to generate the assembly code for the simple calculator bootloader.
// (1000 * 10 = 10000)

#include <stdio.h>

int main() {
    printf("; --- SCSA-16 Calculator Bootloader Assembly (1000 * 10) ---\n");
    printf(".ORG 0xE000\n");
    printf("VAL_A: .WORD 1000\n");
    printf("VAL_B: .WORD 10\n");
    printf("\n");
    printf(".ORG 0x0100 ; Bootloader start address\n");
    printf("LDA R0, 0xE000  ; R0 <- 1000\n");
    printf("MUL R0, 0xE002  ; R0 <- 1000 * 10 = 10000\n");
    printf("STA R0, 0xFFFE  ; Store result to output port\n");
    printf("HLT             ; Halt\n");
    return 0;
}
