#include <cstring>
#include <iostream>

using namespace std;

void valid() {
    cout << "\n-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
    cout << "      Access Granted.\n";
    cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
}

void invalid() {
    cout << "\nAccess Denied.\n";
    cout << "Try again...\n";
}

int check_authentication(char *password) {
    char password_buffer[16];

    strcpy(password_buffer, password);

    if(strcmp(password_buffer, "brillig") == 0)
        return true;
    if(strcmp(password_buffer, "outgrabe") == 0)
        return true;

    return false;
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        cout << "Usage: " << argv[0] << " password\n";
        exit(0);
    }
    if(check_authentication(argv[1]))
        valid();
    else
        invalid();
        
    return 0;
}