# Assignment 4

This program, for a given valid input, can encode and decode using Hamming codes. Even in the case that erros are introduced into the input, the program will in certain cases be able to fix the error, and provide the user with information relevant to the number of bytes processed by the program, the number of corrected and uncorrected errors and the error rate. 

## Build

	$ clang -Wall -Wextra -Werror -Wpedantic

## Clean 

	$ rm -f gen generator.o dec decoder.o 
