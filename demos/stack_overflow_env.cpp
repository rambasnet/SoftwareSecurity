#include <cstring>
#include <iostream>

using namespace std;

char * mgets(char *dst) {
    char *ptr = dst;
    int ch; 
	/* skip leading white spaces */ 
    while ((ch = getchar()) && (ch == ' ' or ch == '\t')) 
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
    char buffer[16];
    cout << "Enter text: ";
    mgets(buffer);
    cout << "text = " << buffer << endl;
}

int main(int argc, char *argv[]) {
    //char *ptr;
    //ptr = getenv("SHELLCODE");
    //printf("SHELLCODE is at %p\n", ptr);
    bad();
    cout << "Good bye!\n";
    return 0;
}
	
