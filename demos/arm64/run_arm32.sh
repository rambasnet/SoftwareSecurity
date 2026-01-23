#!/bin/bash

BINARY="$1"

if [ -z "$BINARY" ]; then
  echo "Usage: $0 <binary>"
  exit 1
fi

if ! [ -f "$BINARY" ]; then
  echo "Error: File '$BINARY' not found"
  exit 1
fi

echo "[*] Checking binary type..."
file "$BINARY" | grep -q '32-bit.*ARM' || {
  echo "❌ Not a 32-bit ARM binary."
  exit 1
}

echo "[*] Ensuring armhf architecture support..."
sudo dpkg --add-architecture armhf
sudo apt update

echo "[*] Installing 32-bit ARM runtime libraries..."
sudo apt install -y libc6:armhf libstdc++6:armhf

echo "[*] Checking kernel compatibility..."
if zgrep -q CONFIG_COMPAT=y /proc/config.gz; then
  echo "✅ Kernel supports 32-bit execution"
else
  echo "⚠️ Kernel may not support 32-bit. Trying QEMU fallback..."
fi

echo "[*] Attempting native execution..."
if ./"$BINARY"; then
  echo "✅ Binary ran natively!"
  exit 0
else
  echo "❌ Native execution failed. Trying qemu-arm..."
fi

if ! command -v qemu-arm &> /dev/null; then
  echo "[*] Installing qemu-user..."
  sudo apt install -y qemu-user
fi

echo "[*] Running with qemu-arm..."
qemu-arm ./"$BINARY"
