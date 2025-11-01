// file: src/compiler/scsa-calculator.c (SCSA Virtual Calculator)
// -----------------------------------------------------------------
// Executes user-defined 1-bit arithmetic/logic operations using the SCSA ISA.
// -----------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// HARDWARE CONSTANTS
#define MEMORY_SIZE 32 
unsigned char RAM[MEMORY_SIZE]; 
unsigned char PC = 0;   
unsigned char IR = 0;   
int ACCUMULATOR = 0; 
int I_O_SWITCH = 1; 

// OPCODE MAPPING (Official SCSA ISA)
#define OPCODE_HLT 0x0
#define OPCODE_LDA 0x2
#define OPCODE_STA 0x3
#define OPCODE_LDI 0x4
#define OPCODE_NOT 0x5
#define OPCODE_XOR 0x6
#define OPCODE_AND 0x7
#define OPCODE_ADD 0xC // New ADD instruction
#define OPCODE_OUT 0xB

// Utility to convert binary string to decimal (for internal use)
unsigned char binToDec(const char* bin) {
    return (unsigned char)strtol(bin, NULL, 2);
}

// Function to print CPU state
void print_state(int cycle, unsigned char opcode, unsigned char operand) {
    // Print Opcode in Binary for clarity
    char op_bin[5]; 
    for(int i = 0; i < 4; i++) op_bin[i] = ((opcode >> (3-i)) & 1) ? '1' : '0';
    op_bin[4] = '\0';

    printf("--- CYCLE %d ---\n", cycle);
    printf("PC: %04X | IR: %02X | Opcode: %s | Operand: %04X | ACC: %d | RAM_OUT: %d\n",
           PC, IR, op_bin, operand, ACCUMULATOR, RAM[31] & 1); 
}

// Function to execute the instruction (CUTT Logic)
void execute_instruction(unsigned char instruction) {
    unsigned char opcode = (instruction >> 4) & 0xF; 
    unsigned char operand = instruction & 0xF;      
    int pc_increment = 1; 
    unsigned char AR = operand; 

    // CUTT Logic implementation (simplified for calculator mode)
    switch (opcode) {
        case OPCODE_HLT: 
            printf("HLT (0000) executed. Simulation stopped.\n");
            pc_increment = 0; return; 
        case OPCODE_LDA: 
            ACCUMULATOR = RAM[AR] & 1; 
            printf("LDA %04X executed. ACC = %d.\n", AR, ACCUMULATOR); break;
        case OPCODE_STA: 
            RAM[AR] = ACCUMULATOR;
            printf("STA %04X executed. RAM[%04X] set to %d.\n", AR, AR, ACCUMULATOR); break;
        case OPCODE_LDI: 
            ACCUMULATOR = operand & 1; 
            printf("LDI %04X executed. ACC = %d.\n", operand, ACCUMULATOR); break;
        case OPCODE_NOT: 
            ACCUMULATOR = !ACCUMULATOR;
            printf("NOT executed. ACC = %d.\n", ACCUMULATOR); break;
        case OPCODE_XOR: 
            ACCUMULATOR = ACCUMULATOR ^ (RAM[AR] & 1);
            printf("XOR %04X executed. ACC = %d.\n", AR, ACCUMULATOR); break;
        case OPCODE_AND: 
            ACCUMULATOR = ACCUMULATOR & (RAM[AR] & 1);
            printf("AND %04X executed. ACC = %d.\n", AR, ACCUMULATOR); break;
        case OPCODE_ADD: 
            ACCUMULATOR = (ACCUMULATOR + (RAM[AR] & 1)) % 2; 
            printf("ADD %04X executed. ACC = %d (Sum only).\n", AR, ACCUMULATOR); break;
        case OPCODE_OUT:
             RAM[31] = ACCUMULATOR; 
            printf("OUT executed. LED/RAM[31] set to %d.\n", ACCUMULATOR); break;

        default:
            printf("Instruction %04b not supported in Calculator mode. Halting.\n", opcode);
            return;
    }

    if (pc_increment) {
        PC = (PC + 1) % MEMORY_SIZE; 
    }
}

