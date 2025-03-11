#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
using namespace std;

void get_shell() {
    system("sh");
}

void bad() {
    char buffer[64];
    printf("Enter some text: ");
    cin >> buffer;
    printf("You entered: %s\n", buffer);
}

int main(int argc, char** argv) {
    printf("Welcome... Enter to continue.\n");
    cin.get();
    system("clear");
    bad();
    printf("Good bye!\n");
    return 0;
}
