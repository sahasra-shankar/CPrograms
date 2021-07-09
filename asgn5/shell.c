#include "shell.h"

#include "counter.h"
#include "gaps.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void shell_sort(uint32_t *A, uint32_t n) //referred to shell pseudocode in asgn doc
{
    for (int i = 0; i < GAPS; i++) {
        uint32_t gap = gaps[i];
        uint32_t k = gap;
        while (k < n) {
            uint32_t j = k;
            //printf("SHELL:VALUE of gap is %d\n", k);
            //printf("SHELL:VALUE of j is %d\n", j);
            uint32_t temp = A[k];
            while (j >= gap && EVAL(&temp, &A[j - gap]) == true) {
                //printf("SHELL: within EVAL WHILE loop-----%d and %d\n", temp, A[j-gap]);
                //printf("SHELL: ADDRESS of temp= %p and A[j-k]= %p\n", &temp, &A[j-gap]);
                SWAP(&A[j], &A[j - gap]);
                j = j - gap;
                //printf("SHELL: NEW VALUE OF j = %d\n", j);
            }
            //printf("********SHELL: EXITED EVAL WHILE LOOP*******\n");
            A[j] = temp;
            k++;
        }
    }
}
