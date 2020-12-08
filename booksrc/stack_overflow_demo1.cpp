#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

void bad(char *data) {
    char buffer[100];
    printf("buffer is at %p\n", buffer);
    strcpy(buffer, data);
    cout << "buffer contains: \n" << buffer << endl;
}

int main(int argc, char *argv[]) {
    bad(argv[1]);
    cout << "Good bye!" << endl;
    return 0;
}