import os
from pwn import *


#shellcode = asm(shellcraft.sh())
shellcode = asm(shellcraft.cat('/etc/passwd'))

# Create an executable buffer
shellcode_exec = make_elf(shellcode)

# Save and execute the shellcode
with open("shellcode_exec.exe", "wb") as f:
    f.write(shellcode_exec)

os.system("chmod +x shellcode_exec.exe && ./shellcode_exec.exe")
#asm(shellcraft.cat('/etc/passwd')))
