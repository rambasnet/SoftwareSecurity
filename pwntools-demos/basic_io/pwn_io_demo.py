#!/usr/bin/env python
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template so_stdio.exe
import pwn

# run process to interact with
io = pwn.process('./program.exe')

# programmatically receive and use data

prompt = io.recvline(False)
print(f'received:  {prompt}')
# receives bytes array
if prompt.endswith(b'name:'):
    io.sendline("John Smith")

prompt = io.recvuntil("age:")
print(f"prompt = {prompt}")
# can only send bytes and string
io.sendline(b'21')

prompt = p.recvuntil("address:")
print(f"prompt = {prompt}")

io.sendline("111 North Street GJ CO 1111!")


# shellcode = asm(shellcraft.sh())
# payload = fit({
#     32: 0xdeadbeef,
#     'iaaa': [1, 2, 'Hello', 3]
# }, length=128)
# io.send(payload)
# flag = io.recv(...)
# log.success(flag)

io.interactive()

