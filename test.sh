#!/usr/bin/sh
gcc test.c common.c keyvaluestore.c -o test.out -Wall -Werror && ./test.out