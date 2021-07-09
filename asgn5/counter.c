#include "counter.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t eCount = 0;
uint32_t mCount = 0;

bool EVAL(uint32_t *i, uint32_t *j) {
    eCount++;
    //printf("EVAL COUNT is now: %d\n", eCount);
    if (*i < *j == true) {
        //printf("COMPARE: heap1= %d and heap2= %d \n", *i, *j);
        return true;
    } else {
        return false;
    }
}

void SWAP(uint32_t *i, uint32_t *j) {
    //printf("SWAP: heap1= %d and heap2= %d \n", *i, *j);
    int temp;
    temp = *i;
    *i = *j;
    *j = temp;
    mCount += 3;
}
