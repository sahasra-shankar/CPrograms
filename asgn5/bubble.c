#include "bubble.h"

#include "counter.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void bubble_sort(uint32_t *A, uint32_t n) //somewhat reference asgn doc
{
    for (uint32_t i = 0; i < n - 1; i++) {
        for (uint32_t j = i + 1; j < n; j++) {
            //EVAL(&A[j], &A[i]);
            if (EVAL(&A[j], &A[i]) == true) {
                SWAP(&A[j], &A[i]);
            }
        }
    }
}
