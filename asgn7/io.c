#include "io.h"

#include "code.h"
#include "endian.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//in io.h file:
//BLOCK and MAGIC defined
//global variables: total_syms(count of symbols processed) and total_bits(count of bits processed)

static int bitIndex = 0;
static int symIndex = 0; //not fully sure if sep symIndex is needed
static uint8_t bitBuf[BLOCK] = { 0 }; //need to check initialization
static uint8_t symBuf[BLOCK] = { 0 };
static int end_of_buffer = -1; //end of buffer set after last read
uint64_t total_bits = 0;
uint64_t total_syms = 0;

/* void binaryString(uint16_t n)
{
    do{
        printf("%d", n & 1);
        n = n >> 1;
    } while (n > 0);
    printf("\n");
} */
int numBytes(int bits) //helper func to calculate bytes given bits
{
    int numOfBytes = 0;
    if (bits % 8 == 0) {
        numOfBytes = bits / 8;
    } else if (bits % 8 != 0) {
        numOfBytes = (bits / 8) + 1;
    }
    return numOfBytes;
}

int read_bytes(int infile, uint8_t *buf, int to_read) //tested-- works
{
    int total = 0; //total bytes read
    int bytes = 0; //tracks # read
    while ((bytes = read(infile, buf, to_read)) > 0) {
        total += bytes; //increments bytes read
        to_read -= bytes; //decrements bytes left to read by bytes read
        buf += bytes; //increments buf pointer so that no overwriting(overreading) occurs
        if (to_read == 0) {
            break;
        }
    }
    return total;
}

int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int total = 0;
    int bytes = 0;
    while ((bytes = write(outfile, buf, to_write)) > 0) {
        total += bytes; //increments bytes written
        to_write -= bytes; //decrements bytes left to write by bytes written
        buf += bytes; //increments buf pointer so no overwriting occurs
        if (to_write == 0) {
            break;
        }
    }
    return total;
}

void read_header(int infile, FileHeader *header) {
    //FileHeader *readHeader = read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));
    total_bits += (8 * sizeof(FileHeader));
    read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));
    //if byte order != little endian, swap byte order
    if (big_endian()) {
        header->protection = swap16(header->protection);
        header->magic = swap32(header->magic);
    }
}

void write_header(int outfile, FileHeader *header) {
    //FileHeader *writeHeader = write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
    //write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
    total_bits += (8 * sizeof(FileHeader));
    if (big_endian()) {
        header->protection = swap16(header->protection);
        header->magic = swap32(header->magic);
    }
    write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
}

bool read_sym(int infile, uint8_t *sym) //tested-- works
{
    int n = 0;
    if (symIndex == 0) {
        n = read_bytes(infile, symBuf, BLOCK);
        total_syms += n;
        if (n < BLOCK) {
            end_of_buffer = n;
        }
    }
    if (symIndex == end_of_buffer) {
        return false;
    }
    *sym = symBuf[symIndex];
    symIndex++;
    if (symIndex == BLOCK) {
        symIndex = 0;
    }
    return true;
}

void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) //CHECK!!
{
    total_bits += bitlen + 8;
    if (big_endian()) {
        code = swap16(code);
    }
    if (bitIndex == 0)
        for (int i = 0; i < BLOCK; i++)
            bitBuf[i] = 0;

    //printf( "Code  %d  and Binary ->  ", code );
    //binaryString(code);
    for (int i = 0; i < bitlen; i++) //for code
    {
        //uint16_t codeBM = 1;

        //printf("%" PRIu16 ":", binaryString(codeBM)); ((code >> i) & codeBM) == 1
        if (((code >> i) & 1) == 1) {
            //set bit in bitBuf at bitIndex
            //printf(" Bit Index being set to 1 \n");
            bitBuf[bitIndex / 8] |= (1 << (bitIndex % 8));
        } else {
            //clr bit in bitBuf at bitIndex
            //printf(" Bit Index being set to 0 \n");
            bitBuf[bitIndex / 8] &= ~(1 << (bitIndex % 8));
        }
        bitIndex++;
        if (bitIndex == BLOCK * 8) //BLOCK*8?
        {
            write_bytes(outfile, bitBuf, BLOCK);
            bitIndex = 0;
            for (int i = 0; i < BLOCK; i++)
                bitBuf[i] = 0;
        }
    }
    //printf( " 1st bitbuf index value ");
    //binaryString(bitBuf[0]);

    for (int j = 0; j < 8; j++) //for syms
    {
        //uint8_t symBM = 1 << j;
        if (((sym >> j) & 1) == 1) {
            //set bit in bitBuf at bitIndex
            bitBuf[bitIndex / 8] |= (1 << (bitIndex % 8));
        } else {
            //clr bit in bitBuf at bitIndex
            bitBuf[bitIndex / 8] &= ~(1 << (bitIndex % 8));
        }
        bitIndex++;
        if (bitIndex == BLOCK * 8) {
            write_bytes(outfile, bitBuf, BLOCK);
            bitIndex = 0;
            for (int i = 0; i < BLOCK; i++)
                bitBuf[i] = 0;
        }
    }
}

