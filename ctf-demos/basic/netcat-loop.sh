#! /bin/bash

# script to run netcat server in a loop

while true
do
    nc -v -l 127.0.0.1 -p 1234 -e vuln.exe
done