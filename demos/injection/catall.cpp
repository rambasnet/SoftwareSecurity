#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *v[3];
    char program[] = "/bin/cat";
    char *command;
    if (argc < 2)
    {
        printf("Usage: catall <file_path>.\n");
        exit(EXIT_FAILURE);
    }
    v[0] = program; v[1] = argv[1]; v[2] = NULL;
    command = (char *)malloc(strlen(v[0]) + strlen(v[1]) + 2);
    sprintf(command, "%s %s", v[0], v[1]);
    //printf("argv[1] = %s\n", argv[1]);
    system(command);

    return 0;
}