void flush_pairs(int outfile) {
    if (bitIndex != 0) {
        write_bytes(outfile, bitBuf, numBytes(bitIndex));
    }
}

bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) //CHECK!!
{
    total_bits += bitlen + 8;
    uint16_t tempCode = 0;
    int n = 0;
    for (int i = 0; i < bitlen; i++) {
        uint16_t codeMask = 1;
        if (bitIndex == 0) //if the buffer is empty
        {
            n = read_bytes(infile, bitBuf, BLOCK);
        }
        if (((bitBuf[bitIndex / 8] >> (bitIndex % 8)) & codeMask) == 1) {
            tempCode = tempCode | (codeMask << i);
        } else {
            tempCode = tempCode & ~(codeMask << i);
        }
        bitIndex++;
        if (bitIndex == BLOCK * 8) {
            bitIndex = 0;
            for (int i = 0; i < BLOCK; i++)
                bitBuf[i] = 0;
        }
    }
    *code = tempCode; //return tempCode through code ptr

    //same for symbols
    uint8_t tempSym = 0;
    for (int j = 0; j < 8; j++) {
        if (bitIndex == 0) //if the buffer is empty
        {
            read_bytes(infile, bitBuf, BLOCK);
        }
        uint8_t symMask = 1;
        if (((bitBuf[bitIndex / 8] >> (bitIndex % 8)) & symMask) == 1) {
            tempSym = tempSym | (symMask << j);
        } else {
            tempSym = tempSym & ~(symMask << j);
        }
        bitIndex++;
        if (bitIndex == BLOCK * 8) {
            bitIndex = 0;
            for (int i = 0; i < BLOCK; i++)
                bitBuf[i] = 0;
        }
    }
    *sym = tempSym;
    //printf(" tempcode and tempSym at end of read pair %d and %c \n" , tempCode, tempSym);
    if (big_endian()) //need to verify if cond checked properly
    {
        *code = swap16(*code);
    }

    if (tempCode == STOP_CODE) {
        return false; //no more pairs are left
    } else {
        return true;
    }
}

void write_word(int outfile, Word *w) //need help on!
{
    /*for(uint32_t wordSym = &w->syms[0]; wordSym < &w->syms[w->len]; wordSym++) // *w->len?
    {
        //pointer arith. to access each sym of Word?
	symBuf[symIndex] = wordSym;
	symIndex++;
	if(symIndex == BLOCK)
	{
	    write_bytes(outfile, symBuf, BLOCK);
	    symIndex = 0;
	}
    }*/
    for (uint32_t i = 0; i < w->len; i++) {
        symBuf[symIndex] = w->syms[i];
        symIndex++;
        if (symIndex == BLOCK) {
            write_bytes(outfile, symBuf, BLOCK);
            total_syms += BLOCK;
            symIndex = 0;
            for (int i = 0; i < BLOCK; i++)
                symBuf[i] = 0;
        }
    }
}

void flush_words(int outfile) {
    if (symIndex != 0) {
        total_syms += symIndex;
        write_bytes(outfile, symBuf, symIndex);
    }
}
