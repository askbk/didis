#!/usr/bin/sh
gcc -g test.c common.c keyvaluestore.c lists.c strings.c -o test.out -Wall -Werror && ./test.out
