//
//  main.c
//  fib
//
//  Created by abakumod on 2016-11-22.
//  Copyright © 2016 abakumod. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "fib.h"

#ifndef CACHE_SIZE
#define CACHE_SIZE 5
#endif

#ifdef EVEN
const int even = 1;
#else
const int even = 0;
#endif

int main( int argc, char *argv[] )
{
    if (argc > 1)
    {
        int fibNum = atoi(argv[1]);
        printf("CACHE_SIZE: %d. Cache type: %s.   ", CACHE_SIZE, even?"even":"all");
        printf("The %d Fibonacci number is ", fibNum);
        if (even)
            printf ("%ld\n", fib_even(fibNum));
        else
            printf ("%ld\n", fib(fibNum));
    }
    return EXIT_SUCCESS;
}
