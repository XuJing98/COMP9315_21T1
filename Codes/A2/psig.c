// psig.c ... functions on page signatures (psig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include <string.h>
#include "defs.h"
#include "reln.h"
#include "query.h"
#include "psig.h"
#include "tsig.h"
#include "hash.h"


// make a page signature using SIMC

Bits makePageSigSIMC(Reln r, Tuple t)
{

    char **tupleval = tupleVals(r, t);
    Bits psig = newBits(r->params.pm);

    for (int i=0; i<r->params.nattrs; i++)
    {
        if (tupleval[i][0]=='?')
        {
            continue;
        }

        Bits cw = codeword(tupleval[i], r->params.pm, r->params.tk);
        orBits(psig, cw);
        //    free(cw);
    }

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
    return psig;

}


Bits makePageSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	//TODO
    if(r->params.sigtype == 's')
    {
        return makePageSigSIMC(r, t);
    } else {
        return makePageSigCATC(r, t);
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

