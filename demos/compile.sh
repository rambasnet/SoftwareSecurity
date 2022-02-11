#! /bin/bash

# disable aslr
read aslr < /proc/sys/kernel/randomize_va_space
if (( $aslr != 0 )); then
    if (( id -u != 0)); then
        echo "Need sudo priviledge to disable aslr..."
    fi
    # provide password for sudo user
    ls /proc/sys/kernel/randomize_va_space
    echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
fi

# compile disabling all the security flags but displaying all the warning
# https://gcc.gnu.org/onlinedocs/gcc-3.4.6/gcc/Warning-Options.html#Warning-Options
compiler="g++ -g -m32 -std=c++17 -Wall -Wpedantic -Wextra -Wconversion -fno-stack-protector -z execstack -no-pie"
if (( $# == 1 )); then
    $compiler $1 && echo "Output file is a.out"
elif (( $# == 2 )); then
    $compiler -no-pie -o $2 $1
else
    echo "Error... No input file provided..."
    echo "$0 <inputfile.cpp> [a.out]"
fi