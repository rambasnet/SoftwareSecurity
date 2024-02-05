#include <cstdio>
#include <unistd.h>

extern char** environ;

int main(int argc, char* argv[], char* envp[]) {
    int i = 0;
    char* arg[2];
    char* newenv[3];
    if (argc < 2) {
        printf("Missing required argument 1, 2, 3");
        printf("Usage: ./execve_passenv [1/2/3]");
        return 1;
    }

    // construct the argument array
    arg[0] = "/usr/bin/env"; arg[1] = NULL;

    // construct the environment variable array
    newenv[0] = "AAA=aaa";
    newenv[1] = "BBB=123";
    newenv[2] = NULL;
    switch(argv[1][0]) {
        case '1': // passing no env variable
            execve(arg[0], arg, NULL);
            break;
        case '2': // passing new env variables
            execve(arg[0], arg, newenv); 
            break;
        case '3': // passing current env variables
            execve(arg[0], arg, environ);
            break;
        default: // passing no env variable
            execve(arg[0], arg, NULL);
    }

}
