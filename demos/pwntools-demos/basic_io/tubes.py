"""
launch an interactive shell
"""

from pwn import *

io = process('sh') # create io tube to to /usr/bin/sh program
# send echo command to shell via io tube
io.sendline('echo Hello, World!')
print(io.recvline())
io.interactive()
