#! /bin/sh

FILE=$1

if ./bminor --codegen $FILE program.s ; then 
    if gcc -g program.s ./src/library.c -o program ; then
        ./program
        echo "output from run: $?"
        exit 0
    else
        echo "failure on gcc"
    fi
else 
    echo "failure on bminor"
fi
exit 1
