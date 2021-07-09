#include "bm.h"
#include "hamming.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "i:o:"

uint8_t lower_nibble(uint8_t b) {
    return b & 0x0F;
}

uint8_t higher_nibble(uint8_t b) {
    return (b >> 4) & 0x0F;
}

int main(int argc, char *argv[]) {
    FILE *infile = stdin;
    FILE *outfile = stdout;
    int opt = 0;
    char *hamFile, *outputf;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            hamFile = malloc(strlen(optarg));
            strcpy(hamFile, optarg);
            infile = fopen(hamFile, "r");
            // printf("%s\n", infile);
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
    ham_init();
    if (ham_init() == HAM_OK) {
        do {
            uint8_t byteToEnc = fgetc(infile);
            uint8_t lowN = lower_nibble(byteToEnc);
            uint8_t highN = higher_nibble(byteToEnc);
            uint8_t pos1 = 0x00;
            uint8_t pos2 = 0x00;
            if (ham_encode(lowN, &pos1) == HAM_OK && ham_encode(highN, &pos2) == HAM_OK) {
                fputc(pos1, outfile);
                fputc(pos2, outfile); // check how to reference value at pointer
            } else if (ham_encode(lowN, &pos1) == HAM_ERR || ham_encode(highN, &pos2) == HAM_ERR) {
                printf("error because of module initialization failure or null "
                       "pointers\n");
            }
            if (feof(infile))
                break;
        } while (1);
    } else if (ham_init() != HAM_OK) {
        printf("module failed to initialize\n");
    }
    ham_destroy();
    fclose(infile);
    fclose(outfile);
    return 0;
}
