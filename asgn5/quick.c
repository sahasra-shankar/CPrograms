#include "quick.h"

#include "counter.h"
#include "stack.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//referred to quick pseudocode provided in asgn doc
int64_t partition(uint32_t *A, int64_t lo,
    int64_t hi) //determines if an element is greater or less than pivot val
{
    uint32_t pivot = A[lo + ((hi - lo) / 2)];
    int64_t b = lo - 1;
    int64_t c = hi + 1;
    do {
        b++;
        while (EVAL(&A[b], &pivot) == true) {
            //printf("QUICK(for A[b] and pivot): COMPARISON for %d and %d\n", A[b], pivot);
            b++;
        }
        c--;
        while (EVAL(&pivot, &A[c]) == true) {
            //printf("QUICK(pivot and A[c]): COMPARISON for %d and %d\n", pivot, A[c]);
            c--;
        }
        if (b < c) {
            //printf("QUICK: SWAP for A[b]=%d and A[c]=%d\n", A[b], A[c]);
            //printf("QUICK: address A[b]= %p and A[c]= %p \n", &A[b], &A[c]);
            SWAP(&A[b], &A[c]); //swaps according to relation w pivot
        }
    } while (b < c);
    //printf("******QUICK PARTITION: EXITS DO-WHILE LOOP*******\n");
    return c;
}

void quick_sort(
    uint32_t *A, uint32_t n) //uses stack to "remember" values at right and left in array
{
    int64_t high;
    int64_t low;
    int64_t left = 0;
    int64_t right = n - 1;
    Stack *Q = stack_create();
    if (stack_push(Q, left) == true && stack_push(Q, right) == true) {
        //printf("QUICK: PUSHING left= %ld and right= %ld \n", left, right);
        //printf("QUICK: PUSHING address LEFT %p and RIGHT %p \n", &left, &right);
        while (stack_empty(Q) != true) //n != 0
        {
            if (stack_pop(Q, &high) == true && stack_pop(Q, &low) == true) {
                //printf("QUICK: stack_pop %ld\n", high);
                int64_t p = partition(A, low, high);
                if (p + 1 < high) {
                    stack_push(Q, (p + 1));
                    stack_push(Q, high);
                }
                if (low < p) {
                    stack_push(Q, low);
                    stack_push(Q, p);
                }
            }
        }
        //printf("*******QUICK MAIN: WHILE LOOP EXITED*******\n");
        stack_delete(&Q);
    }
}
