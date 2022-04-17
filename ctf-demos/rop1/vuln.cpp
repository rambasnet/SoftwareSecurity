#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace std;

#define BUFSIZE 16

using namespace std;

void give_shell(){
    //system("/bin/sh");
    execve("/bin/sh", NULL, NULL);
}

char * mgets(char *dst) {
    char *ptr = dst;
    int ch; 
	/* skip leading white spaces */ 
    while ((ch = getchar()) && (ch == ' ' or ch == '\t') )
        ; 

    if ((ch == '\n') or (ch == EOF)) { 
        *ptr = '\0';
        return dst; 
    } 
    else
        *ptr = ch;

    /* now read the rest until \n or EOF */ 
    while (true) {
        ch = getchar();
        if (ch == '\n' or ch == EOF) break;
        *(++ptr) = ch; 
    }
    *(++ptr) = 0;
    return dst;
}

void bad() {
    char buffer[BUFSIZE];
    cout << "Enter Password: ";
    fflush(stdout);
    //mgets(buffer); // similar to C's gets;
    memset(buffer, 0, BUFSIZE);
    //read(0, buffer, 200);
    cin >> buffer;
    //gets(buffer); // depricated
    cout << "Acknowledged: " << buffer << " with length " << strlen(buffer) << endl;
    if (strcmp(buffer, "haxor") == 0)
    	printf("Congrats... :)\n");
    else
    	printf("Sorry! Incorrect Password!!\n");
}

int main(int argc, char *argv[]) {
    gid_t gid = getegid();
    setresgid(gid, gid, gid);
    bad();
    cout << "Good bye!\n";
    give_shell();
    return 0;
}
