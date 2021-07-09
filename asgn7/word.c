#include "word.h"

#include "code.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Word *word_create(uint8_t *syms, uint32_t len) //works-- i think?
{
    /*Word *temp = (Word *) calloc(1, sizeof(Word));
    if (temp == NULL)
    {
        return NULL;
    }
    temp->syms = (uint8_t *) calloc(len, sizeof(uint8_t)); //check
    if(temp->syms == NULL)
    {
        free(temp);
	return NULL;
    }
    //for(int i = 0; i < len; i++)
    memcpy(&temp->syms[i], syms, sizeof(uint8_t));
    temp->len = len;
    return temp;
    printf("Struct creation of Word successful\n");*/

    Word *temp = (Word *) calloc(1, sizeof(Word));
    if (temp == NULL) {
        return NULL;
    }
    temp->syms = (uint8_t *) calloc(len, sizeof(uint8_t)); //check
    if (temp->syms == NULL) {
        free(temp);
        return NULL;
    }
    memcpy(temp->syms, syms, len);
    temp->len = len;
    // printf("Struct creation of Word successful\n");
    return temp;
}

Word *word_append_sym(Word *w, uint8_t sym) {
    /*uint32_t NW_len = w->len;
    Word *NW = word_create(w->syms, (NW_len + 1));
    NW->syms[NW_len] = sym;*/

    Word *NW = word_create(w->syms, w->len);
    NW->syms = (uint8_t *) realloc(NW->syms, ((w->len + 1) * sizeof(uint8_t)));
    NW->syms[w->len] = sym;
    NW->len = w->len + 1;
    return NW;

    /*printf(" current length of word %d \n", w->len);
    w->syms = (uint8_t *) realloc( w->syms, ((w->len+1) * sizeof(uint8_t)));
    printf(" Step 1 current length of word %d \n", w->len);
    w->syms[w->len] = sym ;
    printf(" step 2 current length of word %d \n", w->len);
    w->len = w->len+1;
    printf("  step 3 current length of word %d \n", w->len);
    return w;*/
}

void word_delete(Word *w) {
    free(w->syms);
    w->syms = NULL;
    free(w);
    w = NULL;
    //printf(" trying to deleted word %d\n", w->len);
}

WordTable *wt_create(void) {
    WordTable *tempTable = (WordTable *) calloc(MAX_CODE, sizeof(Word *));
    if (tempTable == NULL) {
        return NULL;
    }
    //tempTable[0] = word_create(0, 0);
    tempTable[EMPTY_CODE] = (Word *) calloc(1, sizeof(Word));
    //printf("Word: Done Creating WT\n");
    return tempTable;
}

void wt_reset(WordTable *wt) //test & verify
{
    for (int i = START_CODE; i < MAX_CODE; i++) {
        if (wt[i] != NULL) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
}

void wt_delete(WordTable *wt) {
    if (wt != NULL) {
        for (int i = EMPTY_CODE; i < MAX_CODE; i++) {
            if (wt[i] != NULL) {
                word_delete(wt[i]);
                wt[i] = NULL;
            }
        }
        //printf(" word table len of first position %d \n", wt[START_CODE]->len);
        free(wt);
        wt = NULL;
    }
}

/* int main(void)
{
    uint8_t *symbol = (uint8_t *)("Hello");
    //uint8_t *symbol2 = (uint8_t *)("Dream");
    //uint8_t *symbol3 = (uint8_t *)("Boom");
    Word *test = word_create(symbol, strlen(symbol));
    if(word_create != NULL)
    {
      printf("CREATION OF WORD SUCCESSFUL!\n");
      for(int i = 0; i < test->len; i++)
      {
        printf("%d\n", test->syms[i]);
      }
    }
    printf("The len of word is %d\n", test->len);
    word_delete(test);
    test = NULL;
    if(test == NULL)
    {
        printf("Word: TEST has been DELETED\n");
    }
    else {
        printf("The len of word is %d\n", test->len);
    }
    return 0;
} */
