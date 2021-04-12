cmake_minimum_required(VERSION 3.13)
project(COMP9315_21T1 C)

set(CMAKE_C_STANDARD 11)

add_executable(trial
        Codes/A2/x1.c
        Codes/A2/bits.c
        Codes/A2/bits.h
        Codes/A2/util.c
        Codes/A2/util.h
        Codes/A2/tuple.h
        Codes/A2/reln.h
        Codes/A2/defs.h
        Codes/A2/page.c
        Codes/A2/page.h
        )

add_executable(test
        Codes/A2/x2.c
        Codes/A2/psig.c
        Codes/A2/psig.h
        Codes/A2/tsig.c
        Codes/A2/tsig.h
        Codes/A2/bsig.c
        Codes/A2/bsig.h
        Codes/A2/defs.h
        Codes/A2/hash.c
        Codes/A2/hash.h
        Codes/A2/bits.c
        Codes/A2/bits.h
        Codes/A2/util.c
        Codes/A2/util.h
        Codes/A2/tuple.c
        Codes/A2/tuple.h
        Codes/A2/reln.h
        Codes/A2/reln.c
        Codes/A2/defs.h
        Codes/A2/page.c
        Codes/A2/page.h
        Codes/A2/query.h
        Codes/A2/query.c
        )


