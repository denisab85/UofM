//
//  fib.c
//  fib
//
//  Created by abakumod on 2016-11-22.
//  Copyright © 2016 abakumod. All rights reserved.
//

#include "fib.h"
#include <stdio.h>

#ifndef CACHE_SIZE
#define CACHE_SIZE 5
#endif

long cache[CACHE_SIZE] = {0};

long fib(unsigned int n)
{
    long result = 0;
    if (n<3)
        result = 1;
    else if ( n < (CACHE_SIZE+3) )
    {
        if (! cache[n-3])
            cache[n-3] = fib(n-1) + fib(n-2);
        result = cache[n-3];
    }
    // restriction imposed by LONG type
    else if (n < 93)
        result = fib(n-1) + fib(n-2);
    else return -1;
    return result;
}


long fib_even(unsigned int n)
{
    long result = 0;
    if (n<3)
        result = 1;
    else if ( (n < (CACHE_SIZE*2+3) ) && !(n % 2) )
    {
        if (! cache[n/2-2])
            cache[n/2-2] = fib_even(n-1) + fib_even(n-2);
        result = cache[n/2-2];
    }
    // restriction imposed by LONG type
    else if (n < 93)
        result = fib_even(n-1) + fib_even(n-2);
    else return -1;
    return result;
}

