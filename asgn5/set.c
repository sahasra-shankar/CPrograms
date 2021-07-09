#include "set.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//referred to Sahiti's logic from section

Set set_empty(void) {
    return 0;
}

bool set_member(Set s, uint8_t x) //same as get bit
{
    uint32_t mask = 1 << x % 32;
    return (s & mask) >> (x % 32);
}

Set set_insert(Set s, uint8_t x) //same as set bit
{
    uint32_t mask = 1 << x % 32;
    return s | mask;
}

Set set_remove(Set s, uint8_t x) //same as clr bit
{
    uint32_t mask = ~(1 << x % 32);
    return s & mask;
}

Set set_intersect(Set s, Set t) {
    return s & t;
}

Set set_union(Set s, Set t) {
    return s | t;
}

Set set_complement(Set s) {
    return ~s;
}

Set set_difference(Set s, Set t) {
    return s & (~t);
}
