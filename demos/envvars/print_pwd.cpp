#include <cstdio>
#include <cstdlib>

int main() {
    char buffer[100]; // this may be vulnerable!
    char *ptr;
    ptr = getenv("PWD");
    if (ptr != NULL){
        sprintf(buffer, "Current working directory is: %s", ptr);
        printf("%s\n", buffer);
    }
    return 0;
}