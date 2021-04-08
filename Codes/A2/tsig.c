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

// make a tuple signature using SIMC

Bits makeTupleSigSIMC(Reln r, Tuple t)
{
    Bits tsig,cw;
    char **tupleval = tupleVals(r, t);
    int nAttr = nAttrs(r);
    int m = tsigBits(r);
    int k = codeBits(r);
    tsig = newBits(m);
    for (int i=0; i<nAttr; i++)
    {
        if (tupleval[i][0]=='?')
        {
            printf("question mark");
            cw = newBits(m);
        }else{
            cw = codeword(tupleval[i], m, k);
        }
        orBits(tsig, cw);
    }
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
    return tsig;

}


// make a tuple signature

Bits makeTupleSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	//TODO
	Bits tsig;
	char sigtype = sigType(r);
	printf("sigtype:%c",sigtype);
	switch(sigtype){
	    case 'c':
	        tsig = makeTupleSigCATC(r, t); break;
        case 's':
            tsig = makeTupleSigSIMC(r, t); break;
        default:
            return NULL;
	}
	return tsig;

}

// find "matching" pages using tuple signatures

void findPagesUsingTupSigs(Query q)
{
	assert(q != NULL);
	//TODO
//    Reln r = q->rel;
//    // query signature
//    Bits qsig = makeTupleSig(r, q->qstring);
//    // tuple signatrue
//    Bits tsig = newBits(tsigBits(r));
//    // Pages = AllZeroBits
//    unsetAllBits(q->pages);
//    int pid;
//
//    // iterate all the pages
//    for (int i = 0; i < nTsigPages(r); i++) {
//        pid = i;
//        Page p = getPage(r->tsigf, pid);
//        // for each tuple
//        for (int j = 0; j < pageNitems(p); j++) {
//
//            // get the signature for each tuple store in the tsig
//            getBits(p, j, tsig);
//            if (isSubset(qsig, tsig)) {
//                // means it matches
//                int located_page = q->nsigs / maxTupsPP(r);
//                // include PID in Pages
//                setBit(q->pages, located_page);
//            }
//            // the number of signature we scanned
//            q->nsigs++;
//        }
//        q->nsigpages++;
//    }
    Bits querysig = makeTupleSig(q->rel, q->qstring);
    Page p; Bits sig = newBits(tsigBits(q->rel)); PageID pid, i;
    for (pid = 0; pid < nTsigPages(q->rel); pid++) {
        p = getPage(tsigFile(q->rel), pid);
        for (i = 0; i < pageNitems(p); i++){
            getBits(p, i, sig);
            //if query signature is subset of retrieved signature, then match
            if (isSubset(querysig, sig) == TRUE){
                // printf("sig on pid:%d, i:%d, pagenum: %d,sig:", pid, i, q->nsigs/maxTupsPP(q->rel)); showBits(sig); putchar('\n');
                // get the page num for tuple, and set that bit
                setBit(q->pages, q->nsigs/maxTupsPP(q->rel));
            }
            q->nsigs++;
        }
        q->nsigpages++;
    }


	// The printf below is primarily for debugging
	// Remove it before submitting this function
	printf("Matched Pages:"); showBits(q->pages); putchar('\n');
}
