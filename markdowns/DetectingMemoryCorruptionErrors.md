# Detecting and Mitigating Memory Corruption Errors

## Memory corruption
- https://cwe.mitre.org/data/definitions/787.html
- according to MITRE, "memory corruption" is often used to describe the consequences of writing to memory outside the bounds of a buffer that is invalid, when the root cause is something other than a sequential copy of excessive data from a fixed starting location. This may include issues such as incorrect pointer arithmetic, accessing invalid pointers due to incomplete initialization or memory release, etc.

- from programmers point-of-view, there are two main ways to detect memory corruption errors in C/C++ programs
- White box and black box testing

## White box testing
- also called static analysis
- have access to source code
- manually read and review source code for memory related errors such as memory leak, buffer overflow, underflow, etc.
    - pros and cons?
- can use automated tools to scan for code and API that leads to memory related errors
    - pros and cons?

## Black box testing
- also called dynamic analysis
- manually test the binary/executable
- employ `fuzz testing` - use automated tools called fuzzer to provide invalid, unexpected or random data as inputs to the program

### Use 3rd party scanners such as Valgrind's Memcheck
- https://valgrind.org/docs/manual/quick-start.html
- compile your program using -g (dubuggin info) and -o1 (line numbers in error message)
    - `-o0` is also a good idea, if you can tolerate the slowdown   
- must install valgrind and libc6-dbg:i386 packages

### Use gcc/g++ compiler flags

### NOTE: Automated tools are not perfect!


```python
! echo kali | sudo -S apt install valgrind -y
```


```python
# check valgrind version
! valgrind --version
```

    valgrind-3.15.0



```bash
%%bash
# install libc6-dbg:i386 for debugging x86 program in x64
echo kali | sudo -S sudo apt install libc6-dbg -y
echo kali | sudo -S sudo apt install libc6-dbg:i386 -y
```


```python
! pwd
```

    /workspaces/SoftwareSecurity/notebooks



```python
%cd ../demos/mem_leak
```

    /workspaces/SoftwareSecurity/demos/mem_leak



```python
# let's use demos/memory_leak.cpp program for demo
! cat memory_leak.cpp
```

      #include <stdlib.h>
      #include <cstring>
      #include <cstdio>
    
      void f(char * arg)
      {
    	 // C dynamic memory
    	 int* x = (int *)malloc(10 * sizeof(int));
    	 // C++ dynamic memory
    	 char* name = new char[20];
    	 
    	 x[10] = 0;        // problem 1: heap block overrun
    		                // problem 2: memory leak -- x not freed
    	 strcpy(name, arg);
    	 // problem 3: heap block overrun
    	 // problem 4: memory leak -- x not freed
    	 printf("Hello %s\n", arg);
      }
    
      int main(int argc, char* argv[1])
      {
    	 // what if f() is called over and again in an infinite loop, e.g. 
    	 f(argv[1]);
    	 return 0;
      }



```python
# compile with -g -o0 options to use with valgrind
# compile as 64-bit binary as valgrind will not work on 32-bit due to lack of 
# libc6-dbg:i386 library
! g++ -g -o0 memory_leak.cpp -o memory_leak.exe
```


```python
# Run the program with an argument
! ./memory_leak.exe John
```

    Hello John



```python
# program crashes or behaves unexpectedly
! ./memory_leak.exe "some very very very very long string"
```

    malloc(): corrupted top size



