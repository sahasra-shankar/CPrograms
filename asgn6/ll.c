#include "ll.h"

#include "node.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct LinkedList {
    uint32_t length;
    Node *head;
    Node *tail;
    bool mtf;
};

LinkedList *ll_create(bool mtf) {
    //printf("LL: LL_CREATE ENTERED****\n");
    LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));
    if (ll == NULL) {
        //printf("LL: LL ENTERS MALLOC FAIL\n");
        return NULL;
    }
    ll->length = 0;
    ll->mtf = mtf;
    ll->head = node_create(NULL, NULL);
    ll->tail = node_create(NULL, NULL);
    ll->head->next = ll->tail;
    ll->tail->prev = ll->head;
    //printf("LL: LL_CREATE-- Linked List Created\n");
    return ll;
}

void ll_delete(LinkedList **ll) //check how to do delete
{
    //printf("LL: DELETE-- IN LL_DELETE~~~~~~~~~~~~~~~~~\n");
    /*ll_print(*ll);
    if((*ll)->head == NULL)
    {
        printf("LL: DELETE-- IN IF (*ll)->head == NULL!!!!!!!!!!!!!!!!!!!!\n"); 
    }*/
    while ((*ll)->head != NULL) //condition not being met
    {
        //printf("LL: DELETE-- IN WHILE LOOP CHECKING IF HEAD NOT NULL\n");
        /*Node *tempN = (*ll)->head;
	(*ll)->head = (*ll)->head->next;
	node_delete(&tempN);*/
        Node *tempN = (*ll)->head->next;
        node_delete(&(*ll)->head);
        (*ll)->head = tempN;
        //printf("LL: DELETE-- COMPLETES NODE DELETE\n");
    }
    free(*ll);
    //printf("LL: DELETE-- FREES LINKED LIST\n");
    *ll = NULL;
}

uint32_t ll_length(LinkedList *ll) {
    return ll->length;
}

Node *ll_lookup(LinkedList *ll, char *oldspeak) {
    //printf("LL: LOOKUP Function ENTERED ****\n");
    //printf("LL LENGTH now = %d\n", ll_length(ll));
    for (Node *n = ll->head->next; n != ll->tail; n = n->next) {
        //printf("LL: LOOKUP For Loop ENTERED ****\n");
        int compVal = strcmp(oldspeak, n->oldspeak);
        //node_print(n);
        //printf("The parameter oldspeak is %s\n", oldspeak);
        //printf("LL: LOOKUP The COMPARE VAL of n->oldspeak and oldspeak is %d\n", compVal);
        if (compVal == 0) {
            //printf("LL: LOOKUP if n->oldspeak MATCHES oldspeak!!!!\n");
            if (ll->mtf) {
                //printf(" entering mtf ***************** \n");
                n->prev->next = n->next;
                n->next->prev = n->prev;
                n->next = ll->head->next;
                n->prev = ll->head;
                ll->head->next->prev = n;
                ll->head->next = n;
            }
            return n;
        }
    }
    return NULL;
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
    //printf("*****LL: INSERT ENTERED****\n");
    if (ll_lookup(ll, oldspeak) == NULL) //need to check if statement correct
    {
        //printf("*******LL: ENTERED FOR NO NODE IN LINKED LIST****\n");
        Node *n = node_create(oldspeak, newspeak);
        n->next = ll->head->next;
        n->prev = ll->head;
        ll->head->next->prev = n;
        ll->head->next = n;
        ll->length += 1;
        //printf("*******LL: CREATES AND ADDS NODE*******\n");
    } else {
        //printf("******LL: ENTERS ELSE-- NODE CONTAINING OLDSPEAK FOUND??\n");
    }
    //printf("*******LL: EXITS FOR LOOP AFTER ADDING NODE****\n");
}

void ll_print(LinkedList *ll) {
    if (ll != NULL) {
        //printf("LL: in LINKED LIST != NULL>>>>>>>>>>>>>>>>>>>> \n");
        Node *n = ll->head->next;
        //printf("LL:ll_print after Node initialized to ll->head->next \n");
        for (uint32_t i = 0; i < ll_length(ll); i++) {
            //printf("LL: before node print\n");
            node_print(n);
            n = n->next;
        }
        printf("\n");
    }
}
