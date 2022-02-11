#include <stdlib.h>
#include <cstring>
#include <cstdio>

void f(char * arg)
{
	// C dynamic memory
	int* x = (int *)malloc(10 * sizeof(int));
	// C++ dynamic memory
	char* name = new char[20];

	x[9] = 0; // problem 1: heap block overrun
			// problem 2: memory leak -- x not freed
	strncpy(name, arg, sizeof(char)*20-1);
	name[19] = '\0';
	// problem 3: heap block overrun
	// problem 4: memory leak -- name not freed
	printf("Hello %s\n", name);
	free(x); // C
	delete[] name; // C++
}

int main(int argc, char* argv[1])
{
	// what if f() is called over and again in an infinite loop, e.g. 
	f(argv[1]);
	return 0;
}
