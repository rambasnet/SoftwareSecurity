#include <iostream>
#include <string>

using namespace std;

int main() {
    string name, address;
    int age;
    cout << "Enter your name:\n";
    getline(cin, name);
    cout << "Enter your age:\n";
    cin >> age;
    cout << "Enter your address:\n";
    cin >> ws;
    getline(cin, address);
    cout << "Hello, " << name << "! "
     << "Your're " << age << " old. "
     << "Your address is " << address << endl;
    cout << "Good bye!\n";
    return 0;
}