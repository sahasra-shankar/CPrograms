#include "hash.h"

#include "ll.h"
#include "node.h"
#include "speck.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    bool mtf;
    LinkedList **lists;
};

HashTable *ht_create(uint32_t size, bool mtf) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht == NULL) {
        return NULL;
    }
    //printf("Hash Table created\n");
    if (ht) {
        ht->salt[0] = 0x85ae998311115ae3;
        ht->salt[1] = 0xb6fac2ae33a40089;
        ht->size = size;
        ht->mtf = mtf;
        ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    for (uint32_t i = 0; i < size; i += 1) {
        ht->lists[i] = NULL;
    }
    //printf("out of create\n");
    return ht;
}

void ht_delete(HashTable **ht) //verify correct pointer ref
{
    //printf("HT: hashtable size %d\n", ht_size(*ht));
    for (uint32_t i = 0; i < ht_size(*ht); i++) {
        //LinkedList *temp = (*ht)->lists[i];
        //printf("HT: DELETE-- ENTERS FOR LOOP ITERATION THROUGH HT\n");
        //if((*ht)->lists[i] != NULL)
        if ((*ht)->lists[i]) {
            //printf("HT: DELETE--ENTERS *if* LINKED LIST IS NOT EMPTY AT i = %d\n", i);
            ll_delete(&((*ht)->lists[i]));
            //printf("HT: DELETE-- DELETES LIST AT HASH TABLE[%d]\n", i);
        }
        //printf("HT: DELETE-- COMES OUT OF IF NOT EMPTY CONDITION\n");
    }
    //printf("HT: DELETE--COMES OUT OF INDEXING THROUGH HT\n");
    free((*ht)->lists); //added @11am 2/27
    free(*ht);
    //printf("HT: DELETE-- DELETES HASH TABLE\n");
    *ht = NULL;
    //printf("HT: DELETE-- Hash Table deleted after SETTING ht=NULL#########\n");
}

uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

Node *ht_lookup(HashTable *ht, char *oldspeak) //incomplete
{
    LinkedList *s = ht->lists[hash(ht->salt, oldspeak) % ht_size(ht)];
    //printf(" ht_lookup *****\n");
    if (s == NULL) {
        //printf("HASH LOOKUP: INSIDE Linked List S = NULL\n");
        return NULL;
    }
    //printf("HASH LOOKUP: s is at key %d\n", hash(ht->salt, oldspeak)%ht_size(ht));
    //printf("The list at key %d is s:\n", hash(ht->salt, oldspeak)%ht_size(ht));
    //ll_print(s);
    Node *lNode = ll_lookup(s, oldspeak);
    //printf("HASH: LOOKUP Result = %s", lNode->oldspeak);
    //return ll_lookup(s, oldspeak);
    return lNode;
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    uint32_t hashKey = (hash(ht->salt, oldspeak) % ht_size(ht));
    //printf("HASH: ****ENTERED HASH INSERT**** %s -> %s and htsize => %d \n", oldspeak, newspeak, ht_size(ht));
    //printf("HASH: HT key ***************************************************** %d \n", hashKey);
    //LinkedList *k = NULL;
    if (ht->lists[hashKey] == NULL) {
        //printf("HASH: INSERT when LL does NOT exist\n");
        //k = ll_create(ht->mtf);
        ht->lists[hashKey] = ll_create(ht->mtf);
        //printf("HASH: INSERT LinkedList k CREATED with key %d \n", hashKey);
    }
    /*else{
	printf("HASH: INSERT ENTERED FOR when LL EXISTS\n");
        k = ht->lists[hashKey];
	printf("HASH: Linkedlist k is initialized \n");
	ll_length(ht->lists[hashKey]);
	printf("HASH: LINKED LIST K SET TO HT->LISTS[key]\n");
    }*/
    ll_insert(ht->lists[hashKey], oldspeak, newspeak); //verify what to refer to ht as
    //printf("HASH: INSERT Linked List Inserted at ***************************** %d \n", hashKey);
}

void ht_print(HashTable *ht) {
    //printf("***********HASH: ENTERED PRINT FUNCTION*************\n");
    //printf("***********HASH: SIZE %d *************\n", ht_size(ht));
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        LinkedList *p = ht->lists[i];
        if (p != NULL) {
            printf(" HT: KEY ** %d ** ==> ", i);
            ll_print(p);
        }
        //printf("HASH: PRINT-- LinkedList p at %d", i);
    }
}
