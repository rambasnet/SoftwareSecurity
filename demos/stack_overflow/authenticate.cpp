#include <cstring>
#include <iostream>
#include <cstdlib>

using namespace std;

int check_authentication(char *password) {
    int auth_flag = 0;
    char password_buffer[16];

    strcpy(password_buffer, password);

    if(strcmp(password_buffer, "brillig") == 0)
        auth_flag = 1;
    if(strcmp(password_buffer, "outgrabe") == 0)
        auth_flag = 1;

    return auth_flag;
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        cout << "Usage: " << argv[0] << " password\n";
        exit(0);
    }
    if(check_authentication(argv[1])) {
        cout << "\n-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
        cout << "      Access Granted.\n";
        cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
    } 
    else
        cout << "\nAccess Denied.\n";
    return 0;
}
	
