#!/bin/bash

echo "Starting SCSA-16 Documentation Cleanup and English/Symbolic Update..."

# 1. Update README.md (General Overview)
cat > ~/scsa/README.md << 'E_README_EN'
# 🚀 SCSA-16: 16-Bit Computer Set Architecture Project (PSI/O Core)

This project is a 16-bit virtual machine (VM) designed to simulate a custom CPU architecture. The system has been upgraded from 8-bit to a **16-bit Word Size**, significantly enhancing processing power and memory addressing.

## 🧠 SCSA-16 Architectural Core Concepts

* **Word Size:** 16 bits (0 to 65535).
* **Memory Map:** 64 KB (Addresses `0x0000` to `0xFFFF`).
* **Instruction Format:** 24 bits (3 Bytes).
    * **Structure:** `[4-bit Opcode | 4-bit Register] | [16-bit Address/Operand]`
    * **Example:** `LDA R0, 0xE000` -> `(0x20 << 16) | 0xE000`

## ⚙️ Fixed Boot PSI/O Firmware

The **PSI/O (Processor/System I/O)** serves as the fixed **Firmware** for the SCSA-16, analogous to a modern **UEFI/BIOS** system.

| Concept | Value | Explanation |
| :---: | :---: | :---: |
| **Fixed Boot Address** | `0xF000` | The **VM Environment** (The 'surroundings') initializes the **Program Counter (PC)** at this fixed high memory address. |
| **PSI/O Operation** | `JMP 0x0010` | The first instruction executed is typically a Jump to `0x0010`, transferring control from Firmware to the **User Program**. This ensures a fast, reliable boot process. |
E_README_EN

# 2. Update PCB Files (Conceptual Architecture Confirmation)
cat > ~/scsa/arch/pc-16-bit.pcb << 'E_PCB16_1_EN'
// SCSA-16 - Simple Computer Set Architecture (16-Bit PCB)
// Main PCB Layout Definition - UPGRADED TO 16-BIT

// DATA & ADDRESSING
`DATA_WIDTH` = 16 bits
`ADDRESS_WIDTH` = 16 bits (~ 64KB RAM)
`INSTRUCTION_WIDTH` = 24 bits

// CORE COMPONENTS
`CPU_CORE`: 16-bit Data Path & 16-bit ALU
`RAM`: 64KB (0x0000 -> 0xFFFF)
`FIRMWARE_REGION`: Located at `0xF000` for Fixed Boot PSI/O.
E_PCB16_1_EN

cat > ~/scsa/arch/cpu-16-dits.pcb << 'E_PCB16_2_EN'
// SCSA-16 - CPU Core Logic (16-Bit)
// Instruction Decode Logic:

// 1. Fetch Cycle: Read 3 Bytes from RAM[PC]
// 2. Decode Cycle:
//    `OPCODE` = (RAM[PC] >> 4) & 0xF;  // Extract 4 high bits
//    `REGISTER` = RAM[PC] & 0xF;      // Extract 4 low bits (e.g., R0 = ACC)
//    `OPERAND` = (RAM[PC+1] << 8) | RAM[PC+2]; // Combine two bytes for 16-bit address/value

// 3. Execute Cycle:
//    `ACC` = `ACC` + `RAM[OPERAND]`  // Example: ADD Operation (0xC)
//    `PC` = `PC` + 3               // Normal Increment

// Register Mapping:
// R0 = ACCUMULATOR (16-bit)
// PC = PROGRAM COUNTER (16-bit)
E_PCB16_2_EN

# 3. Update Documentation (ISA)
cat > ~/scsa/doc/Instruction_Set_16bit.txt << 'E_DOC16_EN'
SCSA-16 Instruction Set Architecture (ISA) Reference - 16-Bit Version

Instruction Length: 3 Bytes (24 bits)

| Mnemonic | Opcode (Hex) | Operation | Symbolic Expression |
| :---: | :---: | :---: | :---: |
| HLT | 0x0 | Halt execution. | `CPU_STATE` = `~RUNNING` |
| LDA R, ADDR | 0x2 | Load 16-bit value from RAM. | `R` = `RAM[ADDR]` |
| STA R, ADDR | 0x3 | Store 16-bit value to RAM. | `RAM[ADDR]` = `R` |
| LDI R, VALUE | 0x4 | Load 16-bit immediate value. | `R` = `VALUE` |
| JMP ADDR | 0x8 | Unconditional jump. | `PC` = `ADDR` |
| ADD R, ADDR | 0xC | 16-bit Addition. | `R` = `R` + `RAM[ADDR]` |
| CMP R1, R2 | 0x1 | Compare. | `FLAG` = `R1` > `R2` |
| AND R, ADDR | 0x5 | Bitwise AND. | `R` = `R` & `RAM[ADDR]` |
| XOR R, ADDR | 0x6 | Bitwise XOR. | `R` = `R` ^ `RAM[ADDR]` |

## 💡 Fixed Boot / PSI/O Startup Flow

The core mechanism for system startup ensures fast control transfer:

1.  **Power ON:** Hardware initiates, and the Program Counter is hard-wired to `PC` = `0xF000`.
2.  **PSI/O Execution:** The CPU fetches the first instruction from `0xF000`. This instruction is the **JMP** to the user area.
3.  **Control Transfer:** `PC` is set to `0x0010`.
4.  **User Program Execution:** The user's code begins, running on the full 16-bit SCSA-16 architecture.

E_DOC16_EN

echo "SCSA-16 Documentation, English Translation, and Symbolic Concept Update Complete!"

# 4. Git instructions
echo "-----------------------------------------------------"
echo "Git/Github Instructions:"
echo "1. Staging files:"
echo "git add ~/scsa/README.md ~/scsa/arch/pc-16-bit.pcb ~/scsa/arch/cpu-16-dits.pcb ~/scsa/doc/Instruction_Set_16bit.txt"
echo "2. Committing changes:"
echo 'git commit -m "docs(en): Update all documentation to English with deep symbolic explanation of SCSA-16 and Fixed Boot PSI/O."'
echo "3. Pushing to GitHub:"
echo "git push"
echo "-----------------------------------------------------"
