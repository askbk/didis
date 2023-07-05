#!/usr/bin/sh
gcc -g test.c common.c keyvaluestore.c liststore.c strings.c -o test.out -Wall -Werror && ./test.out
