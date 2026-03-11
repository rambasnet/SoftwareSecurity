#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include "util.h"

void vuln()
{
    struct
    {
        char padding[16];
        char buff[32];
        int secret;
        int notsecret;
    } frame;

    memset(frame.buff, 0, sizeof(frame.buff)); // Zero-out the buffer.
    frame.secret = 0xdeadbeef;
    frame.notsecret = 0xffffff00;
    memset(frame.padding, 'B', sizeof(frame.padding)); // Fill padding with a marker.

    // Initializes the stack visualization. Don't worry about it!
    init_visualize(frame.buff, frame.padding, &frame.notsecret, &frame.secret);

    // Prints out the stack before modification
    visualize(frame.buff, frame.padding, &frame.notsecret, &frame.secret);

    printf("Input some text: ");
    gets(frame.buff); // This is a vulnerable call!

    // Prints out the stack after modification
    visualize(frame.buff, frame.padding, &frame.notsecret, &frame.secret);

    // Check if secret has changed.
    if (frame.secret == 0x67616c66)
    {
        puts("You did it! Congratuations!");
        print_flag(); // Print out the flag. You deserve it.
        return;
    }
    else if (frame.notsecret != 0xffffff00)
    {
        puts("Uhmm... maybe you overflowed too much. Try deleting a few characters.");
    }
    else if (frame.secret != 0xdeadbeef)
    {
        puts("Wow you overflowed the secret value! Now try controlling the value of it!");
    }
    else
    {
        puts("Maybe you haven't overflowed enough characters? Try again?");
    }

    exit(0);
}

int main()
{
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    safeguard();
    vuln();
}
