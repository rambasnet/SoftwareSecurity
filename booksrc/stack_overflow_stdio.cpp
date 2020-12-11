#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace std;

#define BUFSIZE 128

using namespace std;

char * mgets(char *dst) {
    char *ptr = dst;
	char ch; 
    // int ch;
	/* skip leading white spaces */ 
    // ch = getchar()
	while (cin.get(ch) && ch == ' ' or ch == '\t') 
		; 
 
	if ((ch == '\n') or (ch == EOF)) { 
		*ptr = '\0';
		return dst; 
	} 
	else
		*ptr = ch;
 
	/* now read the rest until \n or EOF */ 
	while (ch = getchar()) {
        if (ch == '\n' or ch == EOF) break;
		*(++ptr) = ch; 
	} 
    *(++ptr) = 0;
    return dst;
}

void bad() {
    char buffer[BUFSIZE];
    printf("buffer is at %p\n", buffer);
    printf("Enter some data: ");
    mgets(buffer);
    //copydata(buffer, data);
    cout << "buffer contains: " << buffer << endl;
    printf("size of buffer = %d and length is %d\n", sizeof(buffer), strlen(buffer));
}

int main(int argc, char *argv[]) {
    // Set the gid to the effective gid
    // this prevents /bin/sh from dropping the privileges
    //uid_t uid = getuid();
    //setresgid(uid, uid, uid);
    bad();
    cout << "Good bye!\n";
    return 0;
}