// Test Bits ADT

#include <stdio.h>
#include "defs.h"
#include "reln.h"
#include "tuple.h"
#include "bits.h"

int main(int argc, char **argv)
{
	Bits b = newBits(60);
	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 5);
	printf("t=1: "); showBits(b); printf("\n");
	setBit(b, 0);
	setBit(b, 50);
	setBit(b, 59);
	setBit(b, 22);
	setBit(b, 11);
	printf("t=2: "); showBits(b); printf("\n");
	if (bitIsSet(b,5)) printf("Bit 5 is set\n");
	if (bitIsSet(b,10)) printf("Bit 10 is set\n");
	shiftBits(b,3);
    printf("t=3: "); showBits(b); printf("\n");
    shiftBits(b,0);
    printf("t=3: "); showBits(b); printf("\n");
	setAllBits(b);
	printf("t=4: "); showBits(b); printf("\n");
	unsetBit(b, 40);
	printf("t=5: "); showBits(b); printf("\n");
    unsetAllBits(b);
    printf("t=6: "); showBits(b); printf("\n");
	if (bitIsSet(b,20)) printf("Bit 20 is set\n");
	if (bitIsSet(b,40)) printf("Bit 40 is set\n");
	if (bitIsSet(b,50)) printf("Bit 50 is set\n");
	setBit(b, 59);
	return 0;
}
