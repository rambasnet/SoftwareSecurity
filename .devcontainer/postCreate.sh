#!/usr/bin/env bash
set -euo pipefail

python -m pip install --upgrade pip
python -m pip install -r requirements.txt

# Install popular pwn helper tools from Python ecosystem.
python -m pip install ropper ROPGadget

# Install one_gadget Ruby gem if not already present.
if ! gem list -i one_gadget >/dev/null 2>&1; then
  gem install one_gadget --no-document
fi

# Install GEF and auto-load it in gdb.
if [ ! -f "$HOME/.gdbinit-gef.py" ]; then
  wget -q -O "$HOME/.gdbinit-gef.py" https://gef.blah.cat/sh
fi

if [ ! -f "$HOME/.gdbinit" ] || ! grep -q "source ~/.gdbinit-gef.py" "$HOME/.gdbinit"; then
  echo "source ~/.gdbinit-gef.py" >> "$HOME/.gdbinit"
fi
