# SCSA - Simple Computer Set Architecture (1-Bit)

SCSA is an open-source, educational 1-bit CPU architecture designed to demonstrate the fundamental principles of computer engineering, control unit design (CUTT), and instruction set architecture (ISA).

## Project Structure

* `arch/`: PCB layouts and physical design logic (using a placeholder format).
* `doc/`: Full ISA reference (`Assembler.txt`).
* `src/compiler/`: C-based tools including the `scsa-calculator` VM.
* `src/psio/`: Sample Assembly code (`main.asm`).

## Status

The **SCSA Virtual Calculator** (`scsa-calculator.c`) successfully emulates the full instruction set, including the new ADD instruction.
