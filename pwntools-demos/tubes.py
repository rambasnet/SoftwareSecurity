"""
Tubes Demos
Tubes are effectively I/O wrappers for most types of
"""

from pwn import *

io = process('sh')
#io.sendline('echo Hello, world')
#io.recvline()
io.interactive()