#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

void copydata(char *dst_buf, char *src_buf) {
    // similar to strcpy in cstdio
    size_t i = 0;
    // each character from src_buf to des_buffer
    // read and discard '\n'
    while (*(src_buf+i) != 0) {
        *(dst_buf+i) = *(src_buf+i);
        i++;
    }
    *(dst_buf+i) = 0; // end with NUL character
}

void bad(char *data) {
    char buffer[100];
    printf("buffer is at %p\n", buffer);
    //strcpy(buffer, data);
    copydata(buffer, data);
    cout << "buffer contains: \n" << buffer << endl;
}

int main(int argc, char *argv[]) {
    bad(argv[1]);
    cout << "Good bye!" << endl;
    return 0;
}