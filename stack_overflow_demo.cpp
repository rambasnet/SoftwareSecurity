#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

void bad(char *data) {
    char buffer[64];
    strcpy(buffer, data);
    cout << buffer << " is at " << &buffer << endl;
}

int main(int argc, char *argv[]) {
    bad(argv[1]);
    cout << "Good bye!" << endl;
    return 0;
}