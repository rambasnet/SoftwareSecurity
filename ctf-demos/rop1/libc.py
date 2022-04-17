#! /usr/bin/env python3

# how to invoke C function in Python
from ctypes import *
libc = cdll.LoadLibrary("/lib/x86_64-linux-gnu/libc.so.6")
libc.printf(b"Hello World\n")

