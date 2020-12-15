#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    char buffer[100] = "Hi";
    cout << buffer << endl;
    strcat(buffer, " there!");
    cout << buffer << endl;
    char str[] = "there!"; // Note size is 7; including the NULL
    printf("size of buffer = %d and strlen = %d\n", sizeof(buffer), strlen(buffer));
    printf("size of str = %d and strlen = %d\n", sizeof(str), strlen(str));
    memcpy(buffer+3, str, sizeof(str));
    cout << buffer << endl;
    memset(buffer+3, 'A', 20);
    cout << buffer << endl;
    strcpy(buffer+20, str);
    cout << buffer << endl;
    return 0;
}