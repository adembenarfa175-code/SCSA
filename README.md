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
