#include "heap.h"

#include "counter.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int get_max_child(
    uint32_t *A, int first, int last) //referenced heap pseudocode provided in asgn doc
{
    //printf("ENTERED max child\n");
    int left_child = 2 * first;
    int right_child = left_child + 1;
    if (EVAL(&A[left_child - 1], &A[left_child]) == true && right_child <= last) {
        return right_child;
    }
    return left_child;
    //printf("after getting max child\n");
}

void make_heap(uint32_t *A, int first, int last) {
    bool found = false;
    int parent = first;
    int great = get_max_child(A, parent, last);
    while (parent <= last / 2 && !found) //try found and not found
    {
        if (EVAL(&A[parent - 1], &A[great - 1]) == true) {
            SWAP(&A[parent - 1], &A[great - 1]);
            parent = great;
            great = get_max_child(A, parent, last);
        } else {
            found = true;
        }
    }
    //printf(" after make heap\n");
}

void build_heap(uint32_t *A, int first, int last) {
    //printf("ENTERED  build heap\n");
    for (int p = last / 2; p > first - 1; p--) {
        make_heap(A, p, last);
    }
    //printf(" after build heap\n");
}

void heap_sort(uint32_t *A, uint32_t n) {
    int first = 1;
    int last = n;
    build_heap(A, first, last);
    for (int n = last; n > first; n--) {
        SWAP(&A[first - 1], &A[n - 1]);
        make_heap(A, first, (n - 1));
    }
    //printf("after main heap sort\n");
}
