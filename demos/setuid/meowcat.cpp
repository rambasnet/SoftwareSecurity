// Program that mimics bash cat program
#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("Usage: program <filepath>\n");
        return 1;
    }
    ifstream fin;
    fin.open(argv[1], fin.in | fin.binary | fin.ate);

    if (!fin.is_open())
        cout << "Failed to open " << argv[1] << '\n';
    else {
        // findout the size of the the file; get position in input sequence
        size_t size = fin.tellg();
        // Set position in input sequence
        fin.seekg(0, fin.beg);
        // allocate memory to store file contents
        char * buffer = new char[size];
        
        if (fin.read(buffer, size))
        {
            cout << buffer << endl;
            // parse buffer in memory...
        }
        delete[] buffer;
        fin.close();
    }
    return 0;
}