```python
# by default gives summary of memory leak
# doesn't give the detail/full memory leaks info
! valgrind ./memory_leak.exe "John Smith"
```

    ==6012== Memcheck, a memory error detector
    ==6012== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
    ==6012== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
    ==6012== Command: ./memory_leak.exe John\ Smith
    ==6012== 
    ==6012== Invalid write of size 4
    ==6012==    at 0x1091DD: f(char*) (memory_leak.cpp:12)
    ==6012==    by 0x109236: main (memory_leak.cpp:23)
    ==6012==  Address 0x4daaca8 is 0 bytes after a block of size 40 alloc'd
    ==6012==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==6012==    by 0x1091C2: f(char*) (memory_leak.cpp:8)
    ==6012==    by 0x109236: main (memory_leak.cpp:23)
    ==6012== 
    Hello John Smith
    ==6012== 
    ==6012== HEAP SUMMARY:
    ==6012==     in use at exit: 60 bytes in 2 blocks
    ==6012==   total heap usage: 4 allocs, 2 frees, 73,788 bytes allocated
    ==6012== 
    ==6012== LEAK SUMMARY:
    ==6012==    definitely lost: 60 bytes in 2 blocks
    ==6012==    indirectly lost: 0 bytes in 0 blocks
    ==6012==      possibly lost: 0 bytes in 0 blocks
    ==6012==    still reachable: 0 bytes in 0 blocks
    ==6012==         suppressed: 0 bytes in 0 blocks
    ==6012== Rerun with --leak-check=full to see details of leaked memory
    ==6012== 
    ==6012== For lists of detected and suppressed errors, rerun with: -s
    ==6012== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)



