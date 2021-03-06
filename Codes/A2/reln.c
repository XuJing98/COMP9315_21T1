// reln.c ... functions on Relations
// part of signature indexed files
// Written by John Shepherd, March 2019

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "defs.h"
#include "reln.h"
#include "tsig.h"
#include "psig.h"
#include "page.h"
#include "tuple.h"
#include "bits.h"
#include "hash.h"

// open a file with a specified suffix
// - always open for both reading and writing


File openFile(char *name, char *suffix)
{
	char fname[MAXFILENAME];
	sprintf(fname,"%s.%s",name,suffix);
	File f = open(fname,O_RDWR|O_CREAT,0644);
	assert(f >= 0);
	return f;
}

// create a new relation (five files)
// data file has one empty data page

Status newRelation(char *name, Count nattrs, float pF, char sigtype,
                   Count tk, Count tm, Count pm, Count bm)
{
	Reln r = malloc(sizeof(RelnRep));
	RelnParams *p = &(r->params);
	assert(r != NULL);
	p->nattrs = nattrs;
	p->pF = pF,
	p->sigtype = sigtype;
	p->tupsize = 28 + 7*(nattrs-2);
	Count available = (PAGESIZE-sizeof(Count));
	p->tupPP = available/p->tupsize;
	p->tk = tk; 
	if (tm%8 > 0) tm += 8-(tm%8); // round up to byte size
	p->tm = tm; p->tsigSize = tm/8; p->tsigPP = available/(tm/8);
	if (pm%8 > 0) pm += 8-(pm%8); // round up to byte size
	p->pm = pm; p->psigSize = pm/8; p->psigPP = available/(pm/8);
	if (p->psigPP < 2) { free(r); return -1; }
	if (bm%8 > 0) bm += 8-(bm%8); // round up to byte size
	p->bm = bm; p->bsigSize = bm/8; p->bsigPP = available/(bm/8);
	if (p->bsigPP < 2) { free(r); return -1; }
	r->infof = openFile(name,"info");
	r->dataf = openFile(name,"data");
	r->tsigf = openFile(name,"tsig");
	r->psigf = openFile(name,"psig");
	r->bsigf = openFile(name,"bsig");
	addPage(r->dataf); p->npages = 1; p->ntups = 0;
	addPage(r->tsigf); p->tsigNpages = 1; p->ntsigs = 0;
	addPage(r->psigf); p->psigNpages = 1; p->npsigs = 0;
	addPage(r->bsigf); p->bsigNpages = 1; p->nbsigs = 0;
	// Create a file containing "pm" all-zeroes bit-strings,
    // each of which has length "bm" bits
	//TODO
	PageID bsigpageID;
	Page bsigpage;
//	printf("bm:%d",bm);
	Bits bsig = newBits(bm);
	for (int i=0; i<pm; i++)
    {

	    bsigpageID = p->bsigNpages - 1;
	    bsigpage = getPage(r->bsigf, bsigpageID);
	    if (pageNitems(bsigpage) == p->bsigPP )
        {
            addPage(r->bsigf);
            p->bsigNpages++;
            bsigpageID++;
            free(bsigpage);
            bsigpage = newPage();
            if (bsigpage == NULL) return NO_PAGE;
        }
        putBits(bsigpage, pageNitems(bsigpage), bsig);
        addOneItem(bsigpage);
        p->nbsigs++;
        putPage(r->bsigf, bsigpageID, bsigpage);

    }
	freeBits(bsig);
	closeRelation(r);
	return 0;
}

// check whether a relation already exists

Bool existsRelation(char *name)
{
	char fname[MAXFILENAME];
	sprintf(fname,"%s.info",name);
	File f = open(fname,O_RDONLY);
	if (f < 0)
		return FALSE;
	else {
		close(f);
		return TRUE;
	}
}

// set up a relation descriptor from relation name
// open files, reads information from rel.info

Reln openRelation(char *name)
{
	Reln r = malloc(sizeof(RelnRep));
	assert(r != NULL);
	r->infof = openFile(name,"info");
	r->dataf = openFile(name,"data");
	r->tsigf = openFile(name,"tsig");
	r->psigf = openFile(name,"psig");
	r->bsigf = openFile(name,"bsig");
	read(r->infof, &(r->params), sizeof(RelnParams));
	return r;
}

// release files and descriptor for an open relation
// copy latest information to .info file
// note: we don't write ChoiceVector since it doesn't change

void closeRelation(Reln r)
{
	// make sure updated global data is put in info file
	lseek(r->infof, 0, SEEK_SET);
	int n = write(r->infof, &(r->params), sizeof(RelnParams));
	assert(n == sizeof(RelnParams));
	close(r->infof); close(r->dataf);
	close(r->tsigf); close(r->psigf); close(r->bsigf);
	free(r);
}



