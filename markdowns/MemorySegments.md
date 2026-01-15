# Memory segments

## Table of Contents
- Memory segments
- Demo program

## Memory segments in C/C++ programs
- main memory (RAM) is typically divided into 2 main spaces:
    1. kernel space - reserved for running the kernel (OS background processes, device drivers, etc.)
        - loaded in lower addresses
    2. user space - user-mode applications are loaded
        - loaded in higher addresses
    
- in C/C++ programs, user space is divided into 5 different areas, called segments:

<img src="media/MemorySegments.png">

### Text segment
- also called a code segment
- compiled program/code is loaded
- code segment is typically read-only

### Data segment
- also called the initialized data segment
- initialized global and static variables are stored

### BSS (Block Started by Symbol) segment
- also called the uninitialized data segment)
- zero-initialized or uninitialized global and static variables are stored


### Heap segment
- memory for dynamically allocated variables (using pointers) are allocated in this segment
- Heap size is normally much larger compared to stack size
- size (actual) grows and shrinks based on program needs
- programmers repsponsibility to deallocate dynamic memory
    - memory leak could occur, otherwise!
    

### Stack segment
- also called scratch pad
- function parameters, local variables, and other function-related register variables are stored
- size automatically grows and shrinks based on program needs
    


Programmers primarily focus on the heap and the stack, as that is where most of the interesting stuff takes place.

## Demo program
- let's compile and see memory addresses of demos/memory_segments.cpp file


```python
# see the memory_segments.cpp source file
! cat ../demos/memory_segments.cpp
```

    #include <cstdio>
    #include <cstdlib>
    #include <iostream>
    
    using namespace std;
    
    int global_var;
    int global_initialized_var = 5;
    char global_string[] = "John Smith!";
    
    void function() {
       int stack_var = 100; 
       // notice this variable has the same name as the one in main()
       static int static_initialized_var = 5;
       static_initialized_var += 10;
       printf("%s\n", "Output inside function:");
       printf("static_initialized_var = %d\n", static_initialized_var);
       printf("static_initialized_var is at address %p\n", &static_initialized_var);
       printf("stack_var = %d\n", stack_var);
       printf("stack_var is at address %p\n", &stack_var);
    }
    
    
    int main() {
       int stack_var; // same name as the variable in function()
       static int static_var;
       int * heap_var_ptr;
       static int static_initialized_var = 5;
    
       heap_var_ptr = (int *) malloc(4);
       printf("%s\n", "output from main function");
       // these addressse are in the code/text segment
       printf("main is at address: %p\n", main);
       printf("main is at address: %p\n", &main);
       printf("function is at address: %p\n", function);
    
       // These variables are in the data segment
       printf("global_initialized_var is at address %p\n", &global_initialized_var);
       printf("static_initialized_var is at address %p\n\n", &static_initialized_var);
    
       // These variables are in the bss segment
       printf("static_var is at address %p\n", &static_var);
       printf("global_var is at address %p\n\n", &global_var);
    
       // This variable is in the heap segment
       printf("heap_var is at address %p\n\n", heap_var_ptr);
    
       // These variables are in the stack segment
       printf("stack_var is at address %p\n", &stack_var);
    
       printf("\ncalling function()...\n");   
       function();
    
       // call function again
       printf("\ncalling function() again...\n");
       // what do you think is the values of static variable in function()?
       function();
       return 0;
    }  



```python
# let's compile and run the program
! g++ -m32 -o memory_segments.exe ../demos/memory_segments.cpp
```


```python
! ./memory_segments.exe
```

    output from main function
    main is at address: 0x659e232b
    main is at address: 0x659e232b
    function is at address: 0x659e226d
    global_initialized_var is at address 0x659e5008
    static_initialized_var is at address 0x659e501c
    
    static_var is at address 0x659e502c
    global_var is at address 0x659e5024
    
    heap_var is at address 0x677dbbb0
    
    stack_var is at address 0xffd11884
    
    calling function()...
    Output inside function:
    static_initialized_var = 15
    static_initialized_var is at address 0x659e5018
    stack_var = 100
    stack_var is at address 0xffd11868
    
    calling function() again...
    Output inside function:
    static_initialized_var = 25
    static_initialized_var is at address 0x659e5018
    stack_var = 100
    stack_var is at address 0xffd11868



```python

```
