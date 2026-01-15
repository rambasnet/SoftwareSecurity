# Buffer Overflow Protections
- modern kernel/OS and compilers provide various protections from buffer overflow attacks
- Reference: https://ocw.cs.pub.ro/courses/cns/labs/lab-06

## 1. ASLR - Address Space Layout Randomization
- Kernel specific protection
- loads the stack, data and program in random memory locations making it difficult (not impossible) to guess their addresses

- the following segments are possible to randomize on x86:
    - **stack** is easily randomizable, as all stack addresses are relative to `esp` or `ebp`
    - **Data segment** may be randomized, if e.g., the data segment is set to a random value
    - **Code** can only be randomized by comiling the program as Position Independent Code/Position Independent Executable
       - default for shared libraries, but otherwise executable code is usually placed at fixed addresses

### ASLR values
- Linux allows 3 options:
    - 0: disabled
    - 1: randomize stack, vdso, libraries (except data? not sure...)
    - 2: enabled (including data segment - default value)
    
### checking ASLR
- compile and run demos/aslr.cpp program to check ASLR
- run gdb-peda 

    ```bash
    aslr # run aslr command
    ASLR is OFF # output
    ```


```python
! echo kali | sudo -S cat /proc/sys/kernel/randomize_va_space
```

    0



```python
! echo kali | sudo -S bash -c 'echo 2 | sudo -S tee /proc/sys/kernel/randomize_va_space'
```

    2



```python
! cat ../demos/aslr/aslr.cpp
```

    // aslr.cpp - program to check address space layout randomization
    // compile and run the program multiple times
    // if you see same value for $esp, ASLR is disabled
    // $ g++ -g -m 32 -o aslr.exe aslr.cpp
    #include <stdio.h>
    #include <iostream>
    using namespace std;
    
    int global_var = 100; // data segment
    char global_name[20]; // bss segment
    
    int main(void) {
        char buffer[] = "Hello World!";
        register int esp asm("esp");
        register int ebp asm("ebp");
        cout << "Registers info:\n";
        cout << "===============\n";
        printf("$esp = %p\n", esp);
        printf("$ebp = %p\n", ebp);
        cout << "\nData and BSS segement\n";
        cout << "======================\n";
        printf("global_var is at: %p\n", &global_var);
        printf("global_name is at: %p\n", global_name);
        cout << "\nStack variable:\n";
        cout << "===============\n";
        printf("buffer is at: %p\n", buffer);
        cout << "\nCode segment:\n";
        cout << "=============\n";
        printf("main is at %p\n", main);
        cout << "\nShared library code\n";
        cout << "=====================\n";
        printf("printf is at %p\n", printf);
        printf("cout is at %p\n", &cout); // cout is an operator
        return 0;
    }


```bash
%%bash
input="../demos/aslr/aslr.cpp"
output="aslr.exe"
g++ -m32 $input -o $output
```

    ../demos/aslr/aslr.cpp: In function ‘int main()’:
    ../demos/aslr/aslr.cpp:19:21: warning: format ‘%p’ expects argument of type ‘void*’, but argument 2 has type ‘int’ [-Wformat=]
       19 |     printf("$esp = %p\n", esp);
          |                    ~^     ~~~
          |                     |     |
          |                     void* int
          |                    %d
    ../demos/aslr/aslr.cpp:20:21: warning: format ‘%p’ expects argument of type ‘void*’, but argument 2 has type ‘int’ [-Wformat=]
       20 |     printf("$ebp = %p\n", ebp);
          |                    ~^     ~~~
          |                     |     |
          |                     void* int
          |                    %d



```python
! ./aslr.exe | tee aslr2-1.txt
```

    Registers info:
    ===============
    $esp = 0xffffc460
    $ebp = 0xffffc488
    
    Data and BSS segement
    ======================
    global_var is at: 0x56559008
    global_name is at: 0x56559010
    
    Stack variable:
    ===============
    buffer is at: 0xffffc46f
    
    Code segment:
    =============
    main is at 0x5655623d
    
    Shared library code
    =====================
    printf is at 0xf7c4bd30
    cout is at 0xf7fbbc00



