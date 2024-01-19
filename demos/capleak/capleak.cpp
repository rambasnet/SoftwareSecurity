// https://github.com/seed-labs/seed-labs/blob/master/category-software/Environment_Variable_and_SetUID/Labsetup/cap_leak.c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char* argv[]) {
    int fd;
    char *v[2];
    
    fd = open("/etc/xyz", O_RDWR | O_APPEND | O_CREAT); 
    if (fd == -1) {
        printf("Failed to open /etc/xyz\n");
        exit(1);
    }
    // print out the file descriptor value
    printf("fd is %d\n", fd);
    // FIXME - close the file
    // close(fd);
    // downgrade the privilege my making the effective uid the same as the real uid
    setuid(getuid());
    // execute /bin/sh
    v[0] = "/bin/sh"; v[1] = 0;
    execve(v[0], v, 0);
    return 0;
}
