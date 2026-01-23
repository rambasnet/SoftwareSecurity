#include <stdio.h>
#include <string.h>

void func1(char *s)
{
    char buffer[12];
    strcpy(buffer, s);
    printf("Buffer: %s\n", buffer);
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        func1(argv[1]);
        printf("Everything is fine.\n");
    }
}