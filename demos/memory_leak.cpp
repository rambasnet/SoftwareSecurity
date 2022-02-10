  #include <stdlib.h>
  #include <cstring>
  #include <cstdio>

  void f(char * arg)
  {
  	 // C dynamic memory
     int* x = (int *)malloc(10 * sizeof(int));
     // C++ dynamic memory
     char* name = new char[20];
     
     x[10] = 0;        // problem 1: heap block overrun
                      // problem 2: memory leak -- x not freed
     strcpy(name, arg);
     // problem 1: heap block overrun
     // problem 2: memory leak -- x not freed
     printf("Hello %s\n", arg);
  }

  int main(int argc, char* argv[1])
  {
  	 // what if f() is called over and again in an infinite loop, e.g. 
     f(argv[1]);
     return 0;
  }
