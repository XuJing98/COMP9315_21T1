// psig.c ... functions on page signatures (psig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "psig.h"
#include "hash.h"

Bits codeword1(char *attr_value, int m, int k)
{
    int  nbits = 0;   // count of set bits
    Bits cword = newBits(m);
    srandom(hash_any(attr_value, strlen(attr_value)));
    while (nbits < k) {
        int i = random() % m;
        if (!bitIsSet(cword, i)) {
            setBit(cword, i);
            nbits++;
        }
    }
    return cword;  // m-bits with k 1-bits and m-k 0-bits
}

// make a page signature using SIMC

Bits makePageSigSIMC(Reln r, Tuple t)
{
    Bits psig,cw;
    char **tupleval = tupleVals(r, t);
    int nAttr = nAttrs(r);
    int m = psigBits(r);
    int k = codeBits(r);
    char *questionmark = "?\0";
    psig = newBits(m);
    for (int i=0; i<nAttr; i++)
    {
        if (strcmp(tupleval[i], questionmark)==0)
        {
            cw = newBits(m);
//            printf("question mark ");
//            showBits(cw);
//            putchar('\n');
        }else{
            cw = codeword1(tupleval[i], m, k);
//            printf("attribute tuple ");
//            showBits(cw);
//            putchar('\n');
        }
        orBits(psig, cw);
    }
    return psig;
}

//make a page signature using CATC

Bits makePageSigCATC(Reln r, Tuple t)
{
    Bits psig,cw;
    char **tupleval = tupleVals(r, t);
    int nAttr = nAttrs(r);
    int m = psigBits(r);
    int m1 = psigBits(r) / nAttr;
    int m2 = psigBits(r) % nAttr;
    int k = codeBits(r);
    int counter1, counter2=0;
    char *questionmark = "?\0";
    psig = newBits(m);
    for (int i=0; i<nAttr; i++)
    {
        if (strcmp(tupleval[i],questionmark)==0)
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
                cw = codeword1(tupleval[i], m1+m2, k);
                counter1 = m1 + m2;
            }else{
                cw = codeword1(tupleval[i], m1, k);
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
                setBit(psig, counter2);
            }else{
                unsetBit(psig, counter2);
            }
            counter2++;
        }

    }
//    printf("psig");
//    showBits(psig);
//    putchar('\n');
    return psig;

}


Bits makePageSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	//TODO
	Bits psig;
    char sigtype = sigType(r);
//    printf("pm:%d",psigBits(r));
    switch(sigtype){
        case 'c':
            psig = makePageSigCATC(r, t); break;
        case 's':
            psig = makePageSigSIMC(r, t); break;
    }
//    printf("pagesignature:");
//    showBits(psig);

    return psig;
}

void findPagesUsingPageSigs(Query q)
{
	assert(q != NULL);
	//TODO
    Page p;
    Reln r = q->rel;
    unsetAllBits(q->pages);
    Bits querysig = makePageSig(r, q->qstring);
    Bits pagesig = newBits(psigBits(r));

    for (int i = 0; i < nPsigPages(r); i++) {
        p = getPage(psigFile(r), i);
        for (int j = 0; j < pageNitems(p); j++){
            getBits(p, j, pagesig);

            if (isSubset(querysig, pagesig)){
                // convert page signature into data page ID
                setBit(q->pages, q->nsigs);
            }
            q->nsigs++;
        }
        q->nsigpages++;
    }

}

