# Assignment 7

This assignment includes two programs: encode and decode, that perform LZ78 compression and decompression respectively. The encoding program makes use of tries and bit masking to compress a given input file while the decoding program makes use of bit masking and word tables to decompress a compressed input file. The decoding program will only decompress files that have been compressed with either this encode program or another encode program that uses the same "magic" number to compress.

## Build

    $ clang -Wall -Wextra -Werror -Wpedantic

## Clean

    $ rm -f encode decode *.o

## Format 

    $ clang-format -i -style=file *.c *.h 
