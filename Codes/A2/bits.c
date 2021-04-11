// bits.c ... functions on bit-strings
// part of signature indexed files
// Bit-strings are arbitrarily long byte arrays
// Least significant bits (LSB) are in array[0]
// Most significant bits (MSB) are in array[nbytes-1]

// Written by John Shepherd, March 2019

#include <assert.h>
#include "defs.h"
#include "bits.h"
#include "page.h"

typedef struct _BitsRep {
	Count  nbits;		  // how many bits
	Count  nbytes;		  // how many bytes in array
	Byte   bitstring[1];  // array of bytes to hold bits
	                      // actual array size is nbytes
} BitsRep;

// create a new Bits object

Bits newBits(int nbits)
{
	Count nbytes = iceil(nbits,8);
	Bits new = malloc(2*sizeof(Count) + nbytes);
	new->nbits = nbits;
	new->nbytes = nbytes;
	memset(&(new->bitstring[0]), 0, nbytes);
	return new;
}

// release memory associated with a Bits object

void freeBits(Bits b)
{
	//TODO
	free(b);
}

// check if the bit at position is 1

Bool bitIsSet(Bits b, int position)
{
	assert(b != NULL);
	assert(0 <= position && position < b->nbits);
	//TODO
	if ( (1 << (position % 8)) & (b->bitstring[position / 8]))
    {
        return TRUE;
    }
	else{
        return FALSE;
	}
}

// check whether one Bits b1 is a subset of Bits b2

Bool isSubset(Bits b1, Bits b2)
{
	assert(b1 != NULL && b2 != NULL);
	assert(b1->nbytes == b2->nbytes);

	//TODO

	for (int i=0; i<b2->nbytes; i++)
    {
	    if ((b2->bitstring[i] | b1->bitstring[i]) != b2->bitstring[i])
	        return FALSE;
    }
	return TRUE;
}

// set the bit at position to 1

void setBit(Bits b, int position)
{
	assert(b != NULL);
	assert(0 <= position && position < b->nbits);
	//TODO

    b->bitstring[position / 8] = b->bitstring[position / 8] | (1 << (position % 8));
}

// set all bits to 1

void setAllBits(Bits b)
{
	assert(b != NULL);
	//TODO
	Byte mask = 255;
	for(int i=0; i<b->nbytes ; i++)
    {
	    b->bitstring[i] = b->bitstring[i] | mask;
    }
}

// set the bit at position to 0

void unsetBit(Bits b, int position)
{
	assert(b != NULL);
	assert(0 <= position && position < b->nbits);
	//TODO
    Count arrayIndex, arrayPosition;
    Byte mask;
    //the byte position of the bit is in
    arrayIndex = position / 8;
    //the position of the bit in that byte array
    arrayPosition = position % 8;
    // make the bit at position is 0 else 1
    mask  = ~(1 << arrayPosition);
    b->bitstring[arrayIndex] = mask & (b->bitstring[arrayIndex]);
}

// set all bits to 0

void unsetAllBits(Bits b)
{
	assert(b != NULL);
	//TODO
    Byte mask = 0;
    for(int i=0; i<b->nbytes ; i++)
    {
        b->bitstring[i] = b->bitstring[i] & mask;
    }
}

// bitwise AND ... b1 = b1 & b2

void andBits(Bits b1, Bits b2)
{
	assert(b1 != NULL && b2 != NULL);
	assert(b1->nbytes == b2->nbytes);
	//TODO
    for (int i=0; i<b2->nbytes; i++)
    {
        b1->bitstring[i] = b1->bitstring[i] & b2->bitstring[i];
    }
}

// bitwise OR ... b1 = b1 | b2

void orBits(Bits b1, Bits b2)
{

	assert(b1 != NULL && b2 != NULL);
	assert(b1->nbytes == b2->nbytes);
	//TODO
    for (int i=0; i<b1->nbytes; i++)
    {
        b1->bitstring[i] = b1->bitstring[i] | b2->bitstring[i];
    }
}

// left-shift ... b1 = b1 << n
// negative n gives right shift

void shiftBits(Bits b, int n)
{
    // TODO
    assert(b!=NULL);
    Count arrayIndex, arrayPosition;
    Byte mask;
    int bits = b->nbytes * 8;

    if (n>0)
    {
        for (int i= bits-n-1; i>=0; i--)
        {
            arrayIndex = i / 8;
            arrayPosition = i % 8;
            mask  = (1 << arrayPosition);
            if (mask & (b->bitstring[arrayIndex]))
            {
                arrayIndex = (i+n) / 8;
                arrayPosition = (i+n) % 8;
                mask  = (1 << arrayPosition);
                b->bitstring[arrayIndex] = mask | (b->bitstring[arrayIndex]);
            }
            else{
                arrayIndex = (i+n) / 8;
                arrayPosition = (i+n) % 8;
                mask  = ~(1 << arrayPosition);
                b->bitstring[arrayIndex] = mask & (b->bitstring[arrayIndex]);
            }
        }
        for (int j=0; j<n; j++)
        {
            unsetBit(b,j);
        }
    }
    else{
        for (int i=-n; i< bits; i++)
        {
            arrayIndex = i / 8;
            arrayPosition = i % 8;
            mask  = (1 << arrayPosition);
            if (mask & (b->bitstring[arrayIndex]))
            {
                arrayIndex = (i+n) / 8;
                arrayPosition = (i+n) % 8;
                mask  = (1 << arrayPosition);
                b->bitstring[arrayIndex] = mask | (b->bitstring[arrayIndex]);
            }
            else{
                arrayIndex = (i+n) / 8;
                arrayPosition = (i+n) % 8;
                mask  = ~(1 << arrayPosition);
                b->bitstring[arrayIndex] = mask & (b->bitstring[arrayIndex]);
            }
        }
        for (int j=bits+n+1; j<bits; j++)
        {
            arrayIndex = j / 8;
            arrayPosition = j % 8;
            mask  = ~(1 << arrayPosition);
            b->bitstring[arrayIndex] = mask & (b->bitstring[arrayIndex]);
        }

    }
}

// get a bit-string (of length b->nbytes)
// from specified position in Page buffer
// and place it in a BitsRep structure

void getBits(Page p, Offset pos, Bits b)
{
	//TODO
    Byte *addr = addrInPage(p, pos, b->nbytes);
    memcpy(b->bitstring, addr, b->nbytes);
}

// copy the bit-string array in a BitsRep
// structure to specified position in Page buffer

void putBits(Page p, Offset pos, Bits b)
{
	//TODO
    Byte *addr = addrInPage(p, pos, b->nbytes);
    memcpy(addr, b->bitstring, b->nbytes);
}

// show Bits on stdout
// display in order MSB to LSB
// do not append '\n'

void showBits(Bits b)
{
	assert(b != NULL);
//    printf("(%d,%d)",b->nbits,b->nbytes);
	for (int i = b->nbytes-1; i >= 0; i--) {
		for (int j = 7; j >= 0; j--) {
			Byte mask = (1 << j);
			if (b->bitstring[i] & mask)
				putchar('1');
			else
				putchar('0');
		}
	}
}
