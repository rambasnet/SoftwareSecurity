#include <unistd.h>
#include <cstdio>

int main() {
    printf("I'm sleeping for 2 seconds.\n");
    sleep(2);
    printf("I'm awake now...\n");
    return 0;
}