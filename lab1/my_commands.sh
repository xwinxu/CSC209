#!/usr/bin/env bash

# run echo_arg with the command line argument csc209
./echo_arg csc209 > echo_out.txt

# run echo_stdin with standard inpt redirected from the file echo_stdin.c
./echo_stdin < echo_stdin.c

# determine the total number of digits in the decimal rep of numbers from 0 to 209 inclusive
./count 210 | wc -m

# print out the name of the largest file in the current directory
ls -S | ./echo_stdin
