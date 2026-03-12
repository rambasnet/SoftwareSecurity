#!/usr/bin/env bash
set -euo pipefail

python -m pip install --upgrade pip
python -m pip install -r requirements.txt

ulimit -c unlimited
if ! sudo sysctl -w kernel.core_pattern=core >/dev/null 2>&1; then
  echo "Warning: unable to set kernel.core_pattern (likely read-only in this container); continuing."
fi

# Install popular pwn helper tools from Python ecosystem.
python -m pip install ropper ROPGadget
