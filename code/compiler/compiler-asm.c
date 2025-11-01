// file: code/compiler/compiler-asm.c
// -----------------------------------------------------------------
// 1-Bit Computer Assembler (C-based)
// Converts assembly mnemonics into 4-bit machine code.
// Note: This is a highly simplified pseudo-compiler for demonstration.
// -----------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to convert Opcode Mnemonic to 4-bit binary string
const char* opcode_to_binary(const char* mnemonic) {
    if (strcmp(mnemonic, "HLT") == 0) return "0000";
    if (strcmp(mnemonic, "LDA") == 0) return "0001";
    if (strcmp(mnemonic, "OUT") == 0) return "0010";
    if (strcmp(mnemonic, "JMP") == 0) return "1111";
    return "????"; // Error Code
}

void assemble_line(char *line) {
    char mnemonic[5];
    char operand[5] = "0000"; // Default operand is 0 (4 bits)
    
    // Split the line into mnemonic and operand
    if (sscanf(line, "%4s %4s", mnemonic, operand) >= 1) {
        
        // Convert mnemonic to binary opcode
        const char* opcode = opcode_to_binary(mnemonic);
        
        // Skip comments or labels
        if (opcode[0] == '?') {
            return;
        }

        // Output the combined 8-bit instruction (Opcode + Address/Operand)
        printf("%s%s\n", opcode, operand); 
        // Note: Our 1-bit CPU uses 4-bit instructions only. 
        // For 4-bit ISA, the output is only the 4-bit Opcode if no operand is needed.
        // But since we use 4-bit Opcode + 4-bit Address = 8 bits total per instruction 
        // to simplify address fetching, the 4-bit CPU must fetch 8 bits.
        
    }
}

int main(int argc, char *argv[]) {
    printf("// 1-Bit Computer Machine Code Output\n");
    printf("// Instruction Format: [Opcode 4-bit] [Address/Operand 4-bit]\n\n");

    // Example of assembling the PSI/O code directly:
    assemble_line("LDA 1000"); // 0001 1000
    assemble_line("OUT 0000"); // 0010 0000
    assemble_line("LDA 1001"); // 0001 1001
    assemble_line("JMP 1111"); // 1111 1111
    
    return 0;
}

