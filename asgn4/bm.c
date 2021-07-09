#include "bm.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    uint8_t **mat;
};

uint32_t bytes(uint32_t bits) {
    if ((bits % 8) == 0) {
        return bits / 8;
    } else {
        return (bits / 8) + 1;
    }
}

BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *) calloc(1, sizeof(BitMatrix));
    if (m == NULL) {
        return NULL;
    }
    m->rows = rows;
    m->cols = cols;
    // m->mat = (BitMatrix **) calloc(rows, sizeof(uint8_t *));
    m->mat = (uint8_t **) calloc(rows, sizeof(uint8_t *));
    if (m->mat == NULL) {
        free(m);
        return NULL;
    }
    int r = 0;
    int totalrows = (int) rows;
    int fail = 0;
    for (; r < totalrows; r++) {
        m->mat[r] = (uint8_t *) calloc(bytes(cols), sizeof(uint8_t));
        if (m->mat[r] == NULL) {
            fail = 1;
            break;
        }
    }
    if (fail == 1) {
        for (int d = 0; d < r; d++) {
            free(m->mat[d]);
        }
        free(m->mat);
        free(m);
        return NULL;
    }

    return m;
}

void bm_delete(BitMatrix **m) {
    for (uint32_t n = 0; n < (*m)->rows; n++) {
        free((*m)->mat[n]);
    }
    free((*m)->mat);
    free(*m);
    *m = NULL;
    return; // remove return bc void function?
}

uint32_t bm_rows(BitMatrix *m) {
    return m->rows;
}

uint32_t bm_cols(BitMatrix *m) {
    return m->cols;
}

void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint8_t mask = 1 << c % 8;
    m->mat[r][c / 8] = m->mat[r][c / 8] | mask;
}

void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint8_t mask = ~(1 << c % 8);
    m->mat[r][c / 8] = (m->mat[r][c / 8]) & mask;
}

uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint8_t mask = 1 << (c % 8);
    return (m->mat[r][c / 8] & mask) >> (c % 8);
}
