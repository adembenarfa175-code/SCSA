#!/bin/bash

echo "Designing the SCSA-120 Secure Service Interface..."

# --- Create doc/SCSA-120-Service-Interface.txt ---
echo "Creating: doc/SCSA-120-Service-Interface.txt"
cat > ~/scsa/doc/SCSA-120-Service-Interface.txt << 'E_DOC_SVC_INT'
SCSA-120: PSI/O SECURE SERVICE INTERFACE DESIGN

The Service Interface is the mechanism by which the OS Kernel (or any application) communicates with the PSI/O Runtime Services. This interface is the heart of the "Setting" architecture's security model, ensuring all I/O is vetted.

## 1. The Service Call Mechanism (SVC)

Instead of direct hardware access (as in x86/BIOS), SCSA-120 forces all critical operations through a single, controlled entry point:

1. **Load:** The calling program loads the required **Service ID** into a dedicated 120-bit register (e.g., R_SVC).
2. **Execute:** The program executes a special **SVC** instruction (Conceptual Opcode: 0xFF).
3. **Validation:** The PSI/O Firmware (at 0xF000) intercepts the SVC instruction. It immediately validates the **Security Tag** (the 56 dedicated bits) of the R_SVC register.
4. **Execution/Return:** If the tag is valid, the service runs. The result is stored back into R_SVC.

## 2. Key Service Definitions (UEFI Equivalents)

| Service ID | SCSA-120 Service Name | UEFI Equivalent | Description |
| :---: | :---: | :---: | :---: |
| 0x01 | SERVICE_DIAGNOSTICS | Check/CRC | Performs a 120-bit integrity check on critical memory blocks. |
| 0x04 | SERVICE_TIMER_SET | SetTimerService | Provides controlled access to the system clock/timer hardware. |
| 0x07 | SERVICE_120BIT_ACCESS | GetMemoryMap | Safely returns chunks of the 2^120 memory map to the caller. |

This strictly defined interface prevents any program from directly manipulating the hardware, making the 120-bit security tag the ultimate gatekeeper.
E_DOC_SVC_INT

echo "Service Interface documentation created successfully."
