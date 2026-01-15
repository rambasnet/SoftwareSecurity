# Functions pointers

- pointers can store addresses of functions as well; called function pointers
- used for passing a function as an argument to another higher order function
- declaring function pointer is very similar to declaring functions
- parenthesis around function pointer name is required!
- syntax:
```c++
    type (* functionPtrName) ( parameter list... );
```


```c++
#include <iostream>
#include <cstdio>

using namespace std;
```


```c++
// function that takes two integers and returns the sum
int addition (int a, int b) { 
    return (a + b); 
}
```


```c++
int subtraction (int a, int b) {
    return (a - b);
}
```


```c++
int multiplication(int a, int b) {
    return a*b;
}
```


```c++
int operation (int x, int y, int (*func)(int, int)) {
  int g;
  g = (*func)(x, y); // dereferece func
  return g;
}
```


```c++
// declare a function pointer func
int (* func)(int, int);
```


```c++
func = addition
```




    @0x7ffee65fc140




```c++
cout << "10 + 20 = " << func(10, 20);
```

    10 + 20 = 30


```c++
func = subtraction
```




    @0x7ffee65fc140




```c++
cout << "10 - 20 = " << func(10, 20);
```

    10 - 20 = -10


```c++
func = multiplication
```




    @0x7ffee65fc140




```c++
cout << "10 * 20 = " << func(10, 20);
```

    10 * 20 = 200


```c++
// array of function pointer
int (* functions[])(int, int) = {addition, subtraction, multiplication};
```


```c++
cout << "10 + 20 = " << functions[0](10, 20);
```

    10 + 20 = 30


```c++
cout << "10 - 20 = " << functions[1](10, 20);
```

    10 - 20 = -10


```c++
cout << "10 * 20 = " << functions[2](10, 20);
```

    10 * 20 = 200


```c++
int x, y;
```


```c++
x = 10; y = 20;
```


```c++
// different ways to invoke functions
for (int i=0; i<3; i++) {
    printf("address of function %d = %p\n", i, functions[i]);
    printf("function %d result = %d\n", i, (*(functions+i))(x, y));
    printf("function %d result = %d\n", i, functions[i](x, y));
}
```

    address of function 0 = 0x109f750c0
    function 0 result = 30
    function 0 result = 30
    address of function 1 = 0x109fa50b0
    function 1 result = -10
    function 1 result = -10
    address of function 2 = 0x109fc30b0
    function 2 result = 200
    function 2 result = 200



```c++
// let's print the addres
printf("address of addition = %p\n", addition);
printf("address of subtraction = %p\n", &subtraction);
printf("address of multiplication = %p\n", &multiplication);
```

    address of addition = 0x109f750c0
    address of subtraction = 0x109fa50b0
    address of multiplication = 0x109fc30b0



```c++

```
