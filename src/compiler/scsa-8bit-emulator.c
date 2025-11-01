// file: ~/scsa/src/compiler/scsa-8bit-emulator.c (SCSA-8 AI Ready VM)
// -----------------------------------------------------------------
// SCSA-8 UPGRADE: Added MUL and DIV instructions for basic AI operations.
// -----------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// HARDWARE CONSTANTS (8-Bit Upgrade)
#define MEMORY_SIZE 256 
unsigned char RAM[MEMORY_SIZE]; 
unsigned char PC = 0;   
unsigned char IR_OPCODE = 0; 
unsigned char IR_OPERAND = 0; 
unsigned char ACCUMULATOR = 0; 
unsigned char I_O_PORT = 0xFF; 

// OPCODE MAPPING (SCSA-8 AI ISA)
#define OPCODE_HLT 0x0
#define OPCODE_LDA 0x2
#define OPCODE_STA 0x3
#define OPCODE_LDI 0x4
#define OPCODE_ADD 0xC 
#define OPCODE_SUB 0xD 
#define OPCODE_MUL 0xE // NEW: Multiply Instruction
#define OPCODE_DIV 0xF // NEW: Divide Instruction
// (JMP, JNZ, IN, OUT, NOT, XOR, AND are assumed to be 0x5 to 0xB)

// Function to print CPU state
void print_state(int cycle) {
    printf("--- CYCLE %d ---\n", cycle);
    printf("PC: %02X | Opcode: %01X | Operand: %02X | ACC: %02X (%d) | RAM[FF]: %02X\n",
           PC, IR_OPCODE, IR_OPERAND, ACCUMULATOR, ACCUMULATOR, RAM[0xFF]); 
}

// Function to execute the instruction (8-Bit CUTT Logic)
void execute_instruction() {
    int pc_increment = 2; // Default increment is 2 bytes
    unsigned char MAR = IR_OPERAND; 
    unsigned int temp_result; // Use 16-bit for intermediate calculations (Multiplication)

    switch (IR_OPCODE) {
        case OPCODE_HLT: 
            printf("HLT (00) executed. Simulation stopped.\n");
            pc_increment = 0; return; 
        case OPCODE_LDA: 
            ACCUMULATOR = RAM[MAR]; 
            printf("LDA %02X executed. ACC = %02X.\n", MAR, ACCUMULATOR); break;
        case OPCODE_STA: 
            RAM[MAR] = ACCUMULATOR;
            printf("STA %02X executed. RAM[%02X] set to %02X.\n", MAR, MAR, ACCUMULATOR); break;
        case OPCODE_ADD: 
            ACCUMULATOR += RAM[MAR]; 
            printf("ADD %02X executed. ACC = %02X.\n", MAR, ACCUMULATOR); break;
        case OPCODE_SUB: 
            ACCUMULATOR -= RAM[MAR]; 
            printf("SUB %02X executed. ACC = %02X.\n", MAR, ACCUMULATOR); break;
            
        case OPCODE_MUL: // NEW: Multiplication
            temp_result = (unsigned int)ACCUMULATOR * RAM[MAR];
            ACCUMULATOR = (unsigned char)temp_result; // Store 8 bits (Truncate result)
            printf("MUL %02X executed. ACC = %02X (Result: %u).\n", MAR, ACCUMULATOR, temp_result); break;

        case OPCODE_DIV: // NEW: Division
            if (RAM[MAR] != 0) {
                ACCUMULATOR /= RAM[MAR]; // Integer division
                printf("DIV %02X executed. ACC = %02X.\n", MAR, ACCUMULATOR);
            } else {
                printf("DIVIDE BY ZERO error. Halting.\n");
                pc_increment = 0; return;
            }
            break;
        
        // JMP, JNZ, IN, OUT, etc. cases remain the same (Not repeated here for brevity)
        case 0x8: // JMP
        case 0x9: // JNZ
        case 0xA: // IN
        case 0xB: // OUT
        case 0x4: // LDI
        case 0x5: // NOT
        case 0x6: // XOR
        case 0x7: // AND
            printf("--- Non-Arithmetic Opcode (0x%X) executed ---\n", IR_OPCODE);
            // In a full implementation, the logic for these must be placed here.
            break;

        default:
            printf("Unknown Opcode %01X at PC %02X. Halting.\n", IR_OPCODE, PC);
            pc_increment = 0;
            return;
    }

    if (pc_increment) {
        PC = (PC + pc_increment) % MEMORY_SIZE;
    }
}

// AI Sample Program: Calculate the weighted sum W1*X1 + W2*X2
// W1 = 5, X1 = 10 (RAM[F0], RAM[F1])
// W2 = 2, X2 = 4 (RAM[F2], RAM[F3])
void load_ai_sample_program() {
    printf("Loading SCSA-8 AI Sample Program (W1*X1 + W2*X2)...\n");
    
    // Data Storage (W=Weight, X=Input)
    RAM[0xF0] = 5;  // W1
    RAM[0xF1] = 10; // X1
    RAM[0xF2] = 2;  // W2
    RAM[0xF3] = 4;  // X2
    
    // 1. LDA 0xF0 (Load W1=5)
    RAM[0x00] = 0x20; RAM[0x01] = 0xF0; 

    // 2. MUL 0xF1 (ACC = W1 * X1 = 5 * 10 = 50)
    RAM[0x02] = 0xE0; RAM[0x03] = 0xF1; 

    // 3. STA 0xFC (Store result T1=50)
    RAM[0x04] = 0x30; RAM[0x05] = 0xFC; 

    // 4. LDA 0xF2 (Load W2=2)
    RAM[0x06] = 0x20; RAM[0x07] = 0xF2; 

    // 5. MUL 0xF3 (ACC = W2 * X2 = 2 * 4 = 8)
    RAM[0x08] = 0xE0; RAM[0x09] = 0xF3; 

    // 6. ADD 0xFC (ACC = ACC + T1 = 8 + 50 = 58)
    RAM[0x0A] = 0xC0; RAM[0x0B] = 0xFC; 

    // 7. STA 0xFF (Store Final Sum)
    RAM[0x0C] = 0x30; RAM[0x0D] = 0xFF; 

    // 8. HLT
    RAM[0x0E] = 0x00; RAM[0x0F] = 0x00; 

    PC = 0x00; 
}

int main() {
    load_ai_sample_program();
    
    printf("\n+==========================================+\n");
    printf("| SCSA-8 AI Core VM (8-Bit) Execution      |\n");
    printf("| Program: Weighted Sum (5*10 + 2*4 = 58)  |\n");
    printf("+==========================================+\n");
    
    int cycle = 0;
    while (RAM[PC] >> 4 != OPCODE_HLT && cycle < 15) { 
        IR_OPCODE = (RAM[PC] >> 4) & 0xF; 
        IR_OPERAND = RAM[PC+1];

        print_state(cycle++);
        execute_instruction();
    }
    
    printf("\n--- VM Execution Complete ---\n");
    printf("Final Weighted Sum (RAM[FF]): %d (0x%02X)\n", RAM[0xFF], RAM[0xFF]);
    
    return 0;
}
