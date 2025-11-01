// file: ~/scsa/src/compiler/assembler.c (SCSA: Setting Computer Set Architecture Assembler)
// Converts SCSA-16 Assembly Mnemonics (e.g., LDA R0, 0xE000) into Machine Code and outputs bootloader.bin.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

// --- CONSTANTS ---
#define RAM_SIZE 0x10000 // 64 KB
#define START_PC 0x0100  // Bootloader starting address

// --- OPCODE MAPPING for SCSA-16 ---
typedef struct {
    char *mnemonic;
    uint8_t opcode;
    int operand_count; 
} Instruction;

Instruction ISA[] = {
    {"HLT", 0x0, 0}, {"LDA", 0x2, 2}, {"STA", 0x3, 2}, {"LDI", 0x4, 2},
    {"JMP", 0x8, 1}, {"ADD", 0xC, 2}, {"SUB", 0xD, 2}, {"MUL", 0xE, 2},
    {NULL, 0, 0}
};

Instruction* find_instruction(char *token) {
    for (int i = 0; ISA[i].mnemonic != NULL; i++) {
        if (strcasecmp(ISA[i].mnemonic, token) == 0) return &ISA[i];
    }
    return NULL;
}

uint8_t get_register_id(char *reg_str) {
    if (strcasecmp(reg_str, "R0") == 0 || strcasecmp(reg_str, "ACC") == 0) return 0x0;
    return 0xFF; // Error code
}

uint16_t parse_operand(char *op_str) {
    if (op_str[0] == '0' && (op_str[1] == 'x' || op_str[1] == 'X')) {
        return (uint16_t)strtol(op_str, NULL, 16);
    } else {
        return (uint16_t)strtol(op_str, NULL, 10);
    }
}

// Global buffer to hold the machine code
uint8_t machine_code_buffer[RAM_SIZE];
int program_counter = START_PC;
int current_offset = 0; // Tracks the current offset within the machine_code_buffer

void assemble_line(char *line) {
    char *tokens[4];
    int token_count = 0;
    char *token = strtok(line, " \t,");
    
    while (token != NULL && token_count < 4) {
        if (token[0] == ';') break;
        tokens[token_count++] = token;
        token = strtok(NULL, " \t,");
    }

    if (token_count == 0) return;

    if (tokens[0][0] == '.') {
        if (strcasecmp(tokens[0], ".ORG") == 0 && token_count > 1) {
            program_counter = parse_operand(tokens[1]); 
            current_offset = program_counter; // Update offset when ORG is used
            printf("[ASM] Setting PC/Offset to: 0x%04X\n", program_counter);
        }
        // Handle .WORD directive (for data)
        if (strcasecmp(tokens[0], ".WORD") == 0 && token_count > 1) {
            uint16_t data = parse_operand(tokens[1]);
            machine_code_buffer[current_offset++] = (uint8_t)(data >> 8); // High Byte
            machine_code_buffer[current_offset++] = (uint8_t)(data & 0xFF);  // Low Byte
        }
        return;
    }

    Instruction *instr = find_instruction(tokens[0]);
    if (instr == NULL) {
        fprintf(stderr, "[ERROR] Unknown instruction: %s\n", tokens[0]);
        return;
    }

    uint8_t byte1 = 0;
    uint16_t operand = 0;
    int bytes_written = 3;

    if (instr->operand_count > 0) {
        // Check for Register and Operand
        uint8_t reg_id = get_register_id(tokens[1]);
        if (reg_id == 0xFF) { fprintf(stderr, "[ERROR] Invalid Register: %s\n", tokens[1]); return; }
        
        byte1 = (instr->opcode << 4) | reg_id;
        if (instr->operand_count == 1) { // JMP ADDR
            operand = parse_operand(tokens[1]); 
            byte1 = (instr->opcode << 4) | 0x0; // JMP doesn't use register field functionally
        } else {
            operand = parse_operand(tokens[2]); 
        }
    } else {
        byte1 = (instr->opcode << 4); // HLT
    }
    
    // Write to buffer
    machine_code_buffer[current_offset++] = byte1;
    machine_code_buffer[current_offset++] = (uint8_t)(operand >> 8); 
    machine_code_buffer[current_offset++] = (uint8_t)(operand & 0xFF);
    
    printf("[ASM] %04X: %s -> %02X %02X %02X\n", 
           program_counter, tokens[0], byte1, (uint8_t)(operand >> 8), (uint8_t)(operand & 0xFF));

    program_counter += bytes_written;
}

void assemble_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening assembly file");
        return;
    }
    
    char line[256];
    program_counter = START_PC; 
    current_offset = START_PC;
    memset(machine_code_buffer, 0, RAM_SIZE);

    printf("\n--- SCSA-16 Assembler Start ---\n");

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; 
        char line_copy[256];
        strcpy(line_copy, line);
        
        if (line_copy[0] == ';' || line_copy[0] == '\0' || isspace(line_copy[0])) continue;

        assemble_line(line_copy);
    }
    
    fclose(file);
    printf("--- SCSA-16 Assembler Complete. Size: %d Bytes ---\n", current_offset);

    // --- Write the Machine Code to bootloader.bin ---
    FILE *out_file = fopen("bootloader.bin", "wb");
    if (out_file == NULL) {
        perror("Error creating bootloader.bin");
        return;
    }
    // Write only the portion of RAM that contains the program/data (up to current_offset)
    fwrite(machine_code_buffer, 1, current_offset, out_file);
    fclose(out_file);
    printf("[OUTPUT] Successfully wrote machine code to: bootloader.bin\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <assembly_file.asm>\n", argv[0]);
        return 1;
    }
    assemble_file(argv[1]);
    return 0;
}
