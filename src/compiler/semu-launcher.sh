#!/bin/bash

VNC_PORT=":1"  
DISPLAY_ADDR="localhost${VNC_PORT}"
KILL_VNC="Xvnc" # Use generic name to avoid killing the launcher itself

echo "----------------------------------------------------"
echo "Starting Xvnc Server for SEMU Display (Persistent Mode)..."
# Start Xvnc (Use nohup to keep it running easily)
# NOTE: Replace 'Xvnc' with the actual command in your Termux setup if needed!
Xvnc ${VNC_PORT} -geometry 1024x768 &
VNC_PID=$!
echo "Xvnc started on ${VNC_PORT} (PID: ${VNC_PID})."
sleep 2 

# Update the DISPLAY environment variable
export DISPLAY=${VNC_PORT}

echo "Launching SEMU Core (C-Emulator) on display ${DISPLAY_ADDR}..."

# Execute the C-based SEMU core (which will now wait for user input)
~/scsa/src/compiler/semu-120bit-emulator

# --- MANUAL SEMU KILLALL ---
echo "----------------------------------------------------"
echo "[SEMU_KILLALL] Secure Termination Protocol Initiated."
echo "Killing Xvnc Process (PID: ${VNC_PID})..."
kill -9 ${VNC_PID} 2>/dev/null 
echo "Cleanup complete. Xvnc process terminated."

