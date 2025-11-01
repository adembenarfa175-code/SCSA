# 💻 SCSA: Simple Computer Set Architecture (1-Bit CPU)

## Overview

The **SCSA (Simple Computer Set Architecture)** is a fully documented, open-source 1-bit CPU project designed purely for educational purposes. It demonstrates the fundamental principles of Computer Architecture, focusing on the core interaction between the CPU, Memory, and I/O using the simplest possible logic gates and data paths.

This architecture uses an **8-bit fixed instruction format** (4-bit Opcode + 4-bit Operand/Address).

---

## 🏗️ Project Structure

The repository is organized into distinct directories for hardware design, software tools, and documentation:

```text
.
├── Assembler.txt        # Full SCSA Instruction Set Architecture (ISA) documentation.
├── LICENSE              # MIT License (Open Source details).
├── README.md            # This file.
├── RAM-system.pcb       # Logic diagram for the Memory and I/O Subsystem.
├── cpu-1-dits.pcb       # Logic diagram for the 1-bit CPU Core (ALU, Registers).
├── pc-1-bit.pcb         # Unified and complete system architecture diagram.
├── psio-system.pcb      # Logic diagram for the PSI/O (Boot ROM/Firmware).
└── code
    ├── compiler
    │   ├── compiler-asm     # The compiled assembler executable.
    │   └── compiler-asm.c   # C source code for the Assembler.
    └── psio
        └── main.asm       # The PSI/O (Simple Bootloader) program source code.

