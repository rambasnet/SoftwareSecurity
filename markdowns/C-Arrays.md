# Arrays
- array concept in C++ is burrowed from C

## Array
- range of a particular type of thing
- we've used single variable to store single data/value
- large programs typically deal with a large number of data values that must be stored in memory
    e.g. sorting data values.
- not practicle to declare a large number of variables to store a large number of values
- array is a container used to store a large number of same type values under one name
- array we're learning about in this chapter is C-array
- array in this notebook refers to C-array
- there are two types of arrays
    1. static array
    2. dynamic array
    
## Static array
- the size of the array is determined during compile-time and is fixed
- local static array is stored on stack memory segment
- syntax to declare a static array
```cpp
type arrayName[size];
```
- size tells the compiler how many of similar type of values can be stored in the arrayName
- size must be a positive integer (size_t type) - literal or variable
- the following figure depicts what happens in computer memory when an array of int is declared

![](resources/array.png)

- each member of the array is called an element
- each element has same type and name that only differs by its index
- index -- also called offset -- ranges between 0 to size-1


```c++
#include <iostream>
#include <string>

using namespace std;
```


```c++
// nums array to store 5 integers
int numbers[5];
```

### accessing member elements
- members can be accessed and used ONLY one element per operation
- no aggregate operation is allowed on the array variable as a whole
    - e.g. copy one array into another; printing the whole array, etc.


```c++
// access and store values into each element
numbers[0] = 10;
numbers[1] = 20;
numbers[2] = 30;
numbers[3] = 40;
numbers[4] = 50;
```


```c++
// access some element
cout << numbers[0];
```

    10


```c++
// each element can be used like a single variable
numbers[1] = numbers[2] + numbers[3];
```


```c++
// traverse an array
for(int i=0; i<5; i++) {
    cout << i << " -> " << numbers[i] << endl;
}
```

    0 -> 10
    1 -> 70
    2 -> 30
    3 -> 40
    4 -> 50



```c++
// declaring and initializing an array
// size is optinal; will be determined with the no. of values it's initialzed with
float grades[] = {90.5f, 34.5f, 56, 81, 99, 100, 89.9};
```


```c++
grades
```




    { 90.5000f, 34.5000f, 56.0000f, 81.0000f, 99.0000f, 100.000f, 89.9000f }



## Member functions
- C-array is so primitive that it doesn't come with any useful operations or member functions
- implementing any array operation falls under programmer's responsibility!
- e.g. how can you quickly tell the size or length of an array?


```c++
// finding the size of the array
size_t arr_size = sizeof(grades)/float(sizeof(float));
```


```c++
cout << "array's size or length = " << arr_size;
```

    array's size or length = 7


```c++
cout << "last grade = " << grades[arr_size-1] << endl;
```

    last grade = 89.9


### array size is fixed!
- one has to know how many elements will be stored in a given array
- what happens when the array is full?


