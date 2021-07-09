#include "bf.h"
#include "bv.h"
#include "hash.h"
#include "ll.h"
#include "node.h"
#include "parser.h"
#include "speck.h"

#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WORD    "[a-zA-Z0-9_]+(-|')?[a-zA-Z0-9_]+)*" 
#define OPTIONS "f:h:m"
#define BF_SIZE 1 << 20
#define HT_SIZE 10000

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t bfSize = BF_SIZE; //may change to int
    uint32_t htSize = HT_SIZE;
    bool mtf = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'f': bfSize = atoi(optarg); break;
        case 'h': htSize = atoi(optarg); break;
        case 'm': mtf = true; break;
        default: fprintf(stderr, "Usage %s -[f:h:m]\n", argv[0]);
        }
    }

    BloomFilter *bf = bf_create(bfSize);
    if (!bf) {
        fprintf(stderr, "Failed to create Bloom Filter.\n");
        exit(0);
    }

    HashTable *ht = ht_create(htSize, mtf); //may need if-- if case specified, else:
    if (!ht) {
        fprintf(stderr, "Failed to create Hash Table.\n");
        exit(0);
    }

    FILE *infile = fopen("badspeak.txt", "r");
    char badspeakList[10000];
    while (fscanf(infile, "%s\n", badspeakList) != EOF) {
        //printf("BANHAMMER: IN WHILE-- SCAN BADSPEAK***\n");
        //printf("BANHAMMER: BADSPEAKLIST -- %s ***** \n", badspeakList);
        bf_insert(bf, badspeakList);
        //printf("BANHAMMER: SCANNING BADSPEAK-- INSERTED INTO BF\n");
        ht_insert(ht, badspeakList, NULL);
        //printf("BANHAMMER: SCANNING BADSPEAK-- INSERTED INTO HT\n");
    }
    //printf("BANHAMMER: OUT OF WHILE READING BADSPEAK\n");
    fclose(infile);
    //printf("CLOSES BADSPEAK FILE\n");

    FILE *infile2 = fopen("newspeak.txt", "r");
    char oldspeakList[4000];
    char newspeakList[4000];
    while (fscanf(infile2, "%s %s\n", oldspeakList, newspeakList) != EOF) {
        //printf("BANHAMMER: IN WHILE-- SCAN OLDSPEAK***\n");
        bf_insert(bf, oldspeakList);
        ht_insert(ht, oldspeakList, newspeakList);
    }
    fclose(infile2);

    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }

    char *word = NULL;
    LinkedList *forbidden = ll_create(mtf);
    //printf("BANHAMMER: FORBIDDEN LINKED LIST CREATED\n");
    LinkedList *softForbidden = ll_create(mtf);
    //printf("BANHAMMER: SOFTFORBIDDEN LINKED LIST CREATED\n");

    while ((word = next_word(stdin, &re)) != NULL) {
        //printf("***READING IN USER INPUT\n");
        for (uint32_t i = 0; i < strlen(word); i++) {
            word[i] = tolower(word[i]); //not sure whether this char array or not
        }
        if (bf_probe(bf, word) == true && ht_lookup(ht, word) != NULL) {
            if (ht_lookup(ht, word)->newspeak != NULL) {
                ll_insert(
                    softForbidden, ht_lookup(ht, word)->oldspeak, ht_lookup(ht, word)->newspeak);
            } else if (ht_lookup(ht, word)->newspeak == NULL) {
                ll_insert(forbidden, ht_lookup(ht, word)->oldspeak, NULL);
            }
        }
    }

    if (ll_length(forbidden) != 0 && ll_length(softForbidden) != 0) {
        printf("Dear Comrade,\n");
        printf("You have chosen to use degenerate words that may cause hurt\n");
        printf("feelings or cause your comrades to think unpleasant thoughts.\n");
        printf("This is doubleplus bad. To correct your wrongthink and\n");
        printf("preserve community consensus we will be sending you to joycamp\n");
        printf("administered by Medellin’s Miniluv. Beware of the hippos.\n");
        printf("\n");
        printf("Your errors:\n");
        printf("\n"); //joycamp
        ll_print(forbidden);
        printf("Think of these words on your vacation!\n");
        printf("\n");
        ll_print(softForbidden);
    }

    else if (ll_length(forbidden) != 0 && ll_length(softForbidden) == 0) {
        printf("Dear Comrade,\n");
        printf("You have chosen to use degenerate words that may cause hurt\n");
        printf("feelings or cause your comrades to think unpleasant thoughts.\n");
        printf("This is doubleplus bad. To correct your wrongthink and\n");
        printf("preserve community consensus we will be sending you to joycamp\n");
        printf("administered by Medellin’s Miniluv. Beware of the hippos.\n");
        printf("\n");
        printf("Your errors:\n");
        printf("\n"); //joycamp
        ll_print(forbidden);
    }

    else if (ll_length(forbidden) == 0 && ll_length(softForbidden) != 0) {
        printf("Dear Comrade,\n");
        printf("Submitting your text helps preserve feelings and prevent\n");
        printf("badthink. Some of the words that you used are not goodspeak.\n");
        printf("The list shows how to turn the oldspeak words into newspeak.\n");
        printf("\n"); //counsel
        ll_print(softForbidden);
    }
    //for each oldspeak word in softForbidden linkedlist, write it + translation

    clear_words();
    regfree(&re);
    ll_delete(&forbidden);
    ll_delete(&softForbidden);
    bf_delete(&bf);
    //printf("FINISHES DELETING BF\n");
    ht_delete(&ht);
    //printf("FINISHES DELETING HT\n");
    return 0;
}

/*ht_insert(ht, "hello", "aloha");
    ht_insert(ht, "bye", "ciao");
    ht_insert(ht, "prison", "school");
    ht_insert(ht, "calculus", "statistics");
    ht_insert(ht, "compsci", "ochem");
    ht_insert(ht, "5", "Samoa5");
    ht_insert(ht, "6", "Samoa6");
    ht_insert(ht, "7", "Samoa7");
    ht_insert(ht, "8", "Samoa8");
    ht_insert(ht, "9", "Samoa9");
    ht_insert(ht, "10", "Another10");

    bf_insert(bf, "hello");
    bf_insert(bf, "bye");
    bf_insert(bf, "prison");
    bf_insert(bf, "calculus");
    bf_insert(bf, "compsci");
    bf_insert(bf, "5");
    bf_insert(bf, "6");
    bf_insert(bf, "7");
    bf_insert(bf, "8");
    bf_insert(bf, "9");
    bf_insert(bf, "10");

    char buf[12]; //generating random string values to hash to test hash collision
    for(uint32_t i =0; i<200; i++)
    {
        snprintf(buf, 12, "pre_%d_stuff", i);
	ht_insert(ht, buf, buf);
	//bf_insert(bf, buf);
    }
    node_print(ht_lookup(ht, "hello"));
    //printf("The bit for hello is set to %d\n", bf_probe(bf, "hello"));*/
