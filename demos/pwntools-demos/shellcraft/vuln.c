#include <stdio.h>
#include <string.h>

void vulnerable_function()
{
    char buffer[64];
    printf("Enter your input: ");
    gets(buffer); // Vulnerable function (no bounds check)
    printf("You entered: %s\n", buffer);
    printf("Buffer is at: %p\n", buffer);
}

int main()
{
    vulnerable_function();
    return 0;
}
