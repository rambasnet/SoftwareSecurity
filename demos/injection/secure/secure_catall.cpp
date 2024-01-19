#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    char *command[3];
    
    if (argc < 2){
        printf("Usage: secure_catall <file_path>.\n");
        exit(EXIT_FAILURE);
    }
    command[0] = "/bin/cat";
    command[1] = argv[1];
    command[2] = NULL;
    execve(command[0], command, NULL);

    return 0;
}