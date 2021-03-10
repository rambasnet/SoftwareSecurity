#include <cstring>
#include <iostream>
#include <cstdlib>

using namespace std;

bool check_authentication(char *password) {
    char password_buffer[16];
    bool auth_flag = false;

    strcpy(password_buffer, password);

    if(strcmp(password_buffer, "brillig") == 0)
        auth_flag = true;
    if(strcmp(password_buffer, "outgrabe") == 0)
        auth_flag = true;

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
	
