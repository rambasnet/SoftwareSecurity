#! /bin/bash

# script to run netcat server in a loop

while true
do
    nc -v -l -p 2345 -e vuln.exe
done