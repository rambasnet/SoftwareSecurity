#! /usr/bin/env python3

from pwn import *

target_program = "io_demo.exe"

elf = ELF(target_program)

p = process(target_program)

# https://docs.pwntools.com/en/stable/util/cyclic.html
# create 200 characters cyclic length
p.sendline(cyclic(200, n=8))
p.wait()

core = p.corefile
offset = cyclic_find(core.read(core.esp, 8), n=8)
print(f'offset = {offset}')