```python
! ./aslr.exe | tee aslr2-2.txt
```

    Registers info:
    ===============
    $esp = 0xffffc460
    $ebp = 0xffffc488
    
    Data and BSS segement
    ======================
    global_var is at: 0x56559008
    global_name is at: 0x56559010
    
    Stack variable:
    ===============
    buffer is at: 0xffffc46f
    
    Code segment:
    =============
    main is at 0x5655623d
    
    Shared library code
    =====================
    printf is at 0xf7c4bd30
    cout is at 0xf7fbbc00



```python
# run the diff of aslr2-1.txt and aslr2-2.txt
! diff aslr2-1.txt aslr2-2.txt
```


```python
# ldd invokes the standard dynamic linker with LD_TRACE_LOADED OBJECTS
# causes the dynamic linker to inspect the program's dynamic dependencies and the objects
! ldd ./aslr.exe
```

    	linux-gate.so.1 (0xf7f83000)
    	libstdc++.so.6 => /lib32/libstdc++.so.6 (0xf7c00000)
    	libc.so.6 => /lib32/libc.so.6 (0xf7800000)
    	libm.so.6 => /lib32/libm.so.6 (0xf7e5a000)
    	/lib/ld-linux.so.2 (0xf7f85000)
    	libgcc_s.so.1 => /lib32/libgcc_s.so.1 (0xf7e33000)



```python
# see the library loaded in different addresses
# run it few times to see the random addresses...
! ldd ./aslr.exe
```

    	linux-gate.so.1 (0xf7fc3000)
    	libstdc++.so.6 => /lib32/libstdc++.so.6 (0xf7c00000)
    	libc.so.6 => /lib32/libc.so.6 (0xf7800000)
    	libm.so.6 => /lib32/libm.so.6 (0xf7e9a000)
    	/lib/ld-linux.so.2 (0xf7fc5000)
    	libgcc_s.so.1 => /lib32/libgcc_s.so.1 (0xf7e73000)


### Set ASLR to option 0
- disable ASLR


```python
! echo kali | sudo -S bash -c 'echo 0 | sudo tee /proc/sys/kernel/randomize_va_space'
```

    0



```python
! echo kali | sudo -S cat /proc/sys/kernel/randomize_va_space
```

    0



```python
! ./aslr.exe | tee aslr0-1.txt
```

    Registers info:
    ===============
    $esp = 0xffffc460
    $ebp = 0xffffc488
    
    Data and BSS segement
    ======================
    global_var is at: 0x56559008
    global_name is at: 0x56559010
    
    Stack variable:
    ===============
    buffer is at: 0xffffc46f
    
    Code segment:
    =============
    main is at 0x5655623d
    
    Shared library code
    =====================
    printf is at 0xf7c4bd30
    cout is at 0xf7fbbc00



```python
! ./aslr.exe | tee aslr0-2.txt
```

    Registers info:
    ===============
    $esp = 0xffffc460
    $ebp = 0xffffc488
    
    Data and BSS segement
    ======================
    global_var is at: 0x56559008
    global_name is at: 0x56559010
    
    Stack variable:
    ===============
    buffer is at: 0xffffc46f
    
    Code segment:
    =============
    main is at 0x5655623d
    
    Shared library code
    =====================
    printf is at 0xf7c4bd30
    cout is at 0xf7fbbc00



```python
! diff aslr0-1.txt aslr0-2.txt
# no difference!
```

### ldd command

- print shared object dependencies
- https://man7.org/linux/man-pages/man1/ldd.1.html
- don't run ldd on untrusted ELF
- use objectdump instead

```bass
 $ objdump -p /path/to/program | grep NEEDED
 ```


```python
! ldd ./aslr.exe
```

    	linux-gate.so.1 (0xf7fc7000)
    	libstdc++.so.6 => /lib32/libstdc++.so.6 (0xf7c00000)
    	libc.so.6 => /lib32/libc.so.6 (0xf7800000)
    	libm.so.6 => /lib32/libm.so.6 (0xf7e9e000)
    	/lib/ld-linux.so.2 (0xf7fc9000)
    	libgcc_s.so.1 => /lib32/libgcc_s.so.1 (0xf7e77000)



```python
! ldd ./aslr.exe
```

    	linux-gate.so.1 (0xf7fc7000)
    	libstdc++.so.6 => /lib32/libstdc++.so.6 (0xf7c00000)
    	libc.so.6 => /lib32/libc.so.6 (0xf7800000)
    	libm.so.6 => /lib32/libm.so.6 (0xf7e9e000)
    	/lib/ld-linux.so.2 (0xf7fc9000)
    	libgcc_s.so.1 => /lib32/libgcc_s.so.1 (0xf7e77000)