```c++
// grades doesn't have index 7 as the size is 7
grades[7] = 67;
```

    [1minput_line_22:3:1: [0m[0;1;35mwarning: [0m[1marray index 7 is past the end of the array (which contains 7 elements) [-Warray-bounds][0m
    grades[7] = 67;
    [0;1;32m^      ~
    [0m[1minput_line_14:4:1: [0m[0;1;30mnote: [0marray 'grades' declared here[0m
    float grades[] = {90.5f, 34.5f, 56, 81, 99, 100, 89.9};
    [0;1;32m^
    [0m

## Array and Pointers
- there's a lot of similarity on how array and pointers work!
    - they can be used interchangebly as needed


```c++
int ids[] = {100, 200, 300, 400};
```


```c++
// copy the base address of array
// which is the address of element at index 0; which is &ids[0];
int * ptr = ids;
```


```c++
// print all the memory addresses
cout << ptr << " equals to " << &ids[0] << " equals to " << ids;
```

    0x7ffff7e45080 equals to 0x7ffff7e45080 equals to 0x7ffff7e45080


```c++
// print the data
cout << *ptr << " equals to " << ids[0] << " equals to " << *ids;
```

    100 equals to 100 equals to 100


```c++
// using pointers to traverse array
// point to the second element
ptr++;
```


```c++
cout << *ptr << endl;
```

    200



```c++
ptr = ids; // copy the base address
for(int i=0; i<4; i++) {
    cout << i << "-> " << *(ptr+i) << " == " << ptr[i] << " == " << ids[i] << endl;
}
```

    0-> 100 == 100 == 100
    1-> 200 == 200 == 200
    2-> 300 == 300 == 300
    3-> 400 == 400 == 400


## Dynamic array
- array size can be determined during run time (program execution)
    - once the size is set, it's fixed
- local dynamic array is allocated on the heap memory segment using pointer and **new** operator
- syntax to declare dynamic array:
```cpp
type * arrayName = new type[size];
```
- size can be a variable determined or assigned during program execution
- once the dynamic array is declared, using dynamic array is same as using static array
- dynamic memory must be deallocated to prevent memory leak
- syntax:
```
delete[] arrayName;
```

### visualize dynamic array in [pythontutor.com](http://pythontutor.com/cpp.html#code=//%20pass%20by%20value%20demo%0A%23include%20%3Ciostream%3E%0Ausing%20namespace%20std%3B%0A%0Aint%20main%28%29%20%7B%0A%20%20size_t%20arr_size%20%3D%205%3B%0A%20%20int%20*%20nums%20%3D%20new%20int%5Barr_size%5D%3B%0A%20%20nums%5B0%5D%20%3D%2010%3B%0A%20%20nums%5B1%5D%20%3D%2020%3B%0A%20%20nums%5B2%5D%20%3D%2030%3B%0A%20%20nums%5B3%5D%20%3D%2040%3B%0A%20%20nums%5B4%5D%20%3D%2050%3B%0A%20%20nums%5B1%5D%20%3D%20nums%5B2%5D%20%2B%20nums%5B3%5D%3B%0A%20%20for%28int%20i%3D0%3B%20i%3C5%3B%20i%2B%2B%29%0A%20%20%20%20cout%20%3C%3C%20nums%5Bi%5D%20%3C%3C%20endl%3B%0A%20%20%0A%20%20delete%5B%5D%20nums%3B%0A%20%20return%200%3B%0A%7D&curInstr=0&mode=display&origin=opt-frontend.js&py=cpp&rawInputLstJSON=%5B%5D)


```c++
size_t capacity;
```


```c++
cout << "How many integers would you like to enter? ";
cin >> capacity;
```

    How many integers would you like to enter? 3



```c++
int * some_array = new int[capacity];
```


```c++
// prompt user to store capacity number of integers and store them into array
for(int i=0; i<capacity; i++) {
    cout << "Enter a number: ";
    cin >> some_array[i];
}
```

    Enter a number: 10
    Enter a number: 20
    Enter a number: 30



```c++
// output some values
cout << capacity << " " << some_array[0] << " " << some_array[capacity-1];
```

    3 10 30

## Aggregate operations on arrays
- some commonly used aggregate operators are (=, math operators (+, \*, etc.), comparison operators (>, ==, etc.)
- array doesn't allow any aggregate operations as a whole
    - e.g. copy one array into another; printing the whole array, etc. are arregate operations
    - it doesn't make sense to compare two arrays (compare with what elements' values?)
    - Input/Output needs to be done one element at a time
    
### shallow copy with = operator
- both dynamic and static arrays CAN'T be copied to another static array using = operator
- both dynamic and static array can be assigned to another dynamic array
    - however, it doesn't actually copy the data
- copying one array into another by its name copies only the base address
    - thus creating two allias pointing to the same memory location
    - if one is modified, the other is modified as well

### visualize shallow copy using [pythontutor.com](http://pythontutor.com/cpp.html#code=//%20shallow%20copy%20array%0A%23include%20%3Ciostream%3E%0Ausing%20namespace%20std%3B%0A%0Aint%20main%28%29%20%7B%0A%20%20int%20array1%5B%5D%20%3D%20%7B1,%202,%203,%204%7D%3B%0A%20%20int%20*%20array2%20%3D%20new%20int%5B4%5D%3B%0A%20%20array2%20%3D%20array1%3B%0A%20%20array2%5B0%5D%20%3D%20100%3B%0A%20%20array1%5B3%5D%20%3D%20400%3B%0A%20%20return%200%3B%0A%7D&curInstr=0&mode=display&origin=opt-frontend.js&py=cpp&rawInputLstJSON=%5B%5D)


```c++
int * copy_array = new int[arr_size];
```


```c++
// try to copy some_array into copy_array as a whole
copy_array = some_array;
```


```c++
// let's see some values
cout << some_array[0] << " == " << copy_array[0];
```

    10 == 10


```c++
// let's update some_array
some_array[0] = 100;
```


```c++
// now, let's see the value of copy_array[0]
cout << some_array[0] << " == " << copy_array[0];
```

    100 == 100

### deep copy
- deep copy refers to the actual copy of the data
- data from one array must be copied to another array element by element
- must write your own function or code to achieve the deep copy
- Note: destination array type must match the source array type
- Note: destination array size must be at least as big as the source array size


```c++
// let's copy some_array created above
// let's create an empty array to deep copy data to
int * deep_copy = new int[capacity];
```


```c++
// let's deep copy 
for(int i=0; i<capacity; i++)
    deep_copy[i] = some_array[i];
```


```c++
// if one array is modified it doesn't affect the other array
deep_copy[0] *= 2; // update the first element with twice its value
```




    200




```c++
// let's print the copied data side by side
for(int i=0; i<capacity; i++) {
    cout << i << " -> " << deep_copy[i] << " " << some_array[i] << endl;
}
```

    0 -> 200 100
    1 -> 20 20
    2 -> 30 30



```c++
deep_copy
```




    @0x7fffffffbf70



## Passing array to function
- arrays (both static and dynamic) can be passed to a function
- array provides a very efficient way to pass a larger number of similar values without copying them
    - pass-by reference is by default and the only way!
    - array can't be pass-by value


```c++
// since actual size of the array is not easy to determine,
// size of the array is also passed as an argument
void updateArray(int array[], int size) {
    for(int i = 0; i<size; i++) {
        array[i] *= 2; // simply double the value of each element
    }
}
```


```c++
// print array function; notice passing pointer
void printArray(int * array, int size) {
    cout << "{";
    for(int i=0; i<size; i++)
        cout << array[i] << ", ";
    cout << "}\n";
}
```


```c++
printArray(some_array, arr_size);
```

    {100, 20, 30, 0, 32, 0, 737, }



```c++
updateArray(some_array, arr_size);
```


```c++
printArray(some_array, arr_size);
```

    {200, 40, 60, 0, 64, 0, 1474, }


## Returning array from function
- since assignment operator= is not allowed on array, returning a local static array is not possible
- dynamic array is possbile but not the best practice!
    - details as to why it's a bad practice is left for your own research and exploration
    - it has to do with the ownership and memory management (deleting memory, etc.)
- best practice is to pass an empty array (pass-by reference) and get it filled inside the function
    - getting the data/result out of the function without explictly returing it from a function
- quick demo of returning dynamic array can be visualized at [pythontutor.com](http://pythontutor.com/cpp.html#code=//%20returning%20dynamic%20array%20from%20a%20function%0A//%20not%20recommended%20best%20practice!%0A%23include%20%3Ciostream%3E%0Ausing%20namespace%20std%3B%0A%0Aint%20*%20getData%28%29%20%7B%0A%20%20int%20*%20nums%20%3D%20new%20int%5B2%5D%3B%0A%20%20nums%5B0%5D%20%3D%2010%3B%0A%20%20nums%5B1%5D%20%3D%2020%3B%0A%20%20return%20nums%3B%0A%7D%0A%0Aint%20main%28%29%20%7B%0A%20%20int%20*%20numsPtr%20%3D%20getData%28%29%3B%0A%20%20cout%20%3C%3C%20numsPtr%5B0%5D%20%3C%3C%20endl%3B%0A%20%20return%200%3B%0A%7D&curInstr=0&mode=display&origin=opt-frontend.js&py=cpp&rawInputLstJSON=%5B%5D) 

## Negative index and array bounds
- TinyCore CTF level3 hint
- negative index is allowed in array
- you'll only get warning for going out of bounds on array on either end


```c++
int nums2[] = {100, 200, 300};
int nums[] = {1, 2, 3};
int nums1[] = {10, 20, 30};
```


```c++
printf("value at index %d = %d\n", 0, nums[0]);
```

    value at index 0 = 1



```c++
printf("value at index %d = %d\n", 0, nums[2]);
```

    value at index 0 = 3



```c++
printf("value at index %d = %d\n", 0, nums[3]);
```

    [1minput_line_55:2:40: [0m[0;1;35mwarning: [0m[1marray index 3 is past the end of the array (which contains 3 elements) [-Warray-bounds][0m
     printf("value at index %d = %d\n", 0, nums[3]);
    [0;1;32m                                       ^    ~
    [0m[1minput_line_52:3:1: [0m[0;1;30mnote: [0marray 'nums' declared here[0m
    int nums[] = {1, 2, 3};
    [0;1;32m^
    [0m

    value at index 0 = 10



```c++
printf("value at index %d = %d\n", 0, nums[5]);
```

    [1minput_line_56:2:40: [0m[0;1;35mwarning: [0m[1marray index 5 is past the end of the array (which contains 3 elements) [-Warray-bounds][0m
     printf("value at index %d = %d\n", 0, nums[5]);
    [0;1;32m                                       ^    ~
    [0m[1minput_line_52:3:1: [0m[0;1;30mnote: [0marray 'nums' declared here[0m
    int nums[] = {1, 2, 3};
    [0;1;32m^
    [0m

    value at index 0 = 30



```c++
printf("value at index %d = %d\n", 0, nums[-1]);
```

    [1minput_line_57:2:40: [0m[0;1;35mwarning: [0m[1marray index -1 is before the beginning of the array [-Warray-bounds][0m
     printf("value at index %d = %d\n", 0, nums[-1]);
    [0;1;32m                                       ^    ~~
    [0m[1minput_line_52:3:1: [0m[0;1;30mnote: [0marray 'nums' declared here[0m
    int nums[] = {1, 2, 3};
    [0;1;32m^
    [0m

    value at index 0 = 300



```c++
printf("value at index %d = %d\n", 0, nums[-3]);
```

    [1minput_line_58:2:40: [0m[0;1;35mwarning: [0m[1marray index -3 is before the beginning of the array [-Warray-bounds][0m
     printf("value at index %d = %d\n", 0, nums[-3]);
    [0;1;32m                                       ^    ~~
    [0m[1minput_line_52:3:1: [0m[0;1;30mnote: [0marray 'nums' declared here[0m
    int nums[] = {1, 2, 3};
    [0;1;32m^
    [0m

    value at index 0 = 100



```c++

```
