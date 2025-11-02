#!/bin/bash

echo "----------------------------------------------------"
echo "[SEMU Launcher] Starting Native Xorg/Raylib Session..."

# Check if DISPLAY is set. If not, inform the user to start Xorg first.
if [ -z "$DISPLAY" ]; then
    echo "!!! ERROR: DISPLAY environment variable is NOT set."
    echo "!!! Please start your Xorg session (e.g., 'startxfce4' or 'vncserver') first."
    echo "----------------------------------------------------"
    exit 1
fi

echo "Xorg DISPLAY is set to: ${DISPLAY}"
echo "Launching SEMU Core (C-Emulator) to draw directly to Xorg..."
echo "----------------------------------------------------"

# Execute the C-based SEMU core. It will block until Enter is pressed.
~/scsa/src/compiler/semu-120bit-emulator

echo "SEMU session terminated. Returning to zsh."

