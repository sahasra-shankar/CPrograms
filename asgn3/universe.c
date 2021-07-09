#include "universe.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct Universe {
    int rows;
    int cols;
    bool **grid;
    bool toroidal;
};

bool inbounds(Universe *u, int r,
    int c) // went with Eugene's idea of having a helper function to
// check if r,c is within bounds
{
    if (u->toroidal != true) {
        //printf("INBOUND CHECK for FALSE Toroid: cell %d, %d\n", r, c);
        if (r >= u->rows || r < 0 || c >= u->cols || c < 0) {
            //printf("cell %d, %d OUT OF BOUNDS\n", r, c);
            return false;
        } else {
            //printf("INBOUND: NON-TOROID cell %d, %d WITHIN bound\n", r, c);
            return true;
        }
    } else {
        //printf("Inbound: cell %d, %d WITHIN BOUNDS\n", r, c);
        return true;
    }
    return false;
}

Universe *uv_create(int rows, int cols,
    bool toroidal) // referenced Sahithi's code
{
    //printf("Rows: Cols -> %d, %d\n", rows, cols);
    Universe *u = (Universe *) calloc(1, sizeof(Universe));
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;
    u->grid = (bool **) calloc(rows, sizeof(bool *));
    for (int i = 0; i < rows; i++) {
        u->grid[i] = (bool *) calloc(cols, sizeof(bool));
    }
    return u;
}

void uv_delete(Universe *u) // void value not ignored as it ought to be
{
    for (int r = 0; r < u->rows; r++) // referred to Sahithi's logic
    {
        free(u->grid[r]);
    }
    free(u->grid);
    free(u);
    return;
}

int uv_rows(Universe *u) {
    return u->rows;
}

int uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, int r, int c) // referenced Eugene's logic
{
    if (u->toroidal == true) {
        //printf("LIVE_CELL: Toroidal %d, %d\n", r, c);
        //if (uv_get_cell(u, r, c) == true || uv_get_cell(u, r, c) == false) {
        if (r >= uv_rows(u) || r < 0) {
            r = (r + uv_rows(u))
                % uv_rows(u); //mod was throwing major error bc forgot C doesnt follow pemdas
            //printf("Cell Row Val: %d\n", r);
        }
        if (c >= uv_cols(u) || c < 0) {
            c = (c + uv_cols(u)) % uv_cols(u);
            //printf("Cell Col Val: %d\n", c);
        }
        //printf("Cell Row, Col: %d, %d\n", r, c);
        u->grid[r][c] = true;
        //}
    } else if (u->toroidal == false) {
        //printf("LIVE_CELL: for TOROID check FALSE- cell %d, %d set TRUE\n", r, c);
        if (inbounds(u, r, c) == true) {
            //printf("UV_LIVE_CELL: setting row col to true %d, %d \n", r, c);
            u->grid[r][c] = true;
            //printf("UV_LIVE_CELL:  uv_get_cell %d\n", uv_get_cell(u,r,c));
        }
    }
}

void uv_dead_cell(Universe *u, int r, int c) // referenced Eugene's logic
{

    if (u->toroidal == true) {
        if (uv_get_cell(u, r, c) == true || uv_get_cell(u, r, c) == false) {
            if (r >= uv_rows(u) || r < 0) {
                r = (r + uv_rows(u)) % uv_rows(u);
            }
            if (c >= uv_cols(u) || c < 0) {
                c = (c + uv_cols(u)) % uv_cols(u);
            }
            u->grid[r][c] = false;
        }
    }
    if (u->toroidal == false) {
        if (inbounds(u, r, c) == true) {
            u->grid[r][c] = false;
        }
    }
}

bool uv_get_cell(Universe *u, int r, int c) {
    // printf("get cell function entered\n");
    if (u->toroidal == true) {
        r = (r + uv_rows(u)) % uv_rows(u);
        c = (c + uv_cols(u)) % uv_cols(u);
        return u->grid[r][c];
    }
    if (u->toroidal == false) {
        // printf("TOROIDAL is FALSE enter\n");
        if (inbounds(u, r, c) == true) {
            // printf("inbounds is true\n");
            return u->grid[r][c];
        } else if (inbounds(u, r, c) == false) {
            // printf("inbounds is false\n");
            return false;
        }
        // printf("value of cell is %d\n", u->grid[r][c]);
    }

    return false;
}

