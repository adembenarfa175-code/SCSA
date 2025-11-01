# 🛡️ SCSA: Setting Computer Set Architecture

**SCSA** is an educational project and a complete **Architectural Lab** designed to explore computer architecture from the ground up (1-bit to 16-bit) and challenge modern security standards. The focus is on **setting** a new standard for secure system initialization.

## 🧠 Core Architecture: SCSA-16 & The PSI/O Core

The current active model is **SCSA-16** (16-bit word size). Its central feature is the **PSI/O (Program Status Input/Output)** firmware, which acts as a robust, non-volatile system initializer.

### 🔒 PSI/O Secure Boot Mechanism

The **PSI/O** firmware (Fixed Boot Address: **0xF000**) executes a **7-Step Logical Security Check** to validate the `bootloader.bin` (User OS Loader) before transferring control. This eliminates the traditional vulnerabilities found in systems like BIOS/UEFI.

## 🚀 The Future Vision: SCSA-120 (Hyper-Security Core)

The ultimate goal is **SCSA-120**, a revolutionary 120-bit architecture focused entirely on hyper-security. It introduces a **hardware-fused 56-bit Security Tag** within its 120-bit word, making its boot sequence theoretically tamper-proof.
