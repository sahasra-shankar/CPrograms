#include "stack.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//referred to Sahiti's logic from section
struct Stack {
    uint32_t top;
    uint32_t capacity;
    int64_t *items;
};

Stack *stack_create(void) {
    Stack *s = (Stack *) calloc(1, sizeof(Stack));
    if (s == NULL) {
        return NULL;
    }
    s->top = 0;
    s->capacity = MIN_CAPACITY;
    s->items = (int64_t *) calloc(s->capacity, sizeof(int64_t));
    if (s->items == NULL) {
        free(s);
        return NULL;
    }
    return s;
}

void stack_delete(Stack **s) //deallocates memory-- based off of bm delete
{
    free((*s)->items);
    free(*s);
    *s = NULL;
}

bool stack_empty(Stack *s) {
    return s->top == 0;
}

bool stack_push(Stack *s, int64_t x) {
    if (s->top == s->capacity) {
        s->capacity = 2 * s->capacity;
        s->items = (int64_t *) realloc(s->items, s->capacity * sizeof(int64_t));
        if (s->items == NULL) {
            return false;
        }
    }
    s->items[s->top] = x;
    s->top += 1;
    return true;
}

bool stack_pop(Stack *s, int64_t *x) {
    if (s->top == 0) {
        return false;
    }
    s->top -= 1;
    *x = s->items[s->top];
    return true;
}

void stack_print(Stack *s) //debug function-->double check this function
{
    for (uint32_t p = 0; p < s->top; p++) {
        //stack_pop(s, &val_at_top);
        printf("%ld ", s->items[s->top]);
    }
    printf("\n");
}