Status bputPage(File f, PageID pid, Page p)
{
    assert(pid >= 0);
    int ok = lseek(f, pid*PAGESIZE, SEEK_SET);
    assert(ok >= 0);
    int n = write(f, p, PAGESIZE);
    assert(n == PAGESIZE);
    return 0;
}

// insert a new tuple into a relation
// returns page where inserted
// returns NO_PAGE if insert fails completely

PageID addToRelation(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL && strlen(t) == tupSize(r));
	Page p;  PageID pid;
	RelnParams *rp = &(r->params);
	
	// add tuple to last page
	pid = rp->npages-1;
	p = getPage(r->dataf, pid);
	// check if room on last page; if not add new page
	if (pageNitems(p) == rp->tupPP) {
		addPage(r->dataf);
		rp->npages++;
		pid++;
		free(p);
		p = newPage();
		if (p == NULL) return NO_PAGE;
	}
	addTupleToPage(r, p, t);
	rp->ntups++;  //written to disk in closeRelation()
	putPage(r->dataf, pid, p);

	// compute tuple signature and add to tsigf
	
	//TODO
    Bits tsig = makeTupleSig(r, t);
    pid = rp->tsigNpages - 1;
    p = getPage(r->tsigf, pid);
    // if the page is full
    if (pageNitems(p) == rp->tsigPP) {
        addPage(r->tsigf);
        rp->tsigNpages++;
        pid++;
        free(p);
        p = newPage();
        if (p == NULL) return NO_PAGE;
    }
    putBits(p, pageNitems(p), tsig);
    addOneItem(p);
    rp->ntsigs++;
    putPage(r->tsigf, pid, p);
    free(tsig);
	// compute page signature and add to psigf

	//TODO
    Bits psig = makePageSig(r, t);
    pid = rp->psigNpages - 1;
    p = getPage(r->psigf, pid);

    if (rp->npages != rp->npsigs) {
        // if the page is full
        if (pageNitems(p) == rp->psigPP) {
            addPage(r->psigf);
            rp->psigNpages++;
            pid++;
            free(p);
            p = newPage();
            if (p == NULL) return NO_PAGE;
        }
        putBits(p, pageNitems(p), psig);
        addOneItem(p);
        rp->npsigs++;

    }else{

        Bits ppsig = newBits(rp->pm);
        getBits(p, pageNitems(p)-1, ppsig);
        orBits(ppsig, psig);
        putBits(p, pageNitems(p)-1, ppsig);
        free(ppsig);

    }
    putPage(r->psigf, pid, p);


	// use page signature to update bit-slices

	//TODO
    pid = -1;
    Bits bsigtuple = newBits(rp->bm);
	for (int i=0; i < rp->pm; i++)
    {
	    if (bitIsSet(psig, i))
        {
            if(i/rp->bsigPP != pid) {
//                printf("a, %d\n", rp->bsigPP);
                if (pid!=-1) free(p);
                pid = i / rp->bsigPP;
                p = getPage(r->bsigf, pid);
            }

            getBits(p, i % rp->bsigPP, bsigtuple);
            setBit(bsigtuple, rp->npages-1);
            putBits(p, i % rp->bsigPP, bsigtuple);
            bputPage(r->bsigf, pid, p);
        }

    }
    free(p);
    free(bsigtuple);
	free(psig);




	return nPages(r)-1;
}

// displays info about open Reln (for debugging)

void relationStats(Reln r)
{
	RelnParams *p = &(r->params);
	printf("Global Info:\n");
	printf("Dynamic:\n");
    printf("  #items:  tuples: %d  tsigs: %d  psigs: %d  bsigs: %d\n",
			p->ntups, p->ntsigs, p->npsigs, p->nbsigs);
    printf("  #pages:  tuples: %d  tsigs: %d  psigs: %d  bsigs: %d\n",
			p->npages, p->tsigNpages, p->psigNpages, p->bsigNpages);
	printf("Static:\n");
    printf("  tups   #attrs: %d  size: %d bytes  max/page: %d\n",
			p->nattrs, p->tupsize, p->tupPP);
	printf("  sigs   %s",
            p->sigtype == 'c' ? "catc" : "simc");
    if (p->sigtype == 's')
	    printf("  bits/attr: %d", p->tk);
    printf("\n");
	printf("  tsigs  size: %d bits (%d bytes)  max/page: %d\n",
			p->tm, p->tsigSize, p->tsigPP);
	printf("  psigs  size: %d bits (%d bytes)  max/page: %d\n",
			p->pm, p->psigSize, p->psigPP);
	printf("  bsigs  size: %d bits (%d bytes)  max/page: %d\n",
			p->bm, p->bsigSize, p->bsigPP);
}