// Function to parse user input and load machine code
int parse_and_load(const char* input) {
    char op[10];
    int val_a = 0, val_b = -1; 
    
    // Attempt to parse the input string (e.g., "1 XOR 0" or "NOT 1")
    if (sscanf(input, "%d %s %d", &val_a, op, &val_b) == 3) {
        // Binary operation (e.g., 1 ADD 0)
        if (val_a != 0 && val_a != 1) return 0;
        if (val_b != 0 && val_b != 1) return 0;
    } else if (sscanf(input, "%s %d", op, &val_a) == 2) {
        // Unary operation (e.g., NOT 1)
        if (val_a != 0 && val_a != 1) return 0;
        val_b = 0; 
    } else {
        return 0; // Parsing failed
    }

    // Set RAM[2] and RAM[3] as data storage (Operands)
    RAM[2] = val_a; 
    RAM[3] = val_b; 

    // 1. LDA: Load Operand A (from RAM[2]) into ACC
    RAM[0] = (OPCODE_LDA << 4) | 0x2; 

    // 2. Perform Operation
    unsigned char opcode;
    for(int i = 0; op[i]; i++){ op[i] = toupper(op[i]); }

    if (strcmp(op, "NOT") == 0) {
        // Unary: Operation happens on ACC
        RAM[1] = (OPCODE_NOT << 4) | 0x0; 
    } else if (strcmp(op, "AND") == 0) {
        RAM[1] = (OPCODE_AND << 4) | 0x3; // Operand B is at RAM[3]
    } else if (strcmp(op, "XOR") == 0) {
        RAM[1] = (OPCODE_XOR << 4) | 0x3; 
    } else if (strcmp(op, "ADD") == 0) {
        RAM[1] = (OPCODE_ADD << 4) | 0x3; 
    } else {
        printf("Error: Unsupported operation %s.\n", op);
        return 0; 
    }

    // 3. OUT: Output the result (ACC) to RAM[31]
    RAM[2] = (OPCODE_OUT << 4) | 0x0; 

    // 4. HLT: Stop
    RAM[3] = (OPCODE_HLT << 4) | 0x0;

    return 1; // Success
}

int main() {
    char input_buffer[20];
    
    printf("\n+==========================================+\n");
    printf("| SCSA 1-Bit Virtual Calculator (v2.0)     |\n");
    printf("| Supported Ops: ADD, XOR, AND (A op B), NOT (op A)|\n");
    printf("+==========================================+\n\n");
    
    // Reset VM state
    memset(RAM, 0, MEMORY_SIZE);
    PC = 0;
    ACCUMULATOR = 0;

    printf("Enter the arithmetic operation you want to solve (e.g., 1 XOR 0 or NOT 1):\n> ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) { return 0; }
    input_buffer[strcspn(input_buffer, "\n")] = 0; // Remove newline

    if (!parse_and_load(input_buffer)) {
        printf("Error: Invalid input format or values (Must be 0 or 1). Please restart.\n");
        return 1;
    }

    printf("\n--- VM Execution Start ---\n");
    int cycle = 0;
    
    // Run until HLT is encountered
    while (RAM[PC] != 0x00 && cycle < 10) { 
        IR = RAM[PC];
        unsigned char opcode = (IR >> 4) & 0xF;
        unsigned char operand = IR & 0xF;
        
        print_state(cycle++, opcode, operand);
        execute_instruction(IR);
    }
    
    // Execute HLT instruction (if loop didn't break)
    if (RAM[PC] == 0x00) {
        print_state(cycle, RAM[PC] >> 4, RAM[PC] & 0xF);
        execute_instruction(RAM[PC]);
    }
    
    printf("\n--- VM Execution Complete ---\n");
    printf("Final Result (ACC): %d\n", ACCUMULATOR);
    printf("Result in Output RAM[31]: %d\n", RAM[31] & 1);
    
    return 0;
}
