#include "bm.h"
#include "hamming.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "i:o:"

uint8_t lower_nibble(uint8_t b) {
    return b & 0x0F;
}

uint8_t pack_nibbles(uint8_t highNib, uint8_t lowNib) {
    highNib = highNib << 4;
    return highNib | lowNib;
}

int main(int argc, char *argv[]) {
    FILE *infile = stdin;
    FILE *outfile = stdout;
    int opt = 0;
    char *hamFile, *outputf;
    struct stat buf;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            hamFile = malloc(strlen(optarg));
            strcpy(hamFile, optarg);
            infile = fopen(hamFile, "r");
            if (infile == NULL) {
                fprintf(stderr, "Error- use valid input\n");
                exit(0);
            }
            break;
        case 'o':
            outputf = malloc(strlen(optarg));
            strcpy(outputf, optarg);
            if (strcmp(outputf, "stdout") != 0) {
                outfile = fopen(outputf, "w");
                if (outfile == NULL) {
                    fprintf(stderr, "Error- provide valid outfile\n");
                    exit(0);
                }
            }
            break;
        default: fprintf(stderr, "Usage %s -[i:o:]\n", argv[0]);
        }
    }
    fstat(fileno(infile), &buf);
    fchmod(fileno(outfile), buf.st_mode);
    uint8_t readB;
    uint32_t totalByteCount = 0;
    uint32_t correctedErrs = 0;
    uint32_t uncorrectedErrs = 0;
    ham_init();
    if (ham_init() == HAM_OK) {
        uint8_t pos = 0;
        uint8_t pos1 = 0;
        int byteCount = 1;
        uint8_t fullByte;
        do {
            readB = fgetc(infile);
            ham_rc rc = ham_decode((uint8_t) readB, &pos);
            if (rc == HAM_OK || rc == HAM_ERR_OK) {
                if (byteCount % 2 == 1) {
                    pos1 = pos;
                } else {
                    // here pos1 + pos to get both lower nibble and upper nimble and pack
                    fullByte = pack_nibbles(pos1, pos);
                    fputc(fullByte, outfile); // check how to reference value at pointer
                }
                if (rc == HAM_ERR_OK)
                    correctedErrs++;
            } else if (rc == HAM_ERR) {
                uncorrectedErrs++;
            }
            byteCount++;
            totalByteCount++;
            if (feof(infile))
                break;

        } while (1);

        fprintf(stderr, "Total bytes processed: %d\n", totalByteCount);
        fprintf(stderr, "Uncorrected errors: %d\n", uncorrectedErrs);
        fprintf(stderr, "Corrected errors: %d\n", correctedErrs);
        fprintf(stderr, "Error rate: %.6f\n", ((double) uncorrectedErrs / (double) totalByteCount));

    } else if (ham_init() != HAM_OK) {
        printf("module failed to initialize\n");
    }
    ham_destroy();
    fclose(infile);
    fclose(outfile);
    return 0;
}
