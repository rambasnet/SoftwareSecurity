#!/bin/bash 
SECONDS=0 
value=0 
while true; 
    do value=$(( $value + 1 ))
    duration=$SECONDS 
    min=$(($duration / 60)) 
    sec=$(($duration % 60)) 
    echo "$min minutes and $sec seconds elapsed." 
    echo "The program has been running $value times so far." 
    ./argv.exe $(cat argv_payload.bin)
    sleep 1
done