#include "hamming.h"

#include "bm.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static BitMatrix *G;
static BitMatrix *H;

void printCode(uint8_t r) {
    uint8_t i;
    printf("[");
    for (i = 0x80; i != 0; i >>= 1)
        printf("%c", (r & i) ? '1' : '0');
    printf("]\n");
}

ham_rc ham_init(void) {
    G = bm_create(4, 8); // set bit values efficiently instead of one by one
    if (G == NULL) {
        return HAM_ERR;
    }
    for (uint32_t s = 0; s < bm_rows(G);
         s++) // iterates through rows and cols of G and sets vals of r==c to 1
    {
        for (uint32_t p = 0; p < bm_cols(G) - 4; p++) {
            if (s == p) {
                // G->mat[s][p] = 1;
                bm_set_bit(G, s, p);
                // printf("The value at %d, %d is %u\n", s, p, G->mat[s][p]);
            }
        }
    }
    for (uint32_t q = 0; q < bm_rows(G); q++) {
        for (uint32_t b = 4; b < bm_cols(G); b++) {
            if (q != (b % 4)) {
                bm_set_bit(G, q, b);
                // printf("The value at %d, %d is %d\n", q, b, G->mat[q][b]);
            }
        }
    }
    H = bm_create(8, 4);
    if (H == NULL) {
        return HAM_ERR;
    }
    for (uint32_t i = 0; i < bm_rows(H) - 4;
         i++) // iterates through cols of BM H and only sets when value is 1
    {
        for (uint32_t j = 0; j < bm_cols(H); j++) {
            if (i != j) {
                bm_set_bit(H, i, j);
            }
        }
    }
    for (uint32_t k = 4; k < bm_rows(H); k++) // splits matrix into two halves to set more easily
    {
        for (uint32_t m = 0; m < bm_cols(H); m++) {
            if ((k % 4) == m) {
                bm_set_bit(H, k, m);
            }
        }
    }
    return HAM_OK;
}

void ham_destroy(void) // calls delete from bm
{
    bm_delete(&G);
    bm_delete(&H);
}

ham_rc ham_encode(uint8_t data,
    uint8_t *code) // passing in both nibbles in lower nibble
{
    uint8_t mask = 1;
    uint8_t databit;
    uint8_t result = 0;
    BitMatrix *C = bm_create(1, 4);
    for (uint32_t a = 0; a < 4; a++) {
        databit = (data >> a) & mask;
        if (databit == 1) {
            bm_set_bit(C, 0, a);
        }
    }
    for (uint32_t i = 0; i < bm_rows(C); i++) // iterates through created bitmat
    {
        for (uint32_t j = 0; j < bm_cols(G); j++) // iterates through G
        {
            result = 0;
            for (uint8_t k = 0; k < bm_rows(G); k++) {
                // printf(" C into G matrix %d X %d \n", bm_get_bit(C, i, k),
                // bm_get_bit(G, k, j));
                result = result + (bm_get_bit(C, i, k) * bm_get_bit(G, k, j)); // matrix mult.
            }
            *code |= result % 2 << (7 - j); // sets in each resulting product into pointer code
        }
    }
    return HAM_OK;
}

ham_rc ham_decode(uint8_t code, uint8_t *data) {
    int lookup[16] = { -2, 4, 5, -1, 6, -1, -1, 3, 7, -1, -1, 2, -1, 1, 0, -1 };
    uint8_t decMask = 1;
    uint8_t decBits;
    uint8_t outVal = 0;
    uint8_t errorCheck = 0;
    BitMatrix *D = bm_create(1, 8);
    /*if (code >= 0)
  {
      printf("entered condition: DATA = null\n");
      return HAM_ERR;
  }*/
    for (uint32_t u = 0; u < 8; u++) {
        decBits = (code >> u) & decMask;
        // datamatrix[0][a] = databit;
        if (decBits == 1) {
            bm_set_bit(D, 0, u);
        }
    }
    for (uint32_t m = 0; m < bm_rows(D); m++) {
        for (uint32_t n = 0; n < bm_cols(H); n++) {
            outVal = 0;
            for (uint8_t t = 0; t < bm_rows(H); t++) {
                // printf(" C matrix %d and G Mat %d \n", bm_get_bit(C, i, k),
                // bm_get_bit(G, k, j));
                outVal = outVal + (bm_get_bit(D, m, t) * bm_get_bit(H, t, n));
            }
            // printf("%d th col -> %u  \n", n, outVal%2 );
            if ((outVal % 2) == 1 || (outVal % 2) == 0) {
                errorCheck |= outVal % 2 << (7 - n);
                // printCode(errorCheck);
                //*code = *code >> 1;
            }
        }
    }
    uint8_t bitForInt = 0;
    uint8_t dataIn = 0;
    if (lookup[errorCheck] == -1) {
        return HAM_ERR;
    } else if (lookup[errorCheck] == -2) {
        *data = code;
        return HAM_OK;
    } else if (lookup[errorCheck] >= 0) {
        int bitToChange = lookup[errorCheck];
        for (int b = 0; b < (int) bm_cols(D); b++) {
            if (b == bitToChange) {
                if (bm_get_bit(D, 0, b) == 1) {
                    bm_clr_bit(D, 0, b);
                } else {
                    bm_set_bit(D, 0, b);
                }
            }
            bitForInt = bm_get_bit(D, 0, b);
            dataIn |= bitForInt << b;
        }
        *data = dataIn;
        return HAM_ERR_OK;
    }
    return HAM_ERR;
}
