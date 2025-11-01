# 🚀 SCSA-16: 16-Bit Computer Set Architecture (PSI/O Core)

This is an educational project focusing on Computer Architecture design. It models a simple processor to deeply understand the Fetch-Decode-Execute cycle and the security challenges of modern systems.

## 🧠 SCSA Architectural Roadmap

| Architecture | Word Size | Key Focus | Status |
| :---: | :---: | :---: | :---: |
| **SCSA-16** | 16-bit | **Secure Boot PSI/O**. Complete ASM/Emulator core. | Active |
| **SCSA-64** | 64-bit | **Memory Expansion (256 TB)**. Transition to 4-byte Instruction Words. | Next Phase |
| **SCSA-120** | 120-bit | **Hyper-Security Core**. Custom security tagging (PSI/O Security Tag). | Future Vision |

## 🛡️ Deep Dive: The PSI/O Secure Boot Concept

The Program Status Input/Output (PSI/O) acts as a **Fixed Boot Firmware (at 0xF000)**. It replaces traditional BIOS/UEFI complexity with a rigorous **7-Step Logical Security Check** to ensure only validated, SCSA-16 specific code (bootloader.bin) is executed.

* **Failure Mode:** If any security check fails (e.g., wrong size, wrong architecture tag), the PSI/O jumps to the **PSI/O Shell** for diagnostics, preventing execution of untrusted code.
