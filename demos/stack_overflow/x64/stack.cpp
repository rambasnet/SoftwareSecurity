#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

using namespace std;

const int MAX_DATA = 200;

void vulnerable(char *str) {
	char buffer[100];
	// printf("Buffer is at %p\n", &buffer);
	strcpy(buffer, str);
	cout << buffer << endl;
}

int main(int argc, char *argv[]) {
	char data[MAX_DATA];
	//file_name = strcpy(file_name, argv[1]
	FILE *fin;
	fin = fopen(argv[1], "r");
	fread(data, sizeof(char), MAX_DATA, fin);
	vulnerable(data);
	cout << "All done..." << endl;
	return 0;
}

