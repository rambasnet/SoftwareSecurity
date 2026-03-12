#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

void vuln()
{

  struct
  {
    char buff[32];
    char padding[16];
  } stack_frame;

  memset(stack_frame.buff, 0, sizeof(stack_frame.buff));         // Zero-out the buffer.
  memset(stack_frame.padding, 'A', sizeof(stack_frame.padding)); // Mark the padding with 0xff.

  // Initializes the stack visualization. Don't worry about it!
  init_visualize(stack_frame.buff);

  // Prints out the stack before modification
  visualize(stack_frame.buff);

  printf("Input some text: ");
  gets(stack_frame.buff); // This is a vulnerable call!

  // Prints out the stack after modification
  visualize(stack_frame.buff);
}

int main()
{
  setbuf(stdout, NULL);
  setbuf(stdin, NULL);
  vuln();
}
