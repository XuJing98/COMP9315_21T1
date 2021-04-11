// psig.c ... functions on page signatures (psig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "psig.h"
#include "hash.h"
#include "tsig.h"


// make a page signature using SIMC

Bits makePageSigSIMC(Reln r, Tuple t)
{
    Bits psig,cw;
    char **tupleval = tupleVals(r, t);
    psig = newBits(r->params.pm);
//    cw = newBits(r->params.pm);
    for (int i=0; i<r->params.nattrs; i++)
    {
        if (tupleval[i][0]=='?')
        {
            continue;
        }else{
//            cw = codeword_reduce(cw,tupleval[i], r->params.pm, r->params.tk);
            cw = codeword(tupleval[i], r->params.pm, r->params.tk);
//            printf("cw");
//            showBits(cw);
//            putchar('\n');
        }
        orBits(psig, cw);
    }
    free(cw);
//    printf("psig");
//    showBits(psig);
    return psig;
}

//make a page signature using CATC

Bits makePageSigCATC(Reln r, Tuple t)
{
    Bits psig,cw;
    char **tupleval = tupleVals(r, t);
    int nAttr = r->params.nattrs;
    int m = r->params.pm;
    int m1 = m / nAttr;
    int m2 = m % nAttr;
    int k = r->params.tk;
    int counter1, counter2=0;
    psig = newBits(m);
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
                setBit(psig, counter2);
            }else{
                unsetBit(psig, counter2);
            }
            counter2++;
        }

    }
    free(cw);
//    printf("psig");
//    showBits(psig);
//    putchar('\n');
    return psig;

}


Bits makePageSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	//TODO
    char sigtype = r->params.sigtype;
    if(sigtype == 'c')
    {
        return makePageSigCATC(r, t);
    } else {
        return makePageSigSIMC(r, t);
    }

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
    freeBits(querysig);
    freeBits(pagesig);

}

