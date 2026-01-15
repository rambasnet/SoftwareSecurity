# Pointers and Applications
http://www.cplusplus.com/doc/tutorial/pointers/

## Computer Memory (RAM)
- the primary memory of computer is also called RAM (Random Access Memory)
- program must be loaded into RAM before it can be executed 
- data must be loaded into RAM before program can use it
- literal values or variables are all stored in memory
    - literaly values do not have identifiers associated with them
- variables are programmer-controlled identifiers that maps to some memory location (address)
    - program uses memory addresses
    - programmers uses identifiers/variables
- the following figure depicts a simple representation of RAM

![](media/memory-layout.png)

## Pointers
- special variables that can store physical memory addresses of variables and identifiers
- variables represent values and are used interchangebly
- pointers represent memory address and and are used interchangebly
- like any variable, you must declare a pointer before you can use it
- the following figure helps visualize pointer variable

![](media/pointers-in-cpp.jpg)

## Pointer applications
- pointers are powerful features of C/C++ programs
- allows you to directly manipulate memory!
- there are many applications of pointers; some basic applications are demonstrated below

### Address-of operator (&)
- the address of a variable can be obtained by *address-of-operator (&)* infront of a variable name
- `&` is also used in function parameters for pass-by reference


```c++
#include <iostream>
#include <cstdio>

using namespace std;
```


```c++
int num = 100;
```


```c++
cout << "value of num = " << num << endl;
cout << "address of num = " << &num << endl;
```

    value of num = 100
    address of num = 0x1045c4630


### Dereference operator (\*)
- `*` - *(dereference operator)* can be used to read the **value pointed to by** some memory address


```c++
// what is stored at the address of num?
cout << "value pointed to by &num = " << *(&num) << endl;
```

    value pointed to by &num = 100


## Declaring pointers
- pointers can be declared using `*` de-reference/pointer operator
- syntax:
```c++
    type * pointerVarName;
```
### visualize pointers in pythontutor.com: https://goo.gl/zhCr3G


```c++
// declare pointers
int num1; // variable NOT a pointer
int * pNum1; // declare pNum1 of type int or pointer to int
// declare and initialize pointers
float * fltPtr = nullptr; // initialize with nullptr (pointing to no address)
int * somePtr = &num1; // initialize somePtr with the address of num1
```


```c++
pNum1 = &num1; // assiging value to a pointer
*pNum1 = 200; // dereferencing pNum1; assigning value to the location pointed to by pNum1
```




    200




```c++
// access values of variables and pointers
cout << "*pNum1 = " << *pNum1 << endl;
cout << "pNum = " << pNum1 << endl;
cout << "num1 = " << num1 << endl;
cout << "&num1 = " << &num1 << endl;
```

    *pNum1 = 200
    pNum = 0x1045c4a60
    num1 = 200
    &num1 = 0x1045c4a60


## Pointer arithmetic
- you can add or subtract values to or from pointers
    - pointers will simply point to a different memory location!
- one can move the pointer around pointing to various memory locations
    - that can be dangerous from security point of view!


```c++
pNum1 += 10; // add 10 to pNum1 value (address)
```




    @0x7ffee5c388d0




```c++
cout << "pNum1 = " << pNum1;
```

    pNum1 = 0x10e34dfa8


```c++
// now what value is pNum1 pointing to
cout << "*pNum1 = " << *pNum1;
```

    *pNum1 = 0


```c++
// let's subtract 10
pNum1 -= 10;
```


```c++
cout << "pNum1 = " << pNum1 << endl;
cout << "*pNum1 = " << *pNum1;
```

    pNum1 = 0x10e34df80
    *pNum1 = 200

## Invalid pointers and null pointers
- pointers are meant to point to valid addresses, in principle
- pointers can also point to any any address, including addresses that do not refer to any valid element
    - e.g., uninitialized pointers and pointers to non-existent elements of an array
