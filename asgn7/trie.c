#include "trie.h"

#include "code.h"
#include "endian.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

TrieNode *trie_node_create(uint16_t code) {
    TrieNode *n = (TrieNode *) calloc(1, sizeof(TrieNode));
    //TrieNode *n = (TrieNode *) calloc(sizeof(TrieNode));
    if (n == NULL) {
        return NULL;
    }
    for (int i = 0; i < ALPHABET; i++) {
        n->children[i] = NULL;
    }
    n->code = code;
    return n;
}

void trie_node_delete(TrieNode *n) {
    free(n);
    //n = NULL;
}

TrieNode *trie_create(void) {
    TrieNode *root = trie_node_create(EMPTY_CODE);
    if (root == NULL) {
        return NULL;
    }
    return root;
}

void trie_reset(TrieNode *root) {
    for (int i = 0; i < ALPHABET; i++) //need to check this condition
    {
        if (root->children[i]) {
            trie_delete(root->children[i]);
            root->children[i] = NULL;
        }
    }
}

void trie_delete(TrieNode *n) {
    /*if(n != NULL)
    {
        for(int i = 0; i < ALPHABET; i++) //need to check this condition
	{
            if(n->children[i] != NULL)
	    {
	        printf(" trie_delete: for %d \n", n->children[i]->code);
	        trie_delete(n->children[i]);
		n->children[i] = NULL;
	    }
	}
	printf("trie_delete: no children node delete attemp for %d \n", n->code);
	//trie_node_delete(n); free(n)
	free(n);
	n = NULL; // not sure if this is n->children
	printf("trie_delete: checking n->code val %d \n", n->code);
	if(n==NULL)
	    printf(" after setting NULL current node is set to NULL\n");
    }*/

    if (n != NULL) {
        for (int i = 0; i < ALPHABET; i++) {
            if (n->children[i] != NULL) {
                trie_delete(n->children[i]);
                n->children[i] = NULL;
            }
        }
        trie_node_delete(n);
        n = NULL;
    }
}

TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    return (n->children[sym]);
}

/* int main(void)
{
    TrieNode *root = trie_create();
    root->children[0] = trie_node_create(0);
    root->children[0]->children[4] = trie_node_create(4);
    root->children[1] = trie_node_create(1);
    root->children[2] = trie_node_create(2);
    root->children[2]->children[3] = trie_node_create(3);
    root->children[2]->children[3]->children[5] = trie_node_create(5);
    root->children[2]->children[3]->children[7] = trie_node_create(7);
    root->children[6] = trie_node_create(6);

    TrieNode *t = trie_step(root, 3);
    if(t != NULL)
        printf("%d\n", t->code);
    else
	printf(" find node(3) under root is NULL\n");

    trie_reset(root);
    //trie_step(trie_step(root,2),3)->children[5]=NULL;
    TrieNode *g = trie_step(root, 2);
    if(g == NULL)
        printf(" After tnode_delete(2->3) under node root, is NULL\n");
    for(int i = 0; i< ALPHABET; i++)
    {
        if(root->children[i] != NULL)
        {
            printf("A child of root is not null\n");
        }
    } 
    return 0;
} */
