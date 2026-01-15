# GDB & Peda

https://www.gnu.org/software/gdb/
- allows you to see what is going on 'inside' another program while it executes
    -- what another program was doing at the moment it crashed
- GDB Quick Reference: https://www.cs.virginia.edu/~cr4bd/4630/S2017/gdb-cheat.html

## Install right GDB version

- make sure GDB version 13.1 or older is installed
- the latest version of GDB doesn't seem to work with PEDA library that we need


```python
! gdb --version
```

    [35;1mGNU gdb (Debian 13.1-1) 13.1[m
    Copyright (C) 2023 Free Software Foundation, Inc.
    License GPLv3+: GNU GPL version 3 or later <[32mhttp://gnu.org/licenses/gpl.html[m>
    This is free software: you are free to change and redistribute it.
    There is NO WARRANTY, to the extent permitted by law.



```python
! echo kali | sudo -S apt -y update
```

    Get:1 http://kali.download/kali kali-rolling InRelease [41.5 kB]
    Get:2 http://kali.download/kali kali-rolling/main amd64 Packages [20.6 MB]
    Get:3 http://kali.download/kali kali-rolling/main amd64 Contents (deb) [49.0 MB]
    Get:4 http://kali.download/kali kali-rolling/contrib amd64 Packages [115 kB][33m[33m[33m[33m
    Get:5 http://kali.download/kali kali-rolling/contrib amd64 Contents (deb) [267 kB]
    Get:6 http://kali.download/kali kali-rolling/non-free amd64 Packages [202 kB]
    Get:7 http://kali.download/kali kali-rolling/non-free amd64 Contents (deb) [882 kB]
    Get:8 http://kali.download/kali kali-rolling/non-free-firmware amd64 Packages [10.8 kB]
    Get:9 http://kali.download/kali kali-rolling/non-free-firmware amd64 Contents (deb) [24.2 kB]
    Fetched 71.2 MB in 10s (6,860 kB/s)                                            [0m[33m[33m[33m[33m[33m[33m[33m[33m[33m[33m[33m[33m[33m[33m[33m
    Reading package lists... Done
    Building dependency tree... Done
    Reading state information... Done
    2033 packages can be upgraded. Run 'apt list --upgradable' to see them.



```python
%cd ~
```

    /home/kali



```python
! wget https://snapshot.debian.org/archive/debian/20230221T024615Z/pool/main/g/gdb/gdb_13.1-1_amd64.deb
```

    --2025-03-03 18:00:37--  https://snapshot.debian.org/archive/debian/20230221T024615Z/pool/main/g/gdb/gdb_13.1-1_amd64.deb
    Resolving snapshot.debian.org (snapshot.debian.org)... 151.101.66.132, 151.101.130.132, 151.101.194.132, ...
    Connecting to snapshot.debian.org (snapshot.debian.org)|151.101.66.132|:443... connected.
    HTTP request sent, awaiting response... 302 FOUND
    Location: /file/f7362cdb59ddad6bcc263808513e41828c9e2b1d/gdb_13.1-1_amd64.deb [following]
    --2025-03-03 18:00:37--  https://snapshot.debian.org/file/f7362cdb59ddad6bcc263808513e41828c9e2b1d/gdb_13.1-1_amd64.deb
    Reusing existing connection to snapshot.debian.org:443.
    HTTP request sent, awaiting response... 200 OK
    Length: 3818268 (3.6M)
    Saving to: ‘gdb_13.1-1_amd64.deb.2’
    
    gdb_13.1-1_amd64.de 100%[===================>]   3.64M  15.8MB/s    in 0.2s    
    
    2025-03-03 18:00:37 (15.8 MB/s) - ‘gdb_13.1-1_amd64.deb.2’ saved [3818268/3818268]
    



```python
! ls
```

    Desktop			gdb_13.deb	   Miniconda.sh      SoftwareSecurity
    Documents		gdb-16.2	   Music	     Sp25
    Downloads		gdb-16.2.tar.gz    peda		     Spring25Demo.gpr
    gdb-10.2		gdb-16.2.tar.gz.1  Pictures	     Spring25Demo.rep
    gdb-10.2.tar.gz		gdb.deb		   projects	     Templates
    gdb_13.1-1_amd64.deb	ghidra-ils	   Public	     Videos
    gdb_13.1-1_amd64.deb.1	ghidra-ils.tar.gz  Python-3.10.1
    gdb_13.1-1_amd64.deb.2	miniconda3	   Python3.10.1.tgz



```python
! echo kali | sudo -S dpkg -i ~/gdb_13.1-1_amd64.deb
```

    (Reading database ... 425554 files and directories currently installed.)
    Preparing to unpack .../home/kali/gdb_13.1-1_amd64.deb ...
    Unpacking gdb (13.1-1) over (13.1-1) ...
    Setting up gdb (13.1-1) ...
    Processing triggers for kali-menu (2023.4.7) ...
    Processing triggers for man-db (2.12.0-3) ...



```python
! gdb --version
```

    [35;1mGNU gdb (Debian 13.1-1) 13.1[m
    Copyright (C) 2023 Free Software Foundation, Inc.
    License GPLv3+: GNU GPL version 3 or later <[32mhttp://gnu.org/licenses/gpl.html[m>
    This is free software: you are free to change and redistribute it.
    There is NO WARRANTY, to the extent permitted by law.


## Configure GDB

### Add the following settings in ~/.gdbinit file

```
# Intel syntax is more readable
set disassembly-flavor intel
 
# When inspecting large portions of code the scrollbar works better than 'less'
set pagination off

# Keep a history of all the commands typed. Search is possible using ctrl-r
set history save on
set history filename ~/.gdb_history
set history size 32768
set history expansion on
```

## GDB demo

- use `demos/stack_overflow/stack_demo.cpp` to see stack segment as discussed in x86Assembly and Stack chapter

## compile with -g switch and load binary
- `g++ -g -m32 -o outputProgram inputFile.cpp`
- `gdb -q ./outputProgram` # quietly run gdb
- use `demos/stack_overflow/stack_demo.cpp` program to demo useful gdb commands


```python
! cat ../demos/stack_overflow/stack_demo.cpp
```

    #include <iostream>
    #include <cstdio>
    
    using namespace std;
    
    int function(int para1, int para2, int para3) {
        int var1, var2, var3;
        var1 = 1;
        var2 = 2;
        var3 = 3;
        return (para1 + para2 + para2 + var1 + var2 + var3);
    }
    
    int main(int argc, char * argv[]) {
        printf("before function call\n");
        int n = function(10, 20, 30);
        printf("result = %d\n", n);
        return 0;
    }

## Useful commands

### help
- `help`
- `help <instruction>`

### starting/stopping
- `run ARGS` : start the program with the arguments ARGS
- `run ARGS < input.txt` : start the program with the arguments ARGS, providing std input from the input.txt file
- `kill` : terminate the current program
- `step or s` : step into the function
- `stepi or si` : step forward by one instruction (step into function)
- `next or n` : step forward one statement or function call (execte the entire function with one keypress) 
- `nexti or ni` : step forward by one instruction (skipping any called function)
- `continue or c` : run until next breakpoint

### break points, break or b
- `break [line number, function name, memory address]`: stop execution at the breakpoint
- `break function`: set a breakpoint at entry to a particular function

### disassemble
- `disassemble function` : disassemble a function by name
- `set disassembly-flavor intel` : use Intel assembly syntax

### print variables and memory address, and basic arithmetic
- `print 2+2`
- `print $ebp + 4`
- `print varName`
- `print $ebp` : similar to `i r ebp`

### print format
- print or p/format [value/address/variable]
- e.g. p/x $ebp+8
- p/x - print in hex (default)
- p/d - print as signed decimal
- p/u - print as unsigned decimal
- p/o - print as octal
- p/t - print as binary
- p/c - print as character
- p/s - print as string

## memory examination and data format
- **x/[count][size][format] [register/memory]**
    - count: no. of size to display starting from the register/memory location
    - format:
        - **o** : display in octal
        - **x** : display in hexadecimal
        - **u** : display in unsigned (base-10 decimal)
        - **d** : display in base-10 decimal
        - **t** : display in binary
        - **s** : string
        - **c** : character
    - size:
        - **b** : a single byte
        - **h** : a halfword (2 bytes)
        - **w** : a word (4 bytes) - default
        - **g** : giant (8 bytes)
- **x/40wx $esp** : display 40 word bytes in hex format from top of the stack 

# PEDA - Python Exploit Development Assistance for GDB

## Installation
https://github.com/longld/peda
See #3. Installation


## Useful commands
### can still run all GDB commands inside PEDA!

### help
```
gdb-peda$ peda help
gdb-peda$ help <keyword>
gdb-peda$ help <command>
```

### show selected context not everything while stepping through

- **context all** : run by default whenever a breakpoint is hit
- **context reg** : for the registers and flags
- **context code** : for disassembling around the current instruction pointer
- **context stack** : for examining the stack

### quickly check the security settings of the program

```bash
gdb-peda$ checksec
```

### generate pattern
- generate pattern and store in a file

```bash
gdb-peda$ pattern_create 120 file
```

- generate pattern and set as argument
```bash
gdb-peda$ pattern arg 100
```

### search pattern
- crash the program with the pattern and run patts or pattern_search
- look for EIP offset which is the offset from the target buffer to the return address
```
gdb-peda$ pattern_search
```


### generate shell code

- `gdb-peda$ shellcode`
- `gdb-peda$ shellcode generate`
- `gdb-peda$ shellcode generate x86/linux exec`

### Useful resources

#### Exploit writing using Python
- http://www.fuzzysecurity.com/tutorials/expDev/2.html

#### ROP and ROP Gadgets
- https://www.exploit-db.com/docs/english/28479-return-oriented-programming-(rop-ftw).pdf

#### Buffer overflow resources
- https://samsclass.info/123/proj14/lbuf1.htm
- http://www.tenouk.com/Bufferoverflowc/Bufferoverflow6.html
- https://tc.gtisc.gatech.edu/cs6265/2016/l/lab02-warmup2/README-tut.txt - peda


```python

```