- neither p nor q point to addresses known to contain a value in the following cell
- they do not cause error while declaring...
- but can cause error/problem if dereferenced such pointers
    - may crash program or point to a random data in memory


```c++
// invalid pointers
int * p; // uninitialized pointer
int some_num; // uninitialized variable
```


```c++
p = (&some_num); // add 10 to address of some_num
```




    @0x7ffee5c388d0




```c++
cout << *p << endl;
```

    0



```c++
cout << * << endl;
```

    0


## Dynamic memory
- memory needs from auto/local variables are determined during compile time before program executes
- at times memory needs of a program can only be determined during the runtime
    - e.g., when amount and type of memory needed depends on user input
- in such cases, program needs to dynamically allocate memory
- pointers are used along with other keywords **new** and **delete** to allocate and deallocate dynamic memory
- dynamic memory is allocated in **heap** segment
    - unlike regular auto variables that are declared on **stack**
- dynamic memory must be deallocated to prevent memory leak in the program
- syntax to allocate and deallocate dynamic memory:

```c++
    // allocate memory
    type * pointer = new type;

    // deallocate memory
    delete pointer;
```
- visualize in pythontutor.com: https://goo.gl/5qse7L


```c++
// allocate dynamic memory
int * numb1 = new int;
int * numb2 = new int;
```


```c++
// use dynamic memory
*numb1 = 100;
*numb2 = 50;
cout << *numb1 << " + " << *numb2 << " = " << *numb1 + *numb2 << endl;
cout << *numb1 << " - " << *numb2 << " = " << *numb1 - *numb2 << endl;
cout << *numb1 << " * " << *numb2 << " = " << *numb1 * *numb2 << endl;
```

    100 + 50 = 150
    100 - 50 = 50
    100 * 50 = 5000



```c++
// delete dynamic memory
// intialize them to nullptr just incase garbage collector has not deallocated numb1 and numb2 yet!
numb1 = nullptr;
numb2 = nullptr;
delete numb1;
delete numb2;
```

## Void pointers and pointer types
- pointers do have types
- the type of pointer is the data type of the memory address it's pointing to


```c++
char charArray[5] = {'a', 'b', 'c', '1', '2'};
```


```c++
// cast the type of charArray to void *
void * voidPtr = (void *)charArray;
```


```c++
// use voidPtr to traverse the charArray
for(int i=0; i<5; i++) {
    printf("[char pointer] points to %p with value '%c'\n", voidPtr, *((char *) voidPtr));
    voidPtr = (void *)((char *) voidPtr + 1);
}
```

    [char pointer] points to 0x7ffff7e45028 with value 'a'
    [char pointer] points to 0x7ffff7e45029 with value 'b'
    [char pointer] points to 0x7ffff7e4502a with value 'c'
    [char pointer] points to 0x7ffff7e4502b with value '1'
    [char pointer] points to 0x7ffff7e4502c with value '2'


## Passing pointers to functions
- pointers can be passed to functions
- similar to passed-by-reference 
    - if value pointed to by formal pointer parameter is changed, the value pointed to by actual pointer parameter will also be changed!
- pass pointers as constants (read-only) to prevent the side effect


```c++
// function that takes two int pointers
int addInts(int * p1, int * p2) {
    return *p1 + *p2;
}
```


```c++
// example 1: pass address of regular variables
int n1, n2 = 0;
```


```c++
n1 = 10; n2 = 15;
cout << n1 << " + " << n2 << " = " << addInts(&n1, &n2) << endl;
```

    10 + 15 = 25



```c++
// example 2: pass ptr/dynamic variables
int * ptr1 = new int;
int * ptr2 = new int;
```


```c++
*ptr1 = 100;
*ptr2 = 200;
cout << *ptr1 << " + " << *ptr2 << " = " << addInts(ptr1, ptr2) << endl;
```

    100 + 200 = 300



```c++

```