```python
# detailed but not as good results as ldd
! objdump -p ./aslr.exe
```

    
    ./aslr.exe:     file format elf32-i386
    
    Program Header:
        PHDR off    0x00000034 vaddr 0x00000034 paddr 0x00000034 align 2**2
             filesz 0x00000180 memsz 0x00000180 flags r--
      INTERP off    0x000001b4 vaddr 0x000001b4 paddr 0x000001b4 align 2**0
             filesz 0x00000013 memsz 0x00000013 flags r--
        LOAD off    0x00000000 vaddr 0x00000000 paddr 0x00000000 align 2**12
             filesz 0x00000580 memsz 0x00000580 flags r--
        LOAD off    0x00001000 vaddr 0x00001000 paddr 0x00001000 align 2**12
             filesz 0x000005a0 memsz 0x000005a0 flags r-x
        LOAD off    0x00002000 vaddr 0x00002000 paddr 0x00002000 align 2**12
             filesz 0x00000348 memsz 0x00000348 flags r--
        LOAD off    0x00002eb4 vaddr 0x00003eb4 paddr 0x00003eb4 align 2**12
             filesz 0x00000158 memsz 0x00000174 flags rw-
     DYNAMIC off    0x00002ec0 vaddr 0x00003ec0 paddr 0x00003ec0 align 2**2
             filesz 0x00000100 memsz 0x00000100 flags rw-
        NOTE off    0x000001c8 vaddr 0x000001c8 paddr 0x000001c8 align 2**2
             filesz 0x00000060 memsz 0x00000060 flags r--
    0x6474e553 off    0x000001ec vaddr 0x000001ec paddr 0x000001ec align 2**2
             filesz 0x0000001c memsz 0x0000001c flags r--
    EH_FRAME off    0x00002140 vaddr 0x00002140 paddr 0x00002140 align 2**2
             filesz 0x0000006c memsz 0x0000006c flags r--
       STACK off    0x00000000 vaddr 0x00000000 paddr 0x00000000 align 2**4
             filesz 0x00000000 memsz 0x00000000 flags rw-
       RELRO off    0x00002eb4 vaddr 0x00003eb4 paddr 0x00003eb4 align 2**0
             filesz 0x0000014c memsz 0x0000014c flags r--
    
    Dynamic Section:
      NEEDED               libstdc++.so.6
      NEEDED               libc.so.6
      INIT                 0x00001000
      FINI                 0x00001588
      INIT_ARRAY           0x00003eb4
      INIT_ARRAYSZ         0x00000008
      FINI_ARRAY           0x00003ebc
      FINI_ARRAYSZ         0x00000004
      GNU_HASH             0x00000228
      STRTAB               0x00000328
      SYMTAB               0x00000248
      STRSZ                0x00000152
      SYMENT               0x00000010
      DEBUG                0x00000000
      PLTGOT               0x00003fc0
      PLTRELSZ             0x00000028
      PLTREL               0x00000011
      JMPREL               0x00000558
      REL                  0x000004f8
      RELSZ                0x00000060
      RELENT               0x00000008
      FLAGS                0x00000008
      FLAGS_1              0x08000001
      VERNEED              0x00000498
      VERNEEDNUM           0x00000002
      VERSYM               0x0000047a
      RELCOUNT             0x00000005
    
    Version References:
      required from libstdc++.so.6:
        0x08922974 0x00 04 GLIBCXX_3.4
      required from libc.so.6:
        0x0d696914 0x00 05 GLIBC_2.4
        0x0d696910 0x00 03 GLIBC_2.0
        0x09691f73 0x00 02 GLIBC_2.1.3
    


#### Allow ptrace processes
**ptrace** - allows parent process to observe and control the execution of another process
- used primarily to implement breakpoint debugging and system call tracing


```python
# check the ptrace value
! echo kali | sudo -S sysctl kernel.yama.ptrace_scope
```

    [sudo] password for kali: kernel.yama.ptrace_scope = 0



```python
# set the ptrace value
! echo kali | sudo -S sysctl -w kernel.yama.ptrace_scope=0
```

    [sudo] password for kali: kernel.yama.ptrace_scope = 0


