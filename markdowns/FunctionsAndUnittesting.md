# User Defined Functions and Unit testing

## Adding new functions

- we've used a several functions provided in various standard libraries
- **main( )** is a function that is required for any C++ program
    - indicates where the execution of the program begins
- one can add as many functions as required in the program
- function is a block of code that execute as a group
- the ultimate goal of the function is to break the problem into smaller sub-problems
- if you have many tasks/steps in algoritm steps, function can be used to solve each task/step
- using function is a two-step process
    1. define a function
    2. call or use the function

### Why functions?

- dividing a program into functions or sub-programs have several advantages
    - each sub-problem can be solved using a function
    - makes it easier to design a solution to a big problem
    - makes the solution modular
    - helps reuse the code
        - function once defined can be used many times!
    - makes the solution concise
    - helps test and update a part of program without affecting the rest
    - makes it easier to work in a team where each member can focus on a function


```c++
#include <iostream>
using namespace std;
```


```c++
//1. define a function that prints Hello World!
void sayHello() {
    cout << "Hello World!" << endl;
}
```


```c++
// 2. call sayHello
sayHello();
```

    Hello World!



```c++
// define a function that prints Hello World! three times
// caller function
void sayHelloThrice() {
    // caller calls other functions
    sayHello(); // callee
    sayHello(); // callee
    cout << "Hello World!" << endl;
}
```


```c++
// call sayHelloThrice
sayHelloThrice();
```

    Hello World!
    Hello World!
    Hello World!



```c++
// define a function that greets a person by their name
#include <string>

// name is the only parameter of type string
void greeting(string name) {
    cout << "Hello there, " << name << endl;
}
```


