// tsig.h ... interface to functions on tuple signatures
// part of signature indexed files
// Written by John Shepherd, March 2019

#ifndef TSIG_H
#define TSIG_H 1

#include "defs.h"
#include "query.h"
#include "reln.h"
#include "bits.h"

Bits makeTupleSig(Reln, Tuple);
void findPagesUsingTupSigs(Query);
Bits codeword(char *attr_value, int m, int k);
Bits codeword_reduce(Bits cword,char *attr_value, int m, int k);

#endif
