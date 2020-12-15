#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    int num = 5;
    char buffer_one[8], buffer_two[8];

    strcpy(buffer_one, "one"); /* copy "one" into buffer_one */
    strcpy(buffer_two, "two"); /* copy "two" into buffer_two */

    printf("[BEFORE] buffer_two is at %p and contains \'%s\'\n", buffer_two, buffer_two);
    printf("[BEFORE] buffer_one is at %p and contains \'%s\'\n", buffer_one, buffer_one);
    printf("[BEFORE] num is at %p and is %d (0x%08x)\n", &num, num, num);

    printf("\n[STRCPY] copying %d bytes into buffer_two\n\n",  strlen(argv[1]));
    strcpy(buffer_two, argv[1]); /* copy first argument into buffer_two */

    printf("[AFTER] buffer_two is at %p and contains \'%s\'\n", buffer_two, buffer_two);
    printf("[AFTER] buffer_one is at %p and contains \'%s\'\n", buffer_one, buffer_one);
    printf("[AFTER] num is at %p and is %d (0x%08x)\n", &num, num, num);
    return 0;
}
