#include "bv.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv == NULL) {
        printf("BIT VECTOR CREATE: BV IS NULL entered***\n");
        return NULL;
    }
    bv->length = length;
    bv->vector = (uint8_t *) calloc((length / 8), sizeof(uint8_t));
    if (bv->vector == NULL) {
        printf("BIT VECTOR CREATE: VECTOR IS NULL is entered**\n");
        free(bv);
        return NULL;
    }
    return bv;
}

void bv_delete(BitVector **bv) {
    free((*bv)->vector);
    free(*bv);
    *bv = NULL;
}

uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

void bv_set_bit(BitVector *bv, uint32_t i) {
    uint8_t mask = 1 << (i % 8);
    bv->vector[i / 8] = bv->vector[i / 8] | mask;
}

void bv_clr_bit(BitVector *bv, uint32_t i) {
    uint8_t mask = ~(1 << (i % 8));
    bv->vector[i / 8] = bv->vector[i / 8] & mask;
}

uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
    uint8_t mask = 1 << (i % 8);
    return (bv->vector[i / 8] & mask) >> (i % 8);
}

void bv_print(BitVector *bv) {
    int setBitCount = 0;
    for (uint32_t i = 0; i < bv_length(bv); i++) {
        printf("%d ", bv_get_bit(bv, i));
        if (bv_get_bit(bv, i) == 1) {
            printf("BIT SET at i = %d\n", i);
            setBitCount++;
        }
    }
    printf("\n");
    //printf("The total number of set bits = %d\n", setBitCount);
}
