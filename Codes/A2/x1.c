// Test Bits ADT

#include <stdio.h>
#include "defs.h"
#include "reln.h"
#include "tuple.h"
#include "bits.h"

int main(int argc, char **argv)
{
	Bits b = newBits(60);
	Bits b2 = newBits(60);
    setBit(b2, 5);
    setBit(b2, 11);
    showBits(b2);
	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 5);
	printf("t=1: "); showBits(b); printf("\n");
	setBit(b, 0);
	setBit(b, 50);
	setBit(b, 59);
	setBit(b, 22);
	setBit(b, 11);
	if (isSubset(b2, b))printf("t=2: ");
	showBits(b2); printf("\n");
	showBits(b); printf("\n");
	setBit(b2, 12);
    if (isSubset(b2, b))printf("t=t: ");
    showBits(b2); printf("\n");
    orBits(b, b2);
    showBits(b);

	return 0;
}
