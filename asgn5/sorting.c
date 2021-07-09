#include "bubble.h"
#include "counter.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stack.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "absqhr:n:p:"

void fill_array(uint32_t *A, int n, int seed) //referred to Eugene's logic
{
    srandom(seed);
    uint32_t hexMask = 0x3FFFFFFF;
    for (int r = 0; r < n; r++) {
        A[r] = random() & hexMask;
        //printf("%d ", A[r]);
    }
}

void print_stats(char *sort_name, int elemout, int n, uint32_t *A) {
    printf("%s\n", sort_name);
    printf("%d elements, %d moves, %d compares\n", n, mCount, eCount);
    if (elemout > 0) {
        int i;
        for (i = 0; i < elemout; i++) {
            printf("%13" PRIu32, A[i]);
            if ((i + 1) % 5 == 0) //separates into 5 cols
            {
                printf("\n");
            }
        }
        if (i % 5 > 0) //print nl if last element%5 !=0
            printf("\n");
    }
}
enum Sorting //to use with set--sets bit to corresponding sort number
{ bubble = 0,
    shell = 1,
    quick = 2,
    heap = 3 };

int main(int argc, char *argv[]) {
    Set sorting_set = set_empty();

    char *sorting_names[4] = { "Bubble Sort", "Shell Sort", "Quick Sort", "Heap Sort" };

    int opt = 0;
    int elements = 100;
    int size = 100;
    int seed = 7092016; //might change to uint32_t
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': //use set complement w/ empty to set all bits and get universal set
            sorting_set = set_complement(set_empty());
            break;
        case 'b': sorting_set = set_insert(sorting_set, bubble); break;
        case 's': sorting_set = set_insert(sorting_set, shell); break;
        case 'q': sorting_set = set_insert(sorting_set, quick); break;
        case 'h': sorting_set = set_insert(sorting_set, heap); break;
        case 'r': seed = atoi(optarg); break;
        case 'n':
            size = atoi(optarg); //change atoi to strtoul
            break;
        case 'p': elements = atoi(optarg); break;
        default: fprintf(stderr, "Usage %s -[absqhr:n:p:]\n", argv[0]);
        }
    }
    if (elements > size) //limits printed elements to only the number of elements within array
    {
        elements = size;
    }
    /*uint32_t *A = (uint32_t*)malloc(size * sizeof(uint32_t)); 
    fill_array(A, size, seed);
    heap_sort(A, size);
    printf("SORTED ARRAY OUTPUTTED\n");
    for (int h = 0; h<size; h++)
    {
        printf("%16d \n", A[h]);
    }
    printf("Number of Swaps %d \n", mCount);
    printf("Number of Comparisons %d \n", eCount);
    free(A);*/
    //took Eugene's advice on implementing a function pointer table
    void (*sortfns_table[4])(
        uint32_t *, uint32_t); //matches set bit to corresponding sort to call from for loop
    sortfns_table[0] = bubble_sort;
    sortfns_table[1] = shell_sort;
    sortfns_table[2] = quick_sort;
    sortfns_table[3] = heap_sort;
    uint32_t *A = (uint32_t *) malloc(size * sizeof(uint32_t));
    for (int i = bubble; i <= heap; i++) {
        //try using function ptrs. if not use switch case
        if (set_member(sorting_set, i)) {
            eCount = 0;
            mCount = 0;
            fill_array(A, size, seed); //resets array to unsorted
            sortfns_table[i](A, size);
            print_stats(sorting_names[i], elements, size, A);
        }
    }
    free(A);
    return 0;
}
