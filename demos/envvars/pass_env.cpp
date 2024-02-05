#include <cstdio>

// envp[] can only be used within the main program 
// needs to be passsed to other functions as needed
int main(int argc, char* argv[], char* envp[]) {
    printf("Total argument count = %d\n", argc);
    for(int i=0; i < argc; i++){
        printf("arg[%d] = %s\n", i, argv[i]);
    }
    printf("Now printing environment variables accessing envp array:\n");
    int i = 0;
    while (envp[i] != NULL) {
        printf("%s\n", envp[i++]);
    }
    return 0;
}