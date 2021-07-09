#include "code.h"
#include "endian.h"
#include "io.h"
#include "trie.h"
#include "word.h"

#include <fcntl.h>
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

    struct stat sb;
    fstat(infile, &sb);

    FileHeader h;
    h.magic = MAGIC;
    h.protection = sb.st_mode;

    fchmod(outfile, h.protection);
    write_header(outfile, &h);

    TrieNode *root = trie_create();
    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;
    uint8_t curr_sym = 0;
    uint8_t prev_sym = 0;
    uint16_t next_code = START_CODE;

    while (read_sym(infile, &curr_sym) == true) {
        TrieNode *next_node = trie_step(curr_node, curr_sym);
        if (next_node != NULL) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            write_pair(outfile, curr_node->code, curr_sym, BitLen(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code);
            curr_node = root;
            next_code = next_code + 1;
        }
        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }
        prev_sym = curr_sym;
    }
    if (curr_node != root) {
        write_pair(outfile, prev_node->code, prev_sym, BitLen(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }
    write_pair(outfile, STOP_CODE, 0, BitLen(next_code));
    flush_pairs(outfile);

    trie_reset(root);
    trie_node_delete(root);

    close(outfile);
    close(infile);

    if (verbose) {
        fprintf(stderr, "Compressed file size: %lu bytes\n", total_bits / 8);
        fprintf(stderr, "Uncompressed file size: %lu bytes\n", total_syms);
        fprintf(stderr, "Space saving: %.2f%%\n",
            (100 * (1 - ((double) (total_bits / 8) / (double) total_syms))));
    }

    return 0;
}