### 2. PIE
- Position Independent Executable
- randomizes Code segment base address
- randomizes GOT (Global Offset Table for global data-variables) and PLT (Procedure Linkage Table)
    - PLT addresses are fixed during  linking to load the libc.so library codes/functions
    - shared libraries are position independent code (PIC); they don't know where they are being loaded
- disable PIE in gcc/g++ using **-no-pie** flag

### 3. Executable Stack Protection (NX) also called (DEP)
- also called DEP - Data Execution Prevention
- in newer g++, programs must be compiled by disabling stack protector to execute code injected into stack
- check executable stack:
`$ readelf -l <filename>`
- note the line: 
GNU_STACK      0x000000 0x00000000 0x00000000 0x00000 0x00000 RW  0x10
RW 
- RW - Read and Write but not Executable
- RWE - Read, Write, and Executable


```bash
%%bash
in=../demos/hello.cpp
out=hello.exe
g++ -g -m32 -o $out $in 

readelf -l $out | grep GNU_STACK
```

      GNU_STACK      0x000000 0x00000000 0x00000000 0x00000 0x00000 RW  0x10


#### enable RWX/disable DEP
- compile the program using -z execstack switch of gcc/g++


```bash
%%bash
in=../demos/hello.cpp
out=hello1.exe
g++ -g -m32 -z execstack -o $out $in 

readelf -l $out | grep GNU_STACK
```

      GNU_STACK      0x000000 0x00000000 0x00000000 0x00000 0x00000 RWE 0x10


### 4. Stack Canaries
- "canary in a coal mine" to detect carbon monoxide by using a warm blooded animal like a bird
- place a small random integer just before the stack return address
    - if the value is overwritten/corrupted during the function call, there's a stack overflow attempt!
- use **-fno-stack-protector** flag in gcc/g++ to disable canaries


### checksec command

- program to check security settings on an ELF
- GDB/PEDA provides checksec command
- to run from bash, need to install checksec


```python
! echo kali | sudo -S apt install checksec
```

    Reading package lists... Done
    Building dependency tree       
    Reading state information... Done
    The following NEW packages will be installed:
      checksec
    0 upgraded, 1 newly installed, 0 to remove and 14 not upgraded.
    Need to get 21.7 kB of archives.
    After this operation, 104 kB of additional disk space will be used.
    Get:1 http://archive.ubuntu.com/ubuntu focal/universe amd64 checksec all 2.1.0+git20191113.bf85698-2 [21.7 kB]
    Fetched 21.7 kB in 1s (42.9 kB/s)   [0m[33m
    
    7[0;23r8[1ASelecting previously unselected package checksec.
    (Reading database ... 70774 files and directories currently installed.)
    Preparing to unpack .../checksec_2.1.0+git20191113.bf85698-2_all.deb ...
    7[24;0f[42m[30mProgress: [  0%][49m[39m [..........................................................] 87[24;0f[42m[30mProgress: [ 20%][49m[39m [###########...............................................] 8Unpacking checksec (2.1.0+git20191113.bf85698-2) ...
    7[24;0f[42m[30mProgress: [ 40%][49m[39m [#######################...................................] 8Setting up checksec (2.1.0+git20191113.bf85698-2) ...
    7[24;0f[42m[30mProgress: [ 60%][49m[39m [##################################........................] 87[24;0f[42m[30mProgress: [ 80%][49m[39m [##############################################............] 8Processing triggers for man-db (2.9.1-1) ...
    
    7[0;24r8[1A[J


```python
! checksec --file=./aslr.exe
```

    RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH	Symbols		FORTIFY	Fortified	Fortifiable  FILE
    [32mFull RELRO   [m   [32mCanary found   [m   [32mNX enabled [m   [32mPIE enabled  [m   [32mNo RPATH [m  [32mNo RUNPATH [m  [31m83 Symbols   [m  [32mYes[m	0		1	./aslr.exe


### High level explanation

- https://blog.siphos.be/2011/07/high-level-explanation-on-some-binary-executable-security/

#### RELRO - Relocation Read Only

- the following resource provides good overview and example of RELRO
- https://www.redhat.com/en/blog/hardening-elf-binaries-using-relocation-read-only-relro
- Global Offset Table (GOT) and Procedure Linkage Table (PLT) can be made Read only

#### FORTIFY

- use compiler functionality to fortify certain variables that could potentially be overflown
- depends on the intelligence/feature of the compiler



```python

```
