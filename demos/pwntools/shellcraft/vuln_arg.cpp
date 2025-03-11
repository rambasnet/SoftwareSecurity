#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void get_shell() {
    system("sh");
}

void bad(char * data) {
    char buffer[64];
    strcpy(buffer, data);
    printf("%s\n", buffer);
    printf("Buffer is at %p\n", buffer);
}

int main(int argc, char** argv) {
    bad(argv[1]);
}
