#include <cstdlib>
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char* argv[]) {
    char buffer[100];
    strcpy(buffer, argv[1]);
    system(buffer);
    cout << "Good bye!";
    return 0;
}