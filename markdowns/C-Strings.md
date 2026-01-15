# C Strings

- also called buffer that can store range of bytes/characters (string)
- C programming language doesn't provide a type to work string data
- C++ provies `<string>` class library that makes it easier to work with string data
- this is a quick review of C-string and the library functions to deal with the string data
- c-string requires array of **char** to store string
- `cin` and other operations on c-strings are not easier without knowing array and pointers well

## C++ string objects
- std::string is a std::basic_string&lt;char&gt; template type defined in **string** header
- more: https://en.cppreference.com/w/cpp/string/basic_string
- string is an advanced type of container with many member variables and member functions
    - variables of advanced type are called objects
    - member functions are called methods
    - one can define any type using **struct** or **class** that we'll learn later


```c++
#include <string> // C++ string
#include <cstdio>
#include <iostream>

using namespace std;

// declare a string variable/object
string first;
```


```c++
// assing string value to string object
first = "Hello, ";
```


```c++
// declare and initialize string object
string second = "World";
```


```c++
// std output C++ string
cout << first << ' ' << second << '!' << endl;
```

    Hello,  World!



```c++
// std input C++ string
cout << "Enter your first name: ";
cin >> first;
```

    Enter your first name: John



```c++
// converting c++string to c_string for printf function
printf("Hello, %s!", first.c_str());
```

    Hello, John!

## C-string

- C language doesn't have a type defiend to work with string like in C++
- now that we understand pointer and C-array, let's revisit C-string
- C-string is array of characters that ends with a NULL character '\0' (ASCII 0)


```c++
// declare and initialization is easier
// null character is automatically added at the end!
char name[] = "John Smith";
```


```c++
// once declared; working with C-string is pain
// work one character at a time!
char f_name[10];
```


```c++
f_name[0] = 'J';
f_name[1] = 'a';
f_name[2] = 'k';
f_name[3] = 'e'; 
f_name[4] = '\0';
```


```c++
// C-string must end with null-character '\0'
cout << f_name;
```

    Jake


```c++
printf("Hello, %s\n", f_name);
```

    Hello, Jake


## Library Functions to work with C-string
- http://www.cplusplus.com/reference/cstring/
- strcpy, strcat, strlen - are commonly used operations on c-strings

## Array of char *
- array of C-string (char *)
- similar to array of C++ string conceptually; harder to work with however!
- must use as a parameter for **main(int argc, char* argv[])**



```c++
// create array of char * that stores addresses of 4 c-strings
char * stuff[4];
```


```c++
char val1[20] = "ball";
```


```c++
char val2[20] = "test";
```


```c++
stuff[0] = val1; // copy address of val1 to stuff[0]
```




    "ball"




```c++
stuff[1] = val2; // cop address of val2 to stuff[1]
```




    "test"



### passing array of char * to function


```c++
// write a function similar to main
int my_main(int argc, char* argv[]) {
    cout << "argc = " << argc << endl;
    for(int i=0; i< argc; i++) {
        printf("argv[%d] = '%s'\n", i, argv[i]);
        if (string(argv[i]) == "test")
            cout << "test is found in argv[]\n";
    }
    return 0;
}
```


```c++
my_main(2, stuff);
```

    argc = 2
    argv[0] = 'ball'
    argv[1] = 'test'
     test is found in argv[]



```c++

```
