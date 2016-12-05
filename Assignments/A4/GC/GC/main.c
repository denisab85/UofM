//
//  main.c
//  GC
//
//  Created by abakumod on 2016-11-28.
//  Copyright © 2016 abakumod. All rights reserved.
//

#include <stdio.h>
#include "ObjectManager.h"



int main(int argc, char *argv[])
{
    char *ptr;
    int i;
    Ref id1, id2, id3;
    initPool();
    
    id1 = insertObject(1020);
    ptr = (char*)retrieveObject(id1);
    for (i = 0; i < 1020; i++)
        ptr[i] = (char)(i%26 + 'A');
    
    id2 = insertObject(4);
    ptr = (char*)retrieveObject(id2);
    for (i = 0; i < 4; i++)
        ptr[i] = (char)(i%26 + 'H');
    
    dropReference(id2);
    
    id3 = insertObject(3);
    dropReference(id3);
    id3 = insertObject(12);
    ptr = (char*)retrieveObject(id3);
    for (i = 0; i < 12; i++)
        ptr[i] = (char)(i%26 + 'O');
    
    ptr = (char*)retrieveObject(id1);
    for (i = 0; i < 30; i++)
        fprintf(stderr,"%c",ptr[i]);
    fprintf(stderr,"\n");
    
    dumpPool();
    dumpPool();
    destroyPool();
    fprintf(stderr,"---\n");
    return 0;
}

