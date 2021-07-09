#include "code.h"
#include "endian.h"
#include "io.h"
#include "trie.h"
#include "word.h"

#include <fcntl.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "i:o:v"

int BitLen(uint16_t code) //helper function to calculate bitlen
{
    if (code >= 1) {
        return ((int) (log2((long) code) + 1));
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int infile = 0;
    int outfile = 1;
    bool verbose = false;
    int opt = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': verbose = true; break;
        case 'i':
            infile = open(optarg, O_RDONLY);
            if (infile < 0) {
                fprintf(stderr, "Error- try with valid input\n");
                exit(0);
            }
            break;
        case 'o':
            outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (infile < 0) {
                fprintf(stderr, "Error- try with valid input\n");
                exit(0);
            }
            break;
        default: fprintf(stderr, "Usage %s -[i:o:v]\n", argv[0]);
        }
    }

    /*int main(void)
{
    int infile, outfile;
    FileHeader h;
    infile = open("output.lz78", O_RDONLY);
    if (infile < 0)
    {
        perror("infile failed");
        exit(1);
    }

    outfile = open("decompressed.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfile < 0)
    {
        perror("outfile failed");
        exit(1);
    }
}*/

    FileHeader h;
    read_header(infile, &h);
    /*printf("After read header \n");
    printf("%X \n", h.magic);
    printf("%d \n", h.protection);*/
    if (h.magic != MAGIC) {
        fprintf(stderr, "Magic number is incorrect.\n");
        return 1;
    }
    fchmod(outfile, h.protection);

    WordTable *table = wt_create();
    //printf("Completed WT create before while readpair\n");
    uint8_t curr_sym = 0;
    uint16_t curr_code = 0;
    uint16_t next_code = START_CODE;
    while (read_pair(infile, &curr_code, &curr_sym, BitLen(next_code)) == true) {
        //printf("Decode --> Entered while read: curr_code = %X and next_code = %X\n", curr_code, next_code);
        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        //printf("Decode: Word set at index curr_code = %X\n", curr_code);
        write_word(outfile, table[next_code]);
        next_code = next_code + 1;
        if (next_code == MAX_CODE) {
            wt_reset(table);
            next_code = START_CODE;
        }
    }
    flush_words(outfile);
    wt_delete(table);

    close(infile);
    close(outfile);

    if (verbose) {
        fprintf(stderr, "Compressed file size: %lu bytes\n", total_bits / 8);
        fprintf(stderr, "Uncompressed file size: %lu bytes\n", total_syms);
        fprintf(stderr, "Space saving: %.2f%%\n",
            (100 * (1 - ((double) (total_bits / 8) / (double) total_syms))));
    }
    return 0;
}
