#! /usr/bin/env python3

# how to invoke C function in Python
from ctypes import *
libc = cdll.LoadLibrary("libc.so.6")
libc.puts("Hello World\n")
