#!/bin/bash

echo 'Usage: ./MAKE.SH [check]
	check - run valgrind'
gcc range.c -o range
./range > tmp.txt
echo $?
if [ "$1" = 'check' ]; then
	which valgrind || echo sudo apt-get install valgrind
	valgrind ./range
fi
