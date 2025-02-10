#include <cstdio>
#include <cstdlib>

int main()
{
    char buffer[100]; // this is vulnerable to stack-overflow!
    char *ptr;
    ptr = getenv("PWD");
    if (ptr != NULL)
    {
        sprintf(buffer, "Current working directory is: %s", ptr);
        printf(buffer); // this is vulnerable to printf-injection!
    }
    return 0;
}