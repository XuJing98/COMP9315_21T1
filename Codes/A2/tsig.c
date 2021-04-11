// tsig.c ... functions on Tuple Signatures (tsig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include <unistd.h>
#include <string.h>
#include "defs.h"
#include "tsig.h"
#include "reln.h"
#include "hash.h"
#include "bits.h"

// generate a codeword with m,k

Bits codeword(char *attr_value, int m, int k)
{
    int  nbits = 0;   // count of set bits
    srandom(hash_any(attr_value, strlen(attr_value)));
    Bits cword = newBits(m);
    while (nbits < k) {
        int i = random() % m;
        if (!bitIsSet(cword, i)) {
            setBit(cword, i);
            nbits++;
        }
    }
    return cword;  // m-bits with k 1-bits and m-k 0-bits
}

// make a tuple signature using SIMC

Bits makeTupleSigSIMC(Reln r, Tuple t)
{
    Bits tsig,cw;
    char **tupleval = tupleVals(r, t);
    int nAttr = r->params.nattrs;
    int m = r->params.tm;
    int k = r->params.tk;
    tsig = newBits(m);
    for (int i=0; i<nAttr; i++)
    {
        if (tupleval[i][0]=='?')
        {
            continue;
        }else{
            cw = codeword(tupleval[i], m, k);
//            printf("attribute tuple ");
//            showBits(cw);
//            putchar('\n');
        }
        orBits(tsig, cw);
    }
    free(cw);
    return tsig;
}

//make a tuple signature using CATC

Bits makeTupleSigCATC(Reln r, Tuple t)
{
    Bits tsig,cw;
    char **tupleval = tupleVals(r, t);
    int nAttr = nAttrs(r);
    int m = tsigBits(r);
    int m1 = tsigBits(r) / nAttr;
    int m2 = tsigBits(r) % nAttr;
    int k = codeBits(r);
    int counter1, counter2=0;
    tsig = newBits(m);
    for (int i=0; i<nAttr; i++)
    {
        if (tupleval[i][0]=='?')
        {
            if (m2!=0 && i==0)
            {
                cw = newBits(m1+m2);
                counter1 = m1 + m2;
            }else{
                cw = newBits(m1);
                counter1 = m1;
            }
        }else{
            if (m2 != 0 && i==0)
            {
                cw = codeword(tupleval[i], m1+m2, k);
                counter1 = m1 + m2;
            }else{
                cw = codeword(tupleval[i], m1, k);
                counter1 = m1;
            }
//            printf("attribute tuple ");
//            showBits(cw);
//            putchar('\n');
        }
        for(int j=0; j<counter1; j++)
        {
            if (bitIsSet(cw, j))
            {
                setBit(tsig, counter2);
            }else{
                unsetBit(tsig, counter2);
            }
            counter2++;
        }

    }
    freeBits(cw);
//    printf("tsig");
//    showBits(tsig);
//    putchar('\n');
    return tsig;

}


// make a tuple signature

Bits makeTupleSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	//TODO
	char sigtype = r->params.sigtype;
	if (sigtype=='c'){
	    return makeTupleSigCATC(r, t);
	}else{
        return makeTupleSigSIMC(r, t);
	}

}

// find "matching" pages using tuple signatures

void findPagesUsingTupSigs(Query q)
{
	assert(q != NULL);
	//TODO
    Page p;
    Reln r = q->rel;
    unsetAllBits(q->pages);
    Bits querysig = makeTupleSig(r, q->qstring);
//    printf("querysig ");
//    showBits(querysig);
//    putchar('\n');
    Bits tuplesig = newBits(tsigBits(r));
//    printf("tuplesig ");
//    showBits(tuplesig);
//    putchar('\n');
//    printf("ntsigpages: %d", nTsigPages(r));
    for (int i = 0; i < nTsigPages(r); i++) {
        p = getPage(tsigFile(r), i);
        for (int j = 0; j < pageNitems(p); j++){
            getBits(p, j, tuplesig);
//            printf("querysig ");
//            showBits(querysig);
//            putchar('\n');
//            printf("tuplesig ");
//            showBits(tuplesig);
//            putchar('\n');
            if (isSubset(querysig, tuplesig)){
            // convert tuple index id into data page ID
                setBit(q->pages, q->nsigs / maxTupsPP(r));
            }
            q->nsigs++;
        }
        q->nsigpages++;
    }

    freeBits(tuplesig);
    freeBits(querysig);

	// The printf below is primarily for debugging
	// Remove it before submitting this function
//	printf("Matched Pages:"); showBits(q->pages); putchar('\n');
}
