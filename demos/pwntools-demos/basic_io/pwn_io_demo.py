#!/usr/bin/env python

import pwn

# run process to interact with
io = pwn.process('./io_demo.exe')

# programmatically receive and use data

prompt = io.recvline(False)
print(f'prompt:  {prompt}')
# receives bytes array
if prompt.endswith(b'name:'):
    io.sendline(b"John Smith")

prompt = io.recvuntil(b"age:")
print(f"prompt = {prompt}")
# can only send bytes and string
io.sendline(b'21')

prompt = io.recvuntil(b"address:")
print(f"prompt = {prompt}")

io.sendline(b"111 North Street GJ CO 1111!")
print(f'{io.recv()}') # receive and discard \n
print(f'{io.recvline()}') # includes \n
print(io.recvline().decode('utf-8'), end='')

#io.interactive()
