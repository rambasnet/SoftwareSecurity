# Simple Buffer Overflow CTF

1. Goal of this CTF is to get the flag by exploiting a buffer overflow vulnerability in the provided C code.

## Instructions
1. Download all the files (.h, .c, Makefile) in the current directory.
2. Compile the code using the provided Makefile by running `make` in the terminal.
3. Run the compiled binary to see the vulnerable program in action
4. Note that the program takes user input as an argument and processes it in a way that can lead to a buffer overflow.
5. Use your knowledge of buffer overflows to craft an input that will allow you to call print_flag() logic and retrieve the flag.
6. Use the visual hint and gdb/peda to analyze the program's memory layout and identify the correct input length and value to exploit the vulnerability.
7. Once you have successfully exploited the vulnerability, you should see the flag printed on the terminal.

## Notes
- you're not allowed to modify the source code of the program, but you can use any tools or techniques to analyze and exploit the vulnerability of the binary
- you're not supposed to use reverse engineering tools to directly read the flag from the binary, but you can use them to understand the program's behavior and memory layout to craft your exploit.
    - you're given the sources!
