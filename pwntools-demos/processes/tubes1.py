from pwn import *

# run bin/sh with command
io = process(['sh', '-c', 'echo A; sleep 1; echo B; sleep 1; echo C; sleep 1; echo ABC'])

print(io.recvline())
# 'A\n'

# receive 4 bytes
print(io.recvn(4))
# 'B\nC\n'

print(hex(io.unpack()))
# 0xa444444