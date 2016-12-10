//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: 4
// QUESTION: 1
//
// REMARKS: Implementation of an object manager
//
//-----------------------------------------


#include <stdlib.h>
#include <stdio.h>
#include "ObjectManager.h"

#define OBJECTS 1000

int main(int argc, char *argv[])
{
    char *ptr;
    int i;
    int j;
    Ref id[OBJECTS];
    
    
    
    for (i=1; i < OBJECTS; i++)
    {
        initPool();
        for (j=0; j<i; j++)
            id[j] = insertObject(rand() % 1024);
        dropReference(id[0]);
        dropReference(id[i-1]);
        //dumpPool();
        destroyPool();
    }
    
    initPool();

    id[1] = insertObject(1020);
    ptr = (char*)retrieveObject(id[1]);
    for (i = 0; i < 1020; i++)
        ptr[i] = (char)(i%26 + 'A');
    
    id[2] = insertObject(4);
    ptr = (char*)retrieveObject(id[2]);
    for (i = 0; i < 4; i++)
        ptr[i] = (char)(i%26 + 'H');
    
    dropReference(id[2]);
    
    id[3] = insertObject(3);
    dropReference(id[3]);
    id[3] = insertObject(12);
    ptr = (char*)retrieveObject(id[3]);
    for (i = 0; i < 12; i++)
        ptr[i] = (char)(i%26 + 'O');
    
    ptr = (char*)retrieveObject(id[1]);
    for (i = 0; i < 30; i++)
        fprintf(stderr,"%c",ptr[i]);
    fprintf(stderr,"\n");
    
    dumpPool();
    dumpPool();
    destroyPool();
    fprintf(stderr,"---\n");
    return 0;
}

