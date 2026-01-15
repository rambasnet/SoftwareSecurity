# Basic Programming concepts & C++ fundamentals
- Note: use your installed C++ compiler or online compilers in case C++ kernel crashes on this notebook or doesn't work in some cases:
    - [https://repl.it/](https://repl.it/)
    - [https://coliru.stacked-crooked.com/](https://coliru.stacked-crooked.com/)
    - [http://cpp.sh](http://cpp.sh/)
    

## Table of Contents

- [Fundamental concepts](#concepts)
- [Fundamental types](#types)
- [Variables](#variables)
- [Type casting](#casting)
- [Input/Output](#io)
- [Output formatting](#format)
- [Functions](#functions)
- [Unit test](#unittest)
- [String data](#string)
- [Escape sequences](#escape)
- [Operators](#operators)
    - [Unary operators](#unary)
    - [Binary operators](#binary)
    - [Bitwise operators](#bitwise)
    - [Ternary operator](#ternary)
    - [Other operators](#other)
- [Math functions](#math)
- [Conditionals](#conditionals)
    - [One-way selector](#oneway)
    - [Two-way selector](#twoway)
    - [Multi-way selector](#multiway)
    - [Switch statement](#switch)
- [Loops](#loop)
    - [for loop](#for)
    - [range-based for loop](#range)
    - [while loop](#while)
    - [do while loop](#dowhile)
- [Arrays](#arrays)
    - [2D Arrays](#2darrays)

<a id="concepts"></a>

## Fundamental concepts/building blocks
 - Data types and variables
 - Input/Output
 - Math operations
 - Decision/Conditionals
 - Loops

## Headers and helper functions
- run include headers and helper function cells right below if Kernel crashes or is restarted


```c++
// headers and namespace required in this notebook demo codes
#include <iostream> //cin, cout
#include <cstdio> //printf
#include <cstring> // funciton to work with cstring
#include <string> // string functions
#include <fstream> // file io
#include <iomanip> // output formatting

using namespace std;
```

<a id="types"></a>

## fundamental data types
https://en.cppreference.com/w/cpp/language/types
- void : type with an empty set of values
- bool : true or false
- int : integer/whole number
    - signed int : signed representation (default)
    - unsigned int : unsigned representation
- short : target type will have width of atleast 16 bits
- long : width of at least 32 bits
- long long : width of at least 64 bits
- size_t : unsigned int type
- int32_t : signed int 32
- int64_t : signed int 64
- char : signed char representation
- float : single precision float (32 bit)
- double : double precision (64 bit)
- long double: extended precision floating point type

- No fundadamental type available to work with string data (array of characters or buffer)

<a id='variables'></a>

## variables
- identifier or named memory location that allows us to store data
- syntax to declare a variable: 
```c++
type varName;
```
- know the rules of naming identifiers


```c++
// declaring variables
int x, y, z;
string buffer;
float test1, test2, test3;
```


```c++
// variable declaration and initialization
// = assignment operator;
bool a = true;
char b = 'Z';
short c = 100;
int d = -2000000000;
unsigned int dd = 23232; // must be positive value only!
long e = 2000000000;
long long f = 123456789;
size_t g = 111;
int64_t h = 2345;
float i = 123.1234567;
double j = 1234.123456789;
long double k = 12112.1212121211121;
string l = "some string";
```


```c++
cout << "sizeof(bool) = " << 8*sizeof(bool) << " bits." << endl;
// printf("sizeof(b) = %lu\n", sizeof(b)*8); doesn't work!
cout << "sizeof(b) = " << 8*sizeof(b) << " bits." << endl;
cout << "sizeof(short) = " << 8*sizeof(short) << " bits." << endl;
cout << "sizeof(int) = " << 8*sizeof(int) << " bits." << endl;
cout << "sizeof(unsigned int) = " << 8*sizeof(unsigned int) << " bits." << endl;
cout << "sizeof(long) = " << 8*sizeof(long) << " bits." << endl;
cout << "sizeof(long long) = " << 8*sizeof(long long) << " bits." << endl;
cout << "sizeof(size_t) = " << 8*sizeof(size_t) << " bits." << endl;
cout << "sizeof(int32_t) = " << 8*sizeof(int32_t) << " bits." << endl;
cout << "sizeof(int64_t) = " << 8*sizeof(int64_t) << " bits." << endl;
cout << "sizeof(float) = " << 8*sizeof(float) << " bits." << endl;
cout << "sizeof(double) = " << 8*sizeof(double) << " bits." << endl;
cout << "sizeof(long double) = " << 8*sizeof(long double) << " bits." << endl;
cout << "sizeof(string) = " << 8*sizeof(string) << " bits." << endl;
```

    sizeof(bool) = 8 bits.
    sizeof(b) = 8 bits.
    sizeof(short) = 16 bits.
    sizeof(int) = 32 bits.
    sizeof(unsigned int) = 32 bits.
    sizeof(long) = 64 bits.
    sizeof(long long) = 64 bits.
    sizeof(size_t) = 64 bits.
    sizeof(int32_t) = 32 bits.
    sizeof(int64_t) = 64 bits.
    sizeof(float) = 32 bits.
    sizeof(double) = 64 bits.
    sizeof(long double) = 128 bits.
    sizeof(string) = 192 bits.


<a id='casting'></a>

## type casting
https://en.cppreference.com/w/cpp/string/basic_string
- converting one type into another if possible
- stoi(), stol(), stoll() : converts a string to a signed int
- stoul(), stoull() : converts a string to unsigned int
- stof(), stod(), stold() : converts a string to float
- to_string() : converts an int or float to string


```c++
int id = stoi("111");
float PI = stof("3.1416");
string strNum = to_string(1000.99);
```


```c++
int num = stoi("1234");
cout << num+10;
```

    1244


```c++
float price = stof("10.99");
cout << price*10;
```

    109.9


```c++
string strNum = to_string(100.99);
cout << strNum + "99" << endl;
```

    100.99000099


<a id='io'></a>

## input/output
- standard input/output
    - iostream: cin, cout
        - getline - read the whole line (until \n) into a string
    - cstdio: printf, scanf
    
- file input/output
    - fstream, ifstream, ofstream
    - steps working with files:
        1. declare file handlers
        2. open file
        3. check if file opened successfully
        4. read from or write to file
        5. close file


```c++
int fileio() {
    ifstream fin;
    string line;
    fin.open("README.md");
    if (!fin)
        cout << "file couldn't be opened!" << endl;
    else {
        while(!fin.eof()) {
            getline(fin, line);
            cout << line << endl;
        }
    }
    return 0;
}
```


```c++
fileio();
```

    # System and Software Security and Exploitation
    
    - Fundamentals of System and C++ Software Security and Exploitation
    - Hacking Tricks, Techniques & Tools
    - Primary Tools: C++, Bash, Python3
    - CTF
    
    # Requirements
    
    - Ubuntu/Debian Linux (Kali Linux 64-bit Preferred)
      - add account kali:kali with sudo access
    - gcc/g++
    - Jupyter Notebook
    - Python3
    - gdb
    - peda - python exploit development assistant for gdb
    - sqlite3 C/C++ library
    - sqlitebrowser
    - pwntools
    - boost C++ libraries
    
    # Install Required Tools
    - update Linux and install tools
    - install Python 3.x Miniconda for Linux: https://conda.io/en/latest/miniconda.html
    - run the following commands on a Terminal
    
    ```bash
    sudo apt update
    sudo apt upgrade
    sudo apt install build-essential # C/C++ compiler, make, etc.
    sudo apt install ccache
    sudo apt install libboost-all-dev
    sudo apt install gcc-multilib g++-multilib # library for compiling 32-bit
    sudo apt install gdb
    sudo apt install gdbserver
    sudo apt install git
    sudo apt install libsqlite3-dev # C++ library for Sqlite
    sudo apt install sqlitebrowser
    curl -o Miniconda.sh https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
    bash Miniconda.sh # pick defaults; python 3 is installed!
    conda update conda
    conda install jupyter notebook # jupyter notebook
    conda install -c conda-forge xeus-cling # C++ kernel
    conda install -c conda-forge retrolab # replate classic notebook with retro style
    pip install ptpython # better python REPL for terminal
    ```
    
    # Play with Notebooks
    
    - clone/download this repository
    - compile and run C++ programs found in `demos` folder
    - using a terminal `cd` into the repository folder and run `jupyter notebook`
    
    ```bash
    cd <EthicalHacking_Repo>
    jupyter notebook
    ```
    
    - copy and paste the link shown on a browser if it doesn't open on its own
    - open 00-TableOfContents.ipynb notebook that provides table of contents with link to all the notebooks
    


<a href="format"></a>

## output formatting
- iomanip - https://en.cppreference.com/w/cpp/header/iomanip
- hex, oct, fixed and scientific formats to display float values
- showpoints


```c++
cout << fixed << setprecision(2) << 19.19999 << endl;
cout << hex << showbase << "hex 16 = " << 16 << " oct 8 = " << oct << 8 << endl;
cout << setfill('*') << setw(10) << right << "hi " << left << setw(15) << "there!" << endl;

```

    19.20
    hex 16 = 0x10 oct 8 = 010
    *******hi there!*********


<a id='functions'></a>

## functions
- sub-routine/sub-program/procedure
- burrowed from math/algebra concept: y = f(x) = 2x^2 + 3x +10
- block of code identified by a single identifier
- two steps: 1. define function 2. call function
- defination syntax:

```c++
type functionName(type1 para1, type2& para2, ...) {
    /* block of code */
    return;
}
```
- call syntax:
```c++
functionName(arg1, arg2, ...);
```
- helps break problems into sub-problems
- helps code reuse and code abstraction (hiding implementation details)
- function can call many other functions
- two ways to pass data to a function: by value and by reference
- fruitful function can return answer/value from function
    - fruitful functions can be automatically tested
- void functions do not return a value; values printed are usually manually tested


```c++
int add(int a, int b) {
    return a+b;
}
```


```c++
int num1 = 100;
int num2 = -50;
cout << add(num1, num2) << endl;
```

    062


<a id='unittest'></a>

## unittest
https://en.cppreference.com/w/cpp/error/assert
- automatic testing of functions
- use assert function defined in assert.h or cassert header file


```c++
#include <cassert>
//#include <assert.h>
assert(add(99, 1) == 100);
```


```c++
assert(add(100, 200) == 400); // this should give assertion error but doesn't work here...
// try it here: https://coliru.stacked-crooked.com/
```

<a id='string'></a>

## string data
https://en.cppreference.com/w/cpp/string/basic_string
- two ways to work with string:
    1. C string - array of char type 
    2. C++ string - Abstract Data Type (ADT);

<a id="cstring"></a>

### C string
- array of characters 
- must be \0 (null terminated) to prevent from buffer-overrun
- many limitations while manipulating c string
- must learn to mitigate buffer overflow vulnerabiliy
```c++
    char name[size];
```


```c++
#include <cstring>
```


```c++
// declare c-string
char name[20];
```


```c++
strncpy(name, "John\0", 5);
cout << name << endl;
cout << strlen(name) << endl;
```

    John
    04



```c++
// declare and initialize c-string
char word[] = "Hello";
```


```c++
cout << word << endl;
cout << "len of word = " << strlen(word) << endl;
```

    Hello
    len of word = 05


<a id="cppstring"></a>

### C++ string
- string header file
- not fundamental type; but ADT (Abstract Data Type); user-defined type that's part of library
```c++
    string varName;
```


```c++
string phrase;
```


```c++
phrase = "There may be a needle in the stack of stack of haystacks!";
```


```c++
cout << phrase.length() << endl;
cout << phrase[0] << endl;
// cout << phrase.find("needle", 0);
// https://en.cppreference.com/w/cpp/string/basic_string/find
```

    071
    T





    @0x112d26558




```c++
// loop throuch each char at a time
for (auto c: phrase)
    cout << c << " ";
```

    T h e r e   m a y   b e   a   n e e d l e   i n   t h e   s t a c k   o f   s t a c k   o f   h a y s t a c k s ! 


```c++
// declare and initialize
string phrase1 = "Another phrase!"
```

<a href="escape"></a>

## escape sequences
https://en.cppreference.com/w/cpp/language/escape
- used to represent certain special characters within string literals

| character | description
| --- | --- |
| \\ | backslash 
| \\' | single quote
| \\" | double quote
| \\n | new line - line feed
| \\r | carriage return
| \t | horizontal tab
| \\v | vertical tab
| \b | backspace



```c++
char q = '\'';
cout << q;
```

    '


```c++
string sent = "\"Oh no!\", exclaimed Jill. \"Jack broke my bike!\"";
cout << sent;
```

    "Oh no!", exclaimed Jill. "Jack broke my bike!"

<a id="operators"></a>

## Operators
- operators and precedence rule: https://en.cppreference.com/w/cpp/language/operator_precedence

- arithmetic operators: https://en.cppreference.com/w/cpp/language/operator_arithmetic

<a id="unary"></a>

## unary operators

| Operator | Symbol | Syntax | Operation |
|--- | --- | --- | --- |
| positive | + | +100 | positive 100 (default) |
| negative | - | -23.45 | negative 23.45 |

<a id="binary"></a>

## binary operators
- take two operands
- follows PEMDAS rule of precedence

| Operator | Symbol | Syntax | Operation |
|--- | --- | --- | --- |
| add | + | x + y | add the value of y with the value of x
| subtract | - | x - y | subtract y from x |
| multiply | * | x * y | multiply x by y |
| divide | / | x / y | divide x by y (int division if x and y are both ints) |
| modulo | % | x % y | remainder when 

<a id="bitwise"></a>

## binary bitwise operators
- https://www.learncpp.com/cpp-tutorial/38-bitwise-operators/

| Operator | Symbol | Syntax |	Operation |
|------| ------ | ---- | ---- |
|bitwise left shift	| << |	x << y	| all bits in x shifted left y bits; multiplication by 2
|bitwise right shift |	>> | x >> y	| all bits in x shifted right y bits; division by 2
bitwise NOT	| ~	| ~x	| all bits in x flipped
|bitwise AND |	& |	x & y |	each bit in x AND each bit in y
|bitwise OR	| \| |	x \| y	| each bit in x OR each bit in y
bitwise XOR |	^ |	x ^ y |	each bit in x XOR each bit in y


```c++
cout << " bitwise and &" << endl;
cout << (1 & 1) << endl;
cout << (1 & 0) << endl;
cout << (0 & 1) << endl;
cout << (0 & 0) << endl;
```

     bitwise and &
    01
    0
    0
    0



```c++
cout << "bitwise or | " << endl;
cout << (1 | 1) << endl;
cout << (1 | 0) << endl;
cout << (0 | 1) << endl;
cout << (0 | 0) << endl;
```

    bitwise or | 
    01
    01
    01
    0



```c++
cout << "bitwise not ~" << endl;
cout << ~(1|1) << endl;
cout << ~0 << endl;
```

    bitwise not ~
    037777777776
    037777777777



```c++
cout << "bitwise xor ^" << endl;
cout << (1 ^ 1) << endl;
cout << (1 ^ 0) << endl;
cout << (0 ^ 1) << endl;
cout << (0 ^ 0) << endl;
```

    bitwise xor ^
    0
    01
    01
    0



```c++
cout << (1 << 10) << endl; // pow(2, 10)
cout << (1024 >> 10) << endl; //
```

    02000
    01





    @0x112d26558



<a id="ternary"></a>

## Ternary conditional operator (? :)
- syntax:
```c++
(condition) ? TrueValue : FalseValue;
```


```c++
int number1, number2, larger;
```


```c++
number1 = 10;
number2 = 20;
```


```c++
larger = (number1 > number2) ? number1 : number2;
cout << "larger = " << larger << endl;
```

    larger = 024


<a id="other"></a>

## Other operators
- scope resolution operator: ::
    - std::string, std::cin
    - can create your own namespace : http://www.cplusplus.com/doc/tutorial/namespaces/
- increment/decrement (pre and post): ++, --
- compound assignments: +=, -=, *=, /-, %=, <<=, >>=, &=, ^=, |=  

<a id="math"></a>

## Math functions
- cmath library for advanced math operations: https://en.cppreference.com/w/cpp/header/cmath
    - ceil, floor, round, sqrt, pow, abs, log, sin, cos, tan

<a id='conditionals'></a>

## conditionals - control flow
- select a block of code to execute based on some condition
- add logic to the code as if your code is thinking and making a decision
- use boolean expression that evaluates to true or false
- use comparision operators ( == , != , <=, >= ) to compare values/expressions that will provide true or false or (yes or no) result
- use logical operators (&&, ||) to formulate compound logical expression
- three types: 
    1. one-way selector
    2. two-way selector
    3. multi-way selector
- one selector type can be nested inside another!

<a id='oneway'></a>

### one way selector 

```c++
if (expression == true) {
  /* execute code... */   
}
```


```c++
// one way selector
bool execute = false;
if (execute)
    cout << "this block executed!" << endl;

cout << "done!"
```

    done!




    @0x112d26558



<a id='twoway'></a>

### two way selector

```c++
if (expression == true) {
    /* execute this block */
}
else {
    /* otherwise, execute this block */
}
```


```c++
// two way selector
// test if a given number is even or odd
bool isEven(int n) {
    if (n%2 == 0) 
        return true;
    else 
        return false;
}
```


```c++
int someNum;
```


```c++
someNum = 11;
if (isEven(someNum))
    cout << someNum << " is even!";
else
    cout << someNum << " is odd!";
```

    013 is odd!

<a id='multiway'></a>

### multi-way selector

```c++
if (expression1 == true) {
    /* execute this block and continue after else block */
}
else if (expression2 == true) {
    /* execute this block and continue after else block */
}
else if (expression3 == true) {
    /* execute this block and continue after else block*/
}
...
else {
   /* if no condition is evaluated true, by default execute
   this block 
   */
}
```


```c++
int day;
```


```c++
// multiway selector
day = 0;
if (day == 0) {
    cout << "Sunday" << endl;
    cout << "Yay!! it's a weekend!\n";
}
else if (day == 1)
    cout << "Monday";
else if (day == 2)
    cout << "Tuesday";
else if (day == 3)
    cout << "Wednesday";
else if (day == 4)
    cout << "Thursday";
else if (day == 5) {
    cout << "Friday";
    cout << "Almost weekend!";
}
else {
    cout << "Saturday" << endl;
    cout << "Yay!! it's a weekend!\n";
}
cout << "done..." << endl;
```

    Sunday
    Yay!! it's a weekend!
    done...


<a id='switch'></a>

### swtich statment 
https://en.cppreference.com/w/cpp/language/switch
- works on integral data/expression to compare its value with many cases
- similar to multi-way selector are more efficient and adds readability

```c++
switch (expression) {
    case value1:
        //...
        break;
    case value2:
    case value3:
        //...
        break;
    default:
        //...
}
```


```c++
enum Colors
{
    COLOR_BLACK,
    COLOR_WHITE,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE
};
```


```c++
void printColor(Colors color) {
    switch (color) {
        case COLOR_BLACK:
            std::cout << "Black";
            break;
        case COLOR_WHITE:
            std::cout << "White";
            break;
        case COLOR_RED:
            std::cout << "Red";
            break;
        case COLOR_GREEN:
            std::cout << "Green";
            break;
        case COLOR_BLUE:
            std::cout << "Blue";
            break;
        default:
            std::cout << "Unknown";
            break;
    }
}
```


```c++
Colors favColor;
```


```c++
favColor = COLOR_BLACK;
printColor(favColor);
```

    Black

<a id='loop'></a>

## loops  - control flows
- repeatedly execute a block of code over-again with a different result
- be careful of infinite loop!
- `break` and `continue` keywords can be used inside loop
    - `break` : breaks the loop immidiately ignoring all the trailing codes and execution continues after loop body
    - `continue` : continues to the next iteration ignoring all the trailing codes inside loop
- four types of loop structures
   1. for loop
   2. range based for loop
   3. while loop
   4. do while

<a id='for'></a>

### for loop
https://en.cppreference.com/w/cpp/language/for
```c++
for(init; condition; update) {
    // statements
}
```
- order of execution: 
    1. init; only one
    2. condition
    3. statements
    4. update
    5. repeat from step 2


```c++
for (int i=1; i<=20; i++) {
    if (i%2 == 0)
        cout << i << " ";
}
```

    02 04 06 010 012 014 016 020 022 024 

<a id='range'></a>

### range-based for loop
https://en.cppreference.com/w/cpp/language/range-for
```c++
for(range_declaraion: range_expression) {
    // statements
}
```


```c++
for (auto num: {1, 2, 3, 4, 100})
    cout << num << " ";
```

    01 02 03 04 0144 


```c++
for (char ch: "Hello World")
    cout << ch << "*";

```

    H*e*l*l*o* *W*o*r*l*d* *

<a id='while'></a>

### while loop
https://en.cppreference.com/w/cpp/language/while
- Executes statement(s) repeatedly, until the value of condition becomes false. 
- The test takes place before each iteration.

```c++
while (condition) {
    // statements
}
```


```c++
int MAX;;
int i;
```


```c++
MAX = 20;
i = 0;
while (i <= MAX) {
    if (i%2 == 0) 
        cout << i << " ";
    i += 1; // DO NOT FORGET TO UPDATE LOOP VARIABLE TO AVOID INFINITE LOOP!!!
}
```

    0 02 04 06 010 012 014 016 020 022 024 

<a id='dowhile'></a>

### do while loop
https://en.cppreference.com/w/cpp/language/do
- executes statement(s) repeatedly, until the value of condition becomes false. 
- the test takes place after each iteration.

```c++
do {
    // statements
} while (condition);
```


```c++
int MAX_TIMES;
int times;
```


```c++
// DO NOT run do while... crashes kernel
times = 0;
MAX_TIMES = 20;
do {
    cout << times << " ";
    times ++; // DO NOT FORGET TO UPDATE LOOP VARIABLE TO AVOID INFINITE LOOP!!!
    break;
} while(times <= MAX_TIMES);
```

    0 

<a id='arrays'></a>

## arrays
- container that stores 1 or more similar data called elements
- use array when you need to store large number of data values as declaring individual variable for each variable is not desireable or not even possible!
- size of the array has to be known and is fixed
- use 0-based index to access each element stored in array
- array is passed by reference ONLY to a function
- array can't be returned from a function
- aggregate operations such as IO, assignment, comparision are not allowed
- syntax:
```c++
type arrayName[const_size];
```


```c++
int tests[10];
float prices[] = {1, 2, 3, 100.99};
string names[2] = {"John", "James"};
```


```c++
tests
```




    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }




```c++
prices
```




    { 1.00000f, 2.00000f, 3.00000f, 100.990f }




```c++
names
```




    { "John", "James" }




```c++
tests[0] = 100;
tests[9] = 75;
tests
```




    { 100, 0, 0, 0, 0, 0, 0, 0, 0, 75 }




```c++
prices = {2, 3, 4, 5} // once array is declared, it can't be assigned as a whole
```

    [1minput_line_77:2:9: [0m[0;1;31merror: [0m[1marray type 'float [4]' is not assignable[0m
     prices = {2, 3, 4, 5}
    [0;1;32m ~~~~~~ ^
    [0m


    Interpreter Error: 



```c++
cout << prices[10] << endl;
```

    [1minput_line_78:2:10: [0m[0;1;35mwarning: [0m[1marray index 10 is past the end of the array (which contains 4 elements) [-Warray-bounds][0m
     cout << prices[10] << endl;
    [0;1;32m         ^      ~~
    [0m[1minput_line_67:3:1: [0m[0;1;30mnote: [0marray 'prices' declared here[0m
    float prices[] = {1, 2, 3, 100.99};
    [0;1;32m^
    [0m

    0.00


<a id='2darrays'></a>

## 2-D arrays
- row-major 2-d arrays
- syntax:
```c++
type name[rowSize][colSize];
```


```c++
int matrix[4][4] = {{1, 2, 3, 4}, {10, 20, 30, 40}, {100, 200, 300, 400}, {1, 1, 1, 1}};
```


```c++
matrix
```




    { { 1, 2, 3, 4 }, { 10, 20, 30, 40 }, { 100, 200, 300, 400 }, { 1, 1, 1, 1 } }




```c++
matrix[0][0] = matrix[2][0]*matrix[3][0]
```




    100




```c++
matrix
```




    { { 100, 2, 3, 4 }, { 10, 20, 30, 40 }, { 100, 200, 300, 400 }, { 1, 1, 1, 1 } }




```c++
for(int i=0; i<4; i++) {
    cout << "[ ";
    for(int j=0; j< 4; j++) {
        cout << matrix[i][j] << ", ";
    }
    cout << "]" << endl;
}
```

    [ 0144, 02, 03, 04, ]
    [ 012, 024, 036, 050, ]
    [ 0144, 0310, 0454, 0620, ]
    [ 01, 01, 01, 01, ]



```c++

```
