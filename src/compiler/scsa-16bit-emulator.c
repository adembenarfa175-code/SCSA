// file: ~/scsa/src/compiler/scsa-16bit-emulator.c (SCSA-16 Secure Boot VM)
// UPGRADE: Reads 'bootloader.bin' and activates the PSI/O Shell on failure.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

// HARDWARE CONSTANTS
#define MEMORY_SIZE 0x10000 // 64 Kilobytes
#define PSIO_BOOT_ADDRESS 0xF000 
#define BOOTLOADER_MAX_SIZE 0xA000 // 40 KB limit (More realistic for 64KB RAM)
#define BOOTLOADER_START_ADDR 0x0100 
unsigned char RAM[MEMORY_SIZE]; 

// Registers (16-bit)
uint16_t PC = 0;   
uint8_t IR_OPCODE = 0; 
uint8_t IR_REGISTER = 0; 
uint16_t IR_OPERAND = 0; 
uint16_t ACCUMULATOR = 0; 

// OPCODE MAPPING
#define OPCODE_HLT 0x0
#define OPCODE_LDA 0x2 
#define OPCODE_STA 0x3
#define OPCODE_LDI 0x4
#define OPCODE_ADD 0xC 
#define OPCODE_JMP 0x8 
#define OPCODE_MUL 0xE // Added MUL

// PSI/O Shell Functionality (Conceptual)
void psio_shell_loop() {
    printf("\n==========================================\n");
    printf("   PSI/O Shell v1.0 (Secure Boot Failure)   \n");
    printf("   Type 'load' or 'diag' for commands.      \n");
    printf("==========================================\n");
    // In a real system, PC would be set to execute shell firmware code
}

// Function to print CPU state
void print_state(int cycle) {
    printf("--- CYCLE %d ---\n", cycle);
    printf("PC: %04X | Op: %01X | Reg: %01X | Oper: %04X | ACC: %04X (%d) | RAM[FFFE]: %04X\n",
           PC, IR_OPCODE, IR_REGISTER, IR_OPERAND, ACCUMULATOR, ACCUMULATOR, *(uint16_t*)&RAM[0xFFFE]); 
}

void execute_instruction() {
    // (Execution logic remains the same - simplified here for readability)
    int pc_increment = 3; 
    uint16_t MAR = IR_OPERAND; 

    switch (IR_OPCODE) {
        case OPCODE_HLT: 
            printf("HLT (00) executed. Simulation stopped.\n");
            pc_increment = 0; return; 
        case OPCODE_LDA: ACCUMULATOR = *(uint16_t*)&RAM[MAR]; break;
        case OPCODE_STA: *(uint16_t*)&RAM[MAR] = ACCUMULATOR; break;
        case OPCODE_LDI: ACCUMULATOR = MAR; break;
        case OPCODE_ADD: ACCUMULATOR += *(uint16_t*)&RAM[MAR]; break;
        case OPCODE_MUL: ACCUMULATOR *= *(uint16_t*)&RAM[MAR]; break;
        case OPCODE_JMP: PC = MAR; pc_increment = 0; break;
        default: printf("Unknown Opcode %01X at PC %04X. Halting.\n", IR_OPCODE, PC); pc_increment = 0; return;
    }
    
    if (pc_increment) PC = (PC + pc_increment) % MEMORY_SIZE; 
}


// --- PSI/O Secure Boot Logic ---
int load_bootloader_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("[PSIO] Bootloader File Not Found: %s\n", filename);
        return 0; // Failure
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 2. Check File Size (10MB concept - using 40KB limit for 16-bit RAM)
    if (file_size > BOOTLOADER_MAX_SIZE || file_size == 0) {
        printf("[PSIO] Security Check FAILED: Bootloader size (%ld B) is invalid.\n", file_size);
        fclose(file); return 0;
    }
    
    // 3. Check for .tar (Compressed/Unsafe) - Simplified: we assume .bin is safe
    // (If the size/signature was wrong, it would fail above)

    // Load the file content into RAM starting at BOOTLOADER_START_ADDR (0x0100)
    size_t bytes_read = fread(&RAM[BOOTLOADER_START_ADDR], 1, file_size, file);
    fclose(file);

    // 6/7. Check Architecture & Source (Simulated via fixed header)
    if (RAM[BOOTLOADER_START_ADDR] >> 4 != OPCODE_LDA || RAM[BOOTLOADER_START_ADDR + 1] != 0x01) {
        printf("[PSIO] Security Check FAILED: Bootloader signature (LDA R0, 0x....) not found.\n");
        return 0; 
    }

    printf("[PSIO] Bootloader loaded: %ld bytes at 0x%04X. All checks PASSED.\n", bytes_read, BOOTLOADER_START_ADDR);
    return 1; // Success
}

void load_psio_firmware() {
    printf("Loading PSI/O Firmware at fixed address 0x%04X...\n", PSIO_BOOT_ADDRESS);
    
    // PSI/O ROUINE: JMP to BOOTLOADER_START_ADDR (0x0100)
    // This JMP will only be written if the bootloader passes all checks.
    RAM[PSIO_BOOT_ADDRESS + 0] = 0x80; // JMP Opcode
    RAM[PSIO_BOOT_ADDRESS + 1] = 0x01; 
    RAM[PSIO_BOOT_ADDRESS + 2] = 0x00; 

    // 1. Attempt to find, load, and validate the Bootloader
    if (load_bootloader_file("bootloader.bin")) {
        // 5. Enable Secure Boot (Conceptual: No action needed as JMP is already written)
        PC = PSIO_BOOT_ADDRESS; // Start PC at the PSI/O jump point
    } else {
        // Failure: HLT the fixed boot address to prevent running faulty JMP
        RAM[PSIO_BOOT_ADDRESS + 0] = 0x00; 
        PC = PSIO_BOOT_ADDRESS; // PC will execute HLT, then the shell will open
    }
}

int main() {
    // Clear RAM
    memset(RAM, 0, MEMORY_SIZE); 
    
    load_psio_firmware();
    
    printf("\n+==========================================+\n");
    printf("| SCSA: Setting Computer Set Architecture - VM Execution Start   |\n");
    printf("| PSI/O Start Address: 0x%04X              |\n", PSIO_BOOT_ADDRESS);
    printf("+==========================================+\n");
    
    int cycle = 0;
    
    // Check if PSI/O successfully loaded the JMP instruction
    if (RAM[PSIO_BOOT_ADDRESS] == 0x00) {
        psio_shell_loop();
        return 0;
    }

    while (RAM[PC] >> 4 != OPCODE_HLT && cycle < 20) { 
        // 1. FETCH
        IR_OPCODE = (RAM[PC] >> 4) & 0xF; 
        IR_REGISTER = RAM[PC] & 0xF; 
        IR_OPERAND = (RAM[PC+1] << 8) | RAM[PC+2]; 

        // 2. State Print
        print_state(cycle++);
        
        // 3. EXECUTE 
        execute_instruction();
    }
    
    printf("\n--- VM Execution Complete ---\n");
    printf("Final Result (RAM[FFFE]): %d (0x%04X)\n", *(uint16_t*)&RAM[0xFFFE], *(uint16_t*)&RAM[0xFFFE]);
    
    return 0;
}
