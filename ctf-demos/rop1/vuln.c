#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h> // dlsym()
#include <string.h>
#include <unistd.h> // read()

//https://tldp.org/HOWTO/Program-Library-HOWTO/dl-libraries.html

void start(char *data) {
  fflush(stdout);
  char buf[16];
  memset(buf, 0, sizeof(buf));
  //size_t s = read(0, buf, 256);
  //strcpy(buf, data);
  printf("Acknowledged: %s with %d bytes\n", buf, strlen(data));
  
  if (!strcmp(buf, "haxor\n"))
    printf("Password OK :)\n");
  else
    printf("Invalid Password!\n");
}

int main(int argc, char *argv[])
{
  void *self = dlopen(NULL, RTLD_NOW);
  printf("stack   : %p\n", &argc);
  printf("system(): %p\n", dlsym(self, "system"));
  printf("printf(): %p\n", dlsym(self, "printf"));
  printf("exit(): %p\n", dlsym(self, "exit"));
  printf("main(): %p\n", &main);
  //printf("return: %p\n", dlsym(self, "return"));
  //system("/bin/sh");
  start(argv[1]);
  printf("Good bye!\n");
  return 0;
}
