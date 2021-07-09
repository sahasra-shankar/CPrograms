# Assignment 1

This program uses a test harness with command line options to create a small numerical library. The numerical library consists of the outputs sin, cos, tan, exponential, and logarithmic functions. Outputs can be specified using getopt() options in command line. 

## Build

    $ clang -Wall -Wextra -Werror -Wpedantic -o mathlib-test mathlib-test.c

## Clean

    $ rm -f mathlib-test mathlib-test.o
 
