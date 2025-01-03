// aslr.cpp - program to check address space layout randomization
// compile and run the program multiple times
// if you see same value for $esp, ASLR is disabled
// $ g++ -g -m 32 -o aslr.exe aslr.cpp
#include <cstdio>
#include <iostream>
using namespace std;

int global_var = 100; // data segment
char global_name[20]; // bss segment

int main(void)
{
    char buffer[] = "Hello World!";
    register int esp asm("esp");
    register int ebp asm("ebp");
    cout << "Registers info:\n";
    cout << "===============\n";
    printf("$esp = %p\n", esp);
    printf("$ebp = %p\n", ebp);
    cout << "\nData and BSS segement\n";
    cout << "======================\n";
    printf("global_var is at: %p\n", &global_var);
    printf("global_name is at: %p\n", global_name);
    cout << "\nStack variable:\n";
    cout << "===============\n";
    printf("buffer is at: %p\n", buffer);
    cout << "\nCode segment:\n";
    cout << "=============\n";
    printf("main is at %p\n", main);
    cout << "\nShared library code\n";
    cout << "=====================\n";
    printf("printf is at %p\n", printf);
    printf("cout is at %p\n", &cout); // cout is an operator
    return 0;
}