#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

void bad() {
    char buffer[100];
    printf("buffer is at %p\n", buffer);
    cout << "Enter some text: ";
    cin >> buffer;
    cout << "buffer contains: \n" << buffer << endl;
}

int main(void) {
    bad();
    cout << "Good bye!" << endl;
    return 0;
}