#include <cstdio>
#include <stdlib.h> // getenv, putenv, setenv, unsetenv, etc.
// see: http://www.manpagez.com/man/3/getenv/

int main(int argc, char* argv[]) {
	if (argc == 2) {
		// read value of env variable
		//getenv(char * name) retruns NUL-terminated string; if name doesn't exists, returns NULL
		printf("%s=%s\n", argv[1], getenv(argv[1]));
	}
	else if (argc == 3) { 
		// set env variable, 0 doesn't overwrite, 1 overwrites
		int result = setenv(argv[1], argv[2], 0); // returns 0 on successful, -1 otherwise
		if (result == 0) {
			printf("Successfully set env variable!\n");
			printf("%s=%s\n", argv[1], getenv(argv[1]));
			// you can't use setenv to export variables from current process to the calling process (shell)
		}
		else {
			printf("Couldn't set env variable!");
		}
	}
	else {
		printf("%s\n", "Usage READ: program ENV_VAR_NAME");
		printf("%s\n", "Usage SET: program ENV_VAR_NAME VALUE");
	}
	return 0;
}