```python
! valgrind --leak-check=full -s ./memory_leak.exe "John Smith"
```

    ==6043== Memcheck, a memory error detector
    ==6043== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
    ==6043== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
    ==6043== Command: ./memory_leak.exe John\ Smith
    ==6043== 
    ==6043== Invalid write of size 4
    ==6043==    at 0x1091DD: f(char*) (memory_leak.cpp:12)
    ==6043==    by 0x109236: main (memory_leak.cpp:23)
    ==6043==  Address 0x4daaca8 is 0 bytes after a block of size 40 alloc'd
    ==6043==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==6043==    by 0x1091C2: f(char*) (memory_leak.cpp:8)
    ==6043==    by 0x109236: main (memory_leak.cpp:23)
    ==6043== 
    Hello John Smith
    ==6043== 
    ==6043== HEAP SUMMARY:
    ==6043==     in use at exit: 60 bytes in 2 blocks
    ==6043==   total heap usage: 4 allocs, 2 frees, 73,788 bytes allocated
    ==6043== 
    ==6043== 20 bytes in 1 blocks are definitely lost in loss record 1 of 2
    ==6043==    at 0x483C583: operator new[](unsigned long) (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==6043==    by 0x1091D0: f(char*) (memory_leak.cpp:10)
    ==6043==    by 0x109236: main (memory_leak.cpp:23)
    ==6043== 
    ==6043== 40 bytes in 1 blocks are definitely lost in loss record 2 of 2
    ==6043==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==6043==    by 0x1091C2: f(char*) (memory_leak.cpp:8)
    ==6043==    by 0x109236: main (memory_leak.cpp:23)
    ==6043== 
    ==6043== LEAK SUMMARY:
    ==6043==    definitely lost: 60 bytes in 2 blocks
    ==6043==    indirectly lost: 0 bytes in 0 blocks
    ==6043==      possibly lost: 0 bytes in 0 blocks
    ==6043==    still reachable: 0 bytes in 0 blocks
    ==6043==         suppressed: 0 bytes in 0 blocks
    ==6043== 
    ==6043== ERROR SUMMARY: 3 errors from 3 contexts (suppressed: 0 from 0)
    ==6043== 
    ==6043== 1 errors in context 1 of 3:
    ==6043== Invalid write of size 4
    ==6043==    at 0x1091DD: f(char*) (memory_leak.cpp:12)
    ==6043==    by 0x109236: main (memory_leak.cpp:23)
    ==6043==  Address 0x4daaca8 is 0 bytes after a block of size 40 alloc'd
    ==6043==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==6043==    by 0x1091C2: f(char*) (memory_leak.cpp:8)
    ==6043==    by 0x109236: main (memory_leak.cpp:23)
    ==6043== 
    ==6043== ERROR SUMMARY: 3 errors from 3 contexts (suppressed: 0 from 0)


## gcc/g++ Warning flags and AddressSanitizer

- Google's ASan, introduced in 2012, uses a shadow memory scheme to detect memory bugs
- It is available in many modern comppilers
- https://en.wikipedia.org/wiki/AddressSanitizer
- https://gcc.gnu.org/onlinedocs/gcc-3.4.6/gcc/Warning-Options.html#Warning-Options

- `-Wall` - display all the warning
- `-Wpedantic` - display nonstandard warnings
- `-Wextra` - print extra newer warning messages
- `-Wconversion` - warning any implicit type conversions
- Warnings are like static analysis

- `-fsanitize=address` - use address sanitizer (ONLY works on Linux)
- must compile and run the program to see the results of any buffer-over-flow errors (dynamic analysis)
- For more: https://www.osc.edu/resources/getting_started/howto/howto_use_address_sanitizer


```python
! g++ -std=c++17 -g -o0 -Wall -Wpedantic -Wextra -Wconversion -fsanitize=address memory_leak.cpp -o memory_leak.exe
```


```python
# run the program to see the Address Sanitizer's result
# detects overflow during run-time
! ./memory_leak.exe
```


```python
# let's compile demos/stack_overflow/so_stdio.cpp with address sanitize flag and warning
! g++ -std=c++17 -m32 -g -o0 -Wall -Wpedantic -Wextra -Wconversion -fsanitize=address ../stack_overflow/so_stdio.cpp -o so_stdio.exe
```

    [01m[K../stack_overflow/so_stdio.cpp:[m[K In function ‘[01m[Kchar* mgets(char*)[m[K’:
    [01m[K../stack_overflow/so_stdio.cpp:29:16:[m[K [01;35m[Kwarning: [m[Kconversion from ‘[01m[Kint[m[K’ to ‘[01m[Kchar[m[K’ may change value [[01;35m[K-Wconversion[m[K]
       29 |         *ptr = [01;35m[Kch[m[K;
          |                [01;35m[K^~[m[K
    [01m[K../stack_overflow/so_stdio.cpp:35:20:[m[K [01;35m[Kwarning: [m[Kconversion from ‘[01m[Kint[m[K’ to ‘[01m[Kchar[m[K’ may change value [[01;35m[K-Wconversion[m[K]
       35 |         *(++ptr) = [01;35m[Kch[m[K;
          |                    [01;35m[K^~[m[K
    [01m[K../stack_overflow/so_stdio.cpp:[m[K In function ‘[01m[Kint main(int, char**)[m[K’:
    [01m[K../stack_overflow/so_stdio.cpp:51:14:[m[K [01;35m[Kwarning: [m[Kunused parameter ‘[01m[Kargc[m[K’ [[01;35m[K-Wunused-parameter[m[K]
       51 | int main([01;35m[Kint argc[m[K, char *argv[]) {
          |          [01;35m[K~~~~^~~~[m[K
    [01m[K../stack_overflow/so_stdio.cpp:51:26:[m[K [01;35m[Kwarning: [m[Kunused parameter ‘[01m[Kargv[m[K’ [[01;35m[K-Wunused-parameter[m[K]
       51 | int main(int argc, [01;35m[Kchar *argv[][m[K) {
          |                    [01;35m[K~~~~~~^~~~~~[m[K



```python
# let's manually test it... perhaps string not long enough
! echo "here you go some long long long string..." | ./so_stdio.exe
```

    buffer is at 0xff8ae300
    Give me some text: Acknowledged: here you go some long long long string... with length 41
    Good bye!



```python
# just overflow BUFFSIZE of 128
! python -c 'print("A"*200)' | ./so_stdio.exe
```

    buffer is at 0xff952fd0
    Give me some text: =================================================================
    [1m[31m==10736==ERROR: AddressSanitizer: stack-buffer-overflow on address 0xff953050 at pc 0x66120585 bp 0xff952f48 sp 0xff952f38
    [1m[0m[1m[34mWRITE of size 1 at 0xff953050 thread T0[1m[0m
        #0 0x66120584 in mgets(char*) ../stack_overflow/so_stdio.cpp:35
        #1 0x66120711 in bad() ../stack_overflow/so_stdio.cpp:46
        #2 0x6612083e in main ../stack_overflow/so_stdio.cpp:52
        #3 0xf1911ed4 in __libc_start_main (/lib32/libc.so.6+0x1aed4)
        #4 0x66120324 in _start (/workspaces/SoftwareSecurity/demos/mem_leak/so_stdio.exe+0x1324)
    
    [1m[32mAddress 0xff953050 is located in stack of thread T0 at offset 160 in frame[1m[0m
    [1m[0m    #0 0x661205e6 in bad() ../stack_overflow/so_stdio.cpp:41
    
      This frame has 1 object(s):
        [32, 160) 'buffer' (line 42)[1m[32m <== Memory access at offset 160 overflows this variable[1m[0m
    HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
          (longjmp and C++ exceptions *are* supported)
    SUMMARY: AddressSanitizer: stack-buffer-overflow ../stack_overflow/so_stdio.cpp:35 in mgets(char*)
    Shadow bytes around the buggy address:
      0x3ff2a5b0: [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m
      0x3ff2a5c0: [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m
      0x3ff2a5d0: [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m
      0x3ff2a5e0: [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m
      0x3ff2a5f0: [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[31mf1[1m[0m [1m[31mf1[1m[0m [1m[31mf1[1m[0m [1m[31mf1[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m
    =>0x3ff2a600: [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m[[1m[31mf3[1m[0m][1m[31mf3[1m[0m [1m[31mf3[1m[0m [1m[31mf3[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m
      0x3ff2a610: [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m
      0x3ff2a620: [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m
      0x3ff2a630: [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m
      0x3ff2a640: [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m
      0x3ff2a650: [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m [1m[0m00[1m[0m
    Shadow byte legend (one shadow byte represents 8 application bytes):
      Addressable:           [1m[0m00[1m[0m
      Partially addressable: [1m[0m01[1m[0m [1m[0m02[1m[0m [1m[0m03[1m[0m [1m[0m04[1m[0m [1m[0m05[1m[0m [1m[0m06[1m[0m [1m[0m07[1m[0m 
      Heap left redzone:       [1m[31mfa[1m[0m
      Freed heap region:       [1m[35mfd[1m[0m
      Stack left redzone:      [1m[31mf1[1m[0m
      Stack mid redzone:       [1m[31mf2[1m[0m
      Stack right redzone:     [1m[31mf3[1m[0m
      Stack after return:      [1m[35mf5[1m[0m
      Stack use after scope:   [1m[35mf8[1m[0m
      Global redzone:          [1m[31mf9[1m[0m
      Global init order:       [1m[36mf6[1m[0m
      Poisoned by user:        [1m[34mf7[1m[0m
      Container overflow:      [1m[34mfc[1m[0m
      Array cookie:            [1m[31mac[1m[0m
      Intra object redzone:    [1m[33mbb[1m[0m
      ASan internal:           [1m[33mfe[1m[0m
      Left alloca redzone:     [1m[34mca[1m[0m
      Right alloca redzone:    [1m[34mcb[1m[0m
      Shadow gap:              [1m[0mcc[1m[0m
    ==10736==ABORTING


## Fixing memory leak and over-run vulnerabilities
- find the vulnerable line of code/functions, etc. and fix it
- see `demos/mem_leak/memory_leak_fixed.cpp` for demo


```python
! cat memory_leak_fixed.cpp
```

    #include <stdlib.h>
    #include <cstring>
    #include <cstdio>
    
    void f(char * arg)
    {
    	// C dynamic memory
    	int* x = (int *)malloc(10 * sizeof(int));
    	// C++ dynamic memory
    	char* name = new char[20];
    
    	x[9] = 0; // problem 1: heap block overrun
    			// problem 2: memory leak -- x not freed
    	strncpy(name, arg, sizeof(char)*20-1);
    	name[19] = '\0';
    	// problem 3: heap block overrun
    	// problem 4: memory leak -- name not freed
    	printf("Hello %s\n", name);
    	free(x); // C
    	delete[] name; // C++
    }
    
    int main(int argc, char* argv[1])
    {
    	// what if f() is called over and again in an infinite loop, e.g. 
    	f(argv[1]);
    	return 0;
    }



```python
# compile with -g -o0 options to use with valgrind
! g++ -g -o0 -Wpedantic -Wextra -Wconversion -fsanitize=address memory_leak_fixed.cpp -o memory_leak_fixed.exe
```


```python
# manually check the fix
! ./memory_leak_fixed.exe "some very very very very long strin adfa asf afaf adfa dag"
```


```python
# check with valgrind
! valgrind --leak-check=yes ./memory_leak_fixed.exe "some very very very very long string"
```


```python

```
