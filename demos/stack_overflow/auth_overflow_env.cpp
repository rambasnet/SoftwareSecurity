#include <cstring>
#include <iostream>

using namespace std;

char * mgets(char *dst) {
    char *ptr = dst;
    int ch; 
	/* skip leading white spaces */ 
    while ( (ch == getchar()) && (ch == ' ' or ch == '\t'))
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

int check_authentication() {
    int auth_flag = 0; // bool makes offset an odd value...
    char password_buffer[16];

    cout << "Enter password: ";
    mgets(password_buffer);

    if(strcmp(password_buffer, "brillig") == 0)
        auth_flag = 1;
    if(strcmp(password_buffer, "outgrabe") == 0)
        auth_flag = 1;

    return auth_flag;
}

int main(int argc, char *argv[]) {
    if(check_authentication()) {
        cout << "\n-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
        cout << "      Access Granted.\n";
        cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
    } 
    else
        cout << "\nAccess Denied.\n";
    return 0;
}
	