```c++
// wrong way... calling greeting without argument will generate error!
greeting();
```

    [1minput_line_16:3:1: [0m[0;1;31merror: [0m[1mno matching function for call to 'greeting'[0m
    greeting();
    [0;1;32m^~~~~~~~
    [0m[1minput_line_15:2:6: [0m[0;1;30mnote: [0mcandidate function not viable: requires single argument 'name', but no arguments were provided[0m
    void greeting(string name) {
    [0;1;32m     ^
    [0m


    Interpreter Error: 



```c++
// right way: calling greeting with one string argument
greeting("John"); // passing literal value
```

    Hello there, John



```c++
// wrong way to call greeting passing wrong type of argument
greeting(123);
```

    [1minput_line_18:3:1: [0m[0;1;31merror: [0m[1mno matching function for call to 'greeting'[0m
    greeting(123);
    [0;1;32m^~~~~~~~
    [0m[1minput_line_15:2:6: [0m[0;1;30mnote: [0mcandidate function not viable: no known conversion from 'int' to 'std::__cxx11::string'
          (aka 'basic_string<char>') for 1st argument[0m
    void greeting(string name) {
    [0;1;32m     ^
    [0m


    Interpreter Error: 


## Types of functions
- functions can be roughly divided into two types:
    1. void functions or fruitless functions
        - functions that do not return any value
        - all the functions defined previously in this notebook are void functions
        - NOTE: printing result/value is NOT the same as returning value
    2. type functions or fruitful functions
        - functions that return some value
- syntax of fruitful functions

```cpp
    type NAME(type PARAMETER1, ...) {
        // body
        return someValue;
    }
```

- type of the return value must match the type of the function NAME
- fruitful parameterized functions are the most useful ones
    - can use the returned value however you want!
    - can automatically test the results from the functions
    - most library functions are fruitful and parameterized


```c++
// define a function that takes two numbers and returns their sum
long find_sum(int num1, int num2) {
    long total = num1 + num2;
    return total;
}
```


```c++
// call function with values
find_sum(12, 8);
// where is the returned value or result?
```




    20



## Passing data/value or reference
- C++ provides two ways to pass data to functions:
    1. pass by value
    2. pass by reference
    
### pass by value
 - data of argument is copied into parameter
 - default way the data is passed as we've seen above examples
 - easier to understand; no side effect
 - slower to copy large amount of data
 - since the data is copied, anything done to the data via parameter doesn't affect the actual argument
     - if the formal parameter is modified, the actual parameter or argument is not modified!


```c++
// function to demonstrate pass by value
// num1 and num2 are also called formal parameters
long another_sum(int num1, int num2) {
  num1 += 10; // we don't do this, but only to demonstrate pass by value
  num2 += 20;
  long total = num1 + num2;
  return total;
}
```


```c++
// pass data stored in variables; declare variables
int n1, n2;
long answer;
```


```c++
n1 = 20;
n2 = 30;
// n1 and n2 are actual parameters or arguments
answer = another_sum(n1, n2);
cout << n1 << " + " << n2 << " = " << answer << endl;
// gives wrong answer because another_sum() is not correctly implemented
// at least values of n1 and n2 remain intact, because of the way pass by value works!
```

    20 + 30 = 80





    @0x7ffff7fc7b60



### visualize pass by value on [pythontutor.com](http://pythontutor.com/cpp.html#code=//%20pass%20by%20value%20demo%0A%23include%20%3Ciostream%3E%0Ausing%20namespace%20std%3B%0A%0Along%20sum%28int%20num1,%20int%20num2%29%20%7B%0A%20%20num1%20%2B%3D%2010%3B%0A%20%20num2%20%2B%3D%2020%3B%0A%20%20long%20total%20%3D%20num1%20%2B%20num2%3B%0A%20%20return%20total%3B%0A%7D%0A%0Aint%20main%28%29%20%7B%0A%20%20int%20n1,%20n2%3B%0A%20%20n1%20%3D%2020%3B%0A%20%20n2%20%3D%2030%3B%0A%20%20cout%20%3C%3C%20n1%20%3C%3C%20%22%20%2B%20%22%20%3C%3C%20n2%20%3C%3C%20%22%20%3D%20%22%20%3C%3C%20sum%28n1,%20n2%29%20%3C%3C%20endl%3B%0A%20%20//%20n1%20and%20n2%20still%20have%20same%20original%20values%0A%20%20return%200%3B%0A%7D&curInstr=0&mode=display&origin=opt-frontend.js&py=cpp&rawInputLstJSON=%5B%5D)

### pass by reference
- copying data is expensive/slow operation in terms of memory usage and CPU time
    - avoid copying data with **pass by reference** technique
- pass by reference occurs when the parameter has **&** symbol in-between the type and name
- syntax for pass by reference:

```cpp
    // function definition
    type NAME(type & PARAMETER1, type & PARAMETER2,...) {
        // body
    }
    // function call
    NAME(argument1, argument2, ...);
```

- data is not copied but the refernce (memory address) is passed to function
    - meaning actual and formal parameter reference the same memory location
- ONLY variable arguments (NOT literal) can be passed to the reference parameters
- if the formal parameter is modified, argument or actual parameter is also modified!
- efficient, but may have unintended side effect
- provides a way to retrieve data from function!


```c++
// function to demonstrate pass by reference
// num1 and num2 are also called formal parameters that are passed by reference
long yet_another_sum(int & num1, int & num2) {
  long total = num1 + num2;
  return total;
}
```


```c++
// can't pass literals for pass by reference parameters
yet_another_sum(20, 30)
```

    [1minput_line_29:3:1: [0m[0;1;31merror: [0m[1mno matching function for call to 'yet_another_sum'[0m
    yet_another_sum(20, 30)
    [0;1;32m^~~~~~~~~~~~~~~
    [0m[1minput_line_28:3:6: [0m[0;1;30mnote: [0mcandidate function not viable: expects an l-value for 1st argument[0m
    long yet_another_sum(int & num1, int & num2) {
    [0;1;32m     ^
    [0m


    Interpreter Error: 



```c++
// n1 and n2 are already declared as integers above
n1 = 20;
n2 = 30;
// n1 and n2 are actual parameters or arguments
answer = yet_another_sum(n1, n2);
cout << n1 << " + " << n2 << " = " << answer << endl;
// gives right answer n1 and n2 values are modified
```

    20 + 30 = 50





    @0x7ffff7fc7b60




```c++
// swap function 
// swaps/exchanges the values of two variables
void intSwap(int & n1, int & n2) {
    int temp = n1;
    n1 = n2;
    n2 = temp;
}
```


```c++
// let's swap the values of these two variables
int value1 = 10;
int value2 = 1000;
```


```c++
intSwap(value1, value2);
```


```c++
cout << value1 << " " << value2 << endl;
```

    1000 10


### visualize pass by reference on [pythontutor.com](http://pythontutor.com/cpp.html#code=void%20intSwap%28int%20%26%20n1,%20int%20%26%20n2%29%20%7B%0A%20%20%20%20int%20temp%20%3D%20n1%3B%0A%20%20%20%20n1%20%3D%20n2%3B%0A%20%20%20%20n2%20%3D%20temp%3B%0A%7D%0Aint%20main%28%29%20%7B%0A%20%20//%20let's%20swap%20the%20values%20of%20these%20two%20variables%0A%20%20int%20value1%20%3D%2010%3B%0A%20%20int%20value2%20%3D%201000%3B%0A%20%20intSwap%28value1,%20value2%29%3B%0A%20%20return%200%3B%0A%7D&curInstr=0&mode=display&origin=opt-frontend.js&py=cpp&rawInputLstJSON=%5B%5D)


### retrieve data from void function
- pass by reference can be used to retrieve one or more values/answers from functions


```c++
// extracting value from a function with passed by reference technique
// num1 is passed by value num2 and sum are passed by reference
void computeSum(int num1, int& num2, long &sum) {
    sum = num1 + num2; // sum is modified
    // notice void function, no return value!
}
```


```c++
n1 = 100;
n2 = 300;
answer = 0; // initilize ans to 0; just in case!
// n1, n2, and ans are actual parameters or arguments
computeSum(n1, n2, answer);
cout << n1 << " + " << n2 << " = " << answer << endl;
```

    100 + 300 = 400



```c++
// define a function that computes and returns both area and perimeter
// C++ doesn't allow returning multiple values from a function
// use pass-by-reference technique!
void area_and_perimeter(const float & l, const float & w, float & area, float & peri) {
    area = l*w;
    peri = 2*(l+w);
}
```


```c++
float len = 4.5;
float width = 3.99;
float area, perimeter;
```


```c++
area_and_perimeter(len, width, area, perimeter);
```


```c++
area
```




    17.9550f




```c++
perimeter
```




    16.9800f



### visualize retrieving data from function on [pythontutor.com](http://pythontutor.com/cpp.html#code=//%20pass%20by%20value%20demo%0A%23include%20%3Ciostream%3E%0Ausing%20namespace%20std%3B%0A%0A//%20extracting%20value%20from%20a%20function%20with%20passed%20by%20reference%20technique%0A//%20num1%20is%20passed%20by%20value%20num%202%20and%20sum%20are%20passed%20by%20reference%0Avoid%20computeSum%28int%20num1,%20int%26%20num2,%20long%20%26sum%29%20%7B%0A%20%20%20%20sum%20%3D%20num1%20%2B%20num2%3B%20//%20sum%20is%20modified%0A%20%20%20%20//%20notice%20void%20function,%20no%20return%20value!%0A%7D%0A%0Aint%20main%28%29%20%7B%0A%20%20int%20n1,%20n2%3B%0A%20%20n1%20%3D%2020%3B%0A%20%20n2%20%3D%2030%3B%0A%20%20long%20ans%20%3D%200%3B%0A%20%20computeSum%28n1,%20n2,%20ans%29%3B%0A%20%20cout%20%3C%3C%20n1%20%3C%3C%20%22%20%2B%20%22%20%3C%3C%20n2%20%3C%3C%20%22%20%3D%20%22%20%3C%3C%20ans%20%3C%3C%20endl%3B%0A%20%20return%200%3B%0A%7D&curInstr=0&mode=display&origin=opt-frontend.js&py=cpp&rawInputLstJSON=%5B%5D)

## Debugging and Unittesting with assert( )
- if a problem is broken into sub-problems, it can be easily debugged
- functions can be used to solve sub-problems that can be designed and tested independently
- testing code at the functional level is called **unittesting**
- we'll learn the basics of unittesting by automatically testing functions
- $3^{rd}$ party frameworks such as googletest is used for comprehensive unittesting
- we use **assert()** macro function provided in **&lt;cassert&gt;** library
- syntax to use assert

```cpp
    assert(boolean expression);
```
- boolean expression is created comparing two data values using e.g., **==** operator
    - more on comparison operators covered in Conditionals chapter
- if condition evaluates to true (two values are indeed equal), assertion passes
    - otherwise, assertion fails and the program halts immidiately
- more on assert: https://en.cppreference.com/w/cpp/error/assert
- **NOTE: assert( ) doesn't work in Jupyter notebook**; see the following demo programs

### makefile demos
- make is a program that allows to compile and build complex C/C++ programs
- see the following demos

### debugging demo
- debugging with assert function [demos/assert/assertdebug.cpp](demos/assert/assertdebug.cpp)

```bash
cd ./demos/assertassertdebug.cpp
make
# runt the program
make clean
```


### unittesting demos
- using this concept, we can automatically test if the returned results from functions are correct or not
- see unittesting function with assert here [demos/unittest1/unittesting.cpp](demos/unittest1/unittesting.cpp)

- see improved version 2.0 unittesting example [demos/unittest2/unittesting_v2.cpp](demos/unittest2/unittesting_v2.cpp)

## Scope and name resolution
- scope is the area of visibility of identifiers such as variables and constants within the program
    - scope determines where the variables and identifiers are valid
- scopes can be conceptualized as box frames
- C++ provides 3 common types of identifiers' scopes
    1. global scope
    2. local scope
    3. block scope
    
### global scope
- any identifiers (variables, constants, functions) decalared outside any function have global scope
- most functions have global scopes
- can be used anywhere in the program

### local scope
- any variables and constants defined inside function scope
    - including parameters have local scope
- local identifiers can be used anywhere within the function scope

### block scope
- smallest region that is enclosed inside curly braces { }
- any identifiers declared inside curly braces can be used only withing that block
- very limited scope and rarely used

### name resolution
- program needs to know and resolve what different names are
    - where the names are declared and their scope or visibility
- compiler first looks into the current scope or frame then the frame it's immidiately inside and so on until global frame to try to resolve any name used in the program

- the following figure depicts the various scopes and name resolution in C++

![](resources/scopesandnameresolution.png)



```c++

```
