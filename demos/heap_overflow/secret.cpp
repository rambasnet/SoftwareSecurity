#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <unistd.h> //getuid()
#include <sys/types.h> // getuid()
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

void usage(char *prog_name, char *filename) {
   printf("Usage: %s <secret to add to %s>\n", prog_name, filename);
   exit(0);
}

int main(int argc, char* argv[]) {
    int userid;
    char *secret, *secret_file;
    ofstream fout;
    
    secret = new char[100];
    secret_file = new char[20];


    strcpy(secret_file, "/var/secret");

    if (argc <2)
        usage(argv[0], secret_file);
    
    strcpy(secret, argv[1]);

    printf("[DEBUG] secret      @ %p: \'%s\'\n", secret, secret);
    printf("[DEBUG] secret_file @ %p: \'%s\'\n", secret_file, secret_file);

    userid = getuid();
    fout.open(secret_file, ios_base::app); // append mode
    if (!fout) {
        cerr << "Error while opening file\n";
        cerr << "Make sure " << argv[0] << " has r/w permission to /var folder\n";
        exit(1);
    }
    fout << userid << "\n" << secret << endl;
    fs::permissions(secret_file, fs::perms::group_read|fs::perms::others_read, fs::perm_options::remove);
    fout.close();
    cout << "Secret saved.\n";
    delete[] secret;
    delete[] secret_file;
}