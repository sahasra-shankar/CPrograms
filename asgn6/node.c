#include "node.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*struct Node {
    char *oldspeak;
    char *newspeak;
    Node *next;
    Node *prev;
};*/

//create strdup function later
char *StrCopy(char *str) {
    return strcpy(malloc(strlen(str) + 1), str);
}

Node *node_create(char *oldspeak, char *newspeak) {
    //printf("NODE: NODE_CREATE ENTERED***\n");
    Node *n = (Node *) malloc(sizeof(Node));
    if (n == NULL) {
        //printf("NODE: MALLOC FAILED FOR NODE\n");
        return NULL;
    }
    n->oldspeak = oldspeak ? StrCopy(oldspeak) : NULL;
    n->newspeak = newspeak ? StrCopy(newspeak) : NULL;
    n->next = NULL;
    n->prev = NULL;
    //printf("NODE: NODE CREATED-- EXITING*******\n");
    return n;
}

void node_delete(Node **n) //need to check
{
    free((*n)->oldspeak);
    free((*n)->newspeak);
    free(*n);
    *n = NULL;
}

void node_print(Node *n) {
    if (n->oldspeak && n->newspeak != NULL) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    }
    if (n->oldspeak != NULL && n->newspeak == NULL) {
        printf("%s\n", n->oldspeak);
    }
}