bool uv_populate(Universe *u,
    FILE *infile) // uses counts from census to set each cell to dead/alive
{
    //printf("POPULATE ENTERED\n");
    int rows = 0;
    int cols = 0;
    //char r, c;
    bool returnStatus = true;
    while (!feof(infile)) { //as long as infile != EOF
        fscanf(infile, "%d %d\n", &rows, &cols);
        //printf("in Scan POPULATE: cell %d, %d\n", rows, cols);
        //if(!isdigit(rows) || !isdigit(cols))
        //{
        //  return false;
        //}
        //{ printf("Digit Check : Malformed %s, %s : %d , %d \n", &r, &c, isdigit(r), isdigit(c)); returnStatus = false; break;}
        if (u->toroidal == false) {
            //printf("UV_POPULATE: Non-Toroidal: Cell %d, %d entered \n",rows, cols);
            if (!inbounds(u, rows, cols)) {
                //printf("Inbounds Check: Malformed input\n");
                //check if condition if inbounds == false, print malformed in mv_print works
                returnStatus = false;
                break;
            }
        }
        uv_live_cell(u, rows, cols);
        //printf("UV_POPULATE: set cell rows , cols -> %d, %d\n", rows, cols);
    }
    return returnStatus;
}

int uv_census(Universe *u, int r, int c) {
    int count = 0;
    if (u->toroidal == false) {
        for (int begRow = MAX(0, (r - 1)), rowBound = MIN((r + 2), u->rows); begRow < rowBound;
             begRow++) {
            for (int begCol = MAX(0, (c - 1)), colBound = MIN((c + 2), u->cols); begCol < colBound;
                 begCol++) {
                // printf(" UV_CENSUS: R:C %d:%d  and status : %d\n", begRow, begCol,
                // uv_get_cell(u,begRow,begCol));
                if (begRow != r || begCol != c) {
                    if (uv_get_cell(u, begRow, begCol) == true) {
                        count++;
                    }
                }
            }
        }
        // printf(" UV_CENSUS: number of live cell neighbors for %d, %d : %d\n", r, c, count);
    }
    if (u->toroidal == true) {
        // printf("TOROIDAL enters for TRUE\n");
        for (int firstRow = r - 1; firstRow <= r + 1; firstRow++) {

            for (int firstCol = c - 1; firstCol <= c + 1; firstCol++) {
                // printf("Toroidal check loop r: c -> %d, %d\n", firstRow, firstCol);
                int checkRow
                    = ((firstRow < 0)
                            ? (uv_rows(u) - 1)
                            : (firstRow >= uv_rows(u)
                                    ? (firstRow % uv_rows(u))
                                    : firstRow)); //if first row < 0, then do rows-1, and if first row>rows then do firstrow % rows; else continue with row specified
                int checkCol
                    = ((firstCol < 0) ? (uv_cols(u) - 1)
                                      : (firstCol >= uv_cols(u) ? (firstCol % uv_cols(u))
                                                                : firstCol)); //same logic as rows
                if (checkRow != r || checkCol != c) {
                    // printf("Neighbor Check for Toroidal True for cell %d, %d\n",
                    // checkRow, checkCol);
                    if (uv_get_cell(u, checkRow, checkCol) == true) {
                        // printf("getting neighbor count for TOROID\n");
                        count++;
                    }
                }
            }
        }

        // printf(" UV_CENSUS: number of live cell neighbors for %d, %d : %d\n", r,c, count);
    }
    return count;
}

void uv_print(Universe *u, FILE *outfile) {
    //printf(" universe uv_print %d \n", uv_rows(u));
    for (int i = 0; i < u->rows; i++) {
        for (int j = 0; j < u->cols; j++) {
            //printf(" universe cells %d, %d\n", i, j);
            if (u->grid[i][j] == true) {
                //printf(" in TRUE\n");
                fputc('o', outfile);
            } else if (u->grid[i][j] == false) {
                fputc('.', outfile);
            }
        }
        fprintf(outfile, "\n");
    }
}
