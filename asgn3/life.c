#include "universe.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "tsn:i:o:"
#define DELAY   50000

int main(int argc, char *argv[]) {
    FILE *infile = stdin;
    FILE *outfile = stdout;
    int opt = 0;
    bool toroidal, silenceNC = false;
    int generations = 100;
    char *univfile, *ofile;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 's': silenceNC = true; break;
        case 'n':
            generations = atoi(optarg);
            //printf("optarg generation %d\n",generations);
            break;
        case 'i':
            univfile = malloc(strlen(optarg));
            strcpy(univfile,
                optarg); //allocates memory to file ptr based on optarg len and copies file to infile ptr
            infile = fopen(univfile, "r");
            //infile = optarg;
            printf("optarg infile %s\n", optarg);
            if (infile == NULL) {
                fprintf(stderr, "Error- try with valid input\n");
                exit(0);
            }
            break;
        case 'o':
            ofile = malloc(strlen(optarg));
            strcpy(ofile, optarg); //allocates memory to file ptr for given optarg len
            //printf(" Outfile found %s\n ", ofile);          //and copies specified optarg to output file
            if (strcmp(ofile, "stdout")
                != 0) { //when outfile name does not match default- opens specified file to write to
                //printf(" Outfile found\n ");
                outfile = fopen(optarg, "w");
                if (outfile == NULL) {
                    fprintf(stderr, "Error- specify outfile\n");
                    exit(0);
                }
            }
            break;
        default: fprintf(stderr, "Usage %s -[tsn:i:o]\n", argv[0]);
        }
    }
    //printf("checking infile and outfile %s and %s \n", infile, outfile);
    //getchar();
    int rows = 0;
    int cols = 0;
    bool populateStatus = true;
    fscanf(infile, "%d %d\n", &rows, &cols);
    Universe *A = uv_create(rows, cols, toroidal);
    Universe *B = uv_create(rows, cols, toroidal);
    //printf("created\n");
    populateStatus = uv_populate(A, infile);
    if (!populateStatus) {
        printf("Malformed input.\n");
        exit(0);
    }
    //printf("pop called\n");
    //uv_print(A, stdout);
    if (silenceNC == false) {
        initscr();
        curs_set(FALSE);
    }
    for (int n = 0; n < generations; n++) // for generations = 100
    {
        // printf("entered gen\n");
        for (int d = 0; d < uv_rows(A); d++) // go through each cell
        {
            // printf("entered row loop: %d\n", d);
            for (int k = 0; k < uv_cols(A); k++) {
                // printf("entered column loop: %d\n", k);
                // printf("val of %d,%d: %d\n", d, k, uv_get_cell(A, d, k));
                if (uv_get_cell(A, d, k) == true) {
                    if (uv_census(A, d, k) == 2 || uv_census(A, d, k) == 3) {
                        // printf("entered census 1 for get is true\n");
                        uv_live_cell(B, d, k);
                    } else {
                        uv_dead_cell(B, d, k);
                    }
                    // printf("exits if get  is true\n");
                } else if (uv_get_cell(A, d, k) == false) {
                    if (uv_census(A, d, k) == 3) {
                        // printf("entered census 1 for get is false\n");
                        uv_live_cell(B, d, k);
                    } else {
                        uv_dead_cell(B, d, k);
                    }
                    // printf("goes in else if get is false\n");
                }
                // printf("exits if get is false\n");
            }
        }
        Universe *C = A;
        A = B;
        B = C;
        // printf("swap complete\n");
        //
        if (silenceNC == false) {
            clear();
            for (int i = 0; i < uv_rows(A); i++) {
                for (int j = 0; j < uv_cols(A); j++) {
                    if (uv_get_cell(A, i, j) == true) {
                        mvprintw(i, j, "o");
                    }
                }
            }
            refresh();
            usleep(DELAY);
        }
    }
    fclose(infile);
    if (silenceNC == false) {
        /* printf(" NCURSES enabled");
       usleep(DELAY);*/
        endwin(); //in working ver
    } //in working ver
    uv_print(A, outfile); // could be put outside loop bc nonspecific for silenceNC
    uv_delete(A);
    uv_delete(B);

    if (strcmp(ofile, "stdout") != 0) {
        fclose(outfile);
    }
    //}
    return 0;
}
