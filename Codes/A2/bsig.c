// bsig.c ... functions on Tuple Signatures (bsig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "bsig.h"
#include "psig.h"


void findPagesUsingBitSlices(Query q)
{
	assert(q != NULL);
	//TODO
	PageID pid=-1,offset;
	Page bsigpage;
    Reln r = q->rel;
    Bits qsig = makePageSig(r, q->qstring);
    Bits bsig = newBits(bsigBits(r));
    setAllBits(q->pages);
    for (int i = 0; i < psigBits(r); i++) {
        if (bitIsSet(qsig, i)) {
            if (i/maxBsigsPP(r) !=pid)
            {
                q->nsigpages++;
            }
            pid = i / maxBsigsPP(r);
            offset = i % maxBsigsPP(r);
            bsigpage = getPage(r->bsigf, pid);
            getBits(bsigpage, offset, bsig);
            // if the pos is 0, set pages in that pos to 0
            for (int j = 0; j < nPsigs(r); j++) {
                if (!bitIsSet(bsig, j)) {
                    unsetBit(q->pages, j);
                }
            }
            q->nsigs++;
        }
    }
    freeBits(qsig);
    freeBits(bsig);
}

