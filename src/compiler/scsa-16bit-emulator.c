// file: ~/scsa/src/compiler/scsa-16bit-emulator.c (SCSA-16 Fixed Boot VM)
// -----------------------------------------------------------------
// UPGRADE: PSI/O now located at Fixed Boot Address 0xF000.
// -----------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// HARDWARE CONSTANTS (16-Bit)
#define MEMORY_SIZE 65536
#define PSIO_BOOT_ADDRESS 0xF000 // Fixed address for PSI/O firmware (The "FFFFx0A" concept)
#define USER_PROGRAM_START 0x0010 // Location where PSI/O jumps to user's program
unsigned char RAM[MEMORY_SIZE]; 

// Registers (16-bit)
unsigned short PC = 0;   
unsigned char IR_OPCODE = 0; 
unsigned char IR_REGISTER = 0; 
unsigned short IR_OPERAND = 0; 
unsigned short ACCUMULATOR = 0; 
unsigned short I_O_PORT = 0xFFFE; // Output Port (RAM[0xFFFE])

// Register Map (Simple)
#define R0_ACC 0x0 

// OPCODE MAPPING (SCSA-16 ISA)
#define OPCODE_HLT 0x0
#define OPCODE_LDA 0x2
#define OPCODE_STA 0x3
#define OPCODE_LDI 0x4
#define OPCODE_ADD 0xC 
#define OPCODE_JMP 0x8 

// ... (Functions print_state and execute_instruction remain the same, NOT repeated here for brevity) ... 
// NOTE: In a real environment, you MUST include the full functions here.

// Function to print CPU state
void print_state(int cycle) {
    printf("--- CYCLE %d ---\n", cycle);
    printf("PC: %04X | Opcode: %01X | Reg: %01X | Operand: %04X | ACC: %04X (%d) | RAM[FFFE]: %04X\n",
           PC, IR_OPCODE, IR_REGISTER, IR_OPERAND, ACCUMULATOR, ACCUMULATOR, *(unsigned short*)&RAM[0xFFFE]); 
}

// Function to execute the instruction (16-Bit CUTT Logic)
void execute_instruction() {
    int pc_increment = 3; // Instruction is 3 bytes long
    unsigned short MAR = IR_OPERAND; 

    switch (IR_OPCODE) {
        case OPCODE_HLT: 
            printf("HLT (00) executed. Simulation stopped.\n");
            pc_increment = 0; return; 

        case OPCODE_LDA: 
            ACCUMULATOR = *(unsigned short*)&RAM[MAR]; 
            printf("LDA R0, %04X executed. ACC = %04X.\n", MAR, ACCUMULATOR); break;

        case OPCODE_STA: 
            *(unsigned short*)&RAM[MAR] = ACCUMULATOR;
            printf("STA R0, %04X executed. RAM[%04X] set to %04X.\n", MAR, MAR, ACCUMULATOR); break;
            
        case OPCODE_LDI: 
            ACCUMULATOR = MAR; 
            printf("LDI R0, %04X executed. ACC = %04X.\n", MAR, ACCUMULATOR); break;
            
        case OPCODE_ADD: 
            ACCUMULATOR += *(unsigned short*)&RAM[MAR]; 
            printf("ADD R0, %04X executed. ACC = %04X.\n", MAR, ACCUMULATOR); break;

        case OPCODE_JMP: 
            PC = MAR; 
            pc_increment = 0; 
            printf("JMP %04X executed. New PC = %04X.\n", MAR, PC); break;

        default:
            printf("Unknown Opcode %01X at PC %04X. Halting.\n", IR_OPCODE, PC);
            pc_increment = 0;
            return;
    }

    if (pc_increment) {
        PC = (PC + pc_increment) % MEMORY_SIZE; 
    }
}


// PSI/O Firmware (Fixed at 0xF000)
void load_psio_firmware() {
    printf("Loading PSI/O Firmware at fixed address 0x%04X...\n", PSIO_BOOT_ADDRESS);
    
    // PSI/O ROUINE (1. JMP to USER_PROGRAM_START (0x0010))
    // JMP 0x0010 (Opcode 0x8)
    RAM[PSIO_BOOT_ADDRESS + 0] = 0x80; // Opcode 8 (JMP) + R0
    RAM[PSIO_BOOT_ADDRESS + 1] = 0x00; // Address High (0x0010)
    RAM[PSIO_BOOT_ADDRESS + 2] = 0x10; // Address Low

    // USER PROGRAM (Located at 0x0010) - Same test program: 1000 + 2000 = 3000
    // Data Storage (16-bit) - Now fixed at 0xE000
    *(unsigned short*)&RAM[0xE000] = 1000; 
    *(unsigned short*)&RAM[0xE002] = 2000; 
    
    // 1. LDA R0, 0xE000 (Load 1000 from RAM[E000]) (Opcode 0x2)
    RAM[USER_PROGRAM_START + 0] = 0x20; 
    RAM[USER_PROGRAM_START + 1] = 0xE0; 
    RAM[USER_PROGRAM_START + 2] = 0x00; 

    // 2. ADD R0, 0xE002 (ACC = 1000 + 2000 = 3000) (Opcode 0xC)
    RAM[USER_PROGRAM_START + 3] = 0xC0; 
    RAM[USER_PROGRAM_START + 4] = 0xE0; 
    RAM[USER_PROGRAM_START + 5] = 0x02; 

    // 3. STA R0, 0xFFFE (Store 3000 at Output Port) (Opcode 0x3)
    RAM[USER_PROGRAM_START + 6] = 0x30; 
    RAM[USER_PROGRAM_START + 7] = 0xFF; 
    RAM[USER_PROGRAM_START + 8] = 0xFE; 

    // 4. HLT (Opcode 0x0)
    RAM[USER_PROGRAM_START + 9] = 0x00; 
    RAM[USER_PROGRAM_START + 10] = 0x00;
    RAM[USER_PROGRAM_START + 11] = 0x00; 
}

int main() {
    load_psio_firmware();
    
    printf("\n+==========================================+\n");
    printf("| SCSA-16 (Fixed Boot) VM Execution Start  |\n");
    printf("| PSI/O Start Address: 0x%04X              |\n", PSIO_BOOT_ADDRESS);
    printf("| Program: 1000 + 2000 = 3000              |\n");
    printf("+==========================================+\n");
    
    int cycle = 0;
    
    // **الجهاز الافتراضي يبدأ بالبحث عن المسار الثابت 0xF000**
    PC = PSIO_BOOT_ADDRESS; 
    
    while (RAM[PC] >> 4 != OPCODE_HLT && cycle < 10) { 
        IR_OPCODE = (RAM[PC] >> 4) & 0xF; 
        IR_REGISTER = RAM[PC] & 0xF; 
        IR_OPERAND = (RAM[PC+1] << 8) | RAM[PC+2]; 

        print_state(cycle++);
        execute_instruction();
    }
    
    printf("\n--- VM Execution Complete ---\n");
    printf("Final Result (RAM[FFFE]): %d (0x%04X)\n", *(unsigned short*)&RAM[0xFFFE], *(unsigned short*)&RAM[0xFFFE]);
    
    return 0;
}
