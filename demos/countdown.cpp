#include <iostream>
#include <unistd.h>
#include <cstdlib>
//unsigned int sleep(unsigned int seconds);

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Launching of Perseverence Rover count down...\n";
    sleep(1);
    system("clear");
    for(int i=10; i>=1; i--) {
        cout << i << endl;
        sleep(1);
        system("clear");
    }
    cout << "Blast Off!\n";
    sleep(1);
    system("clear");
    return 0;
}