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



#include "ObjectManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct Index index_item;

struct Index
{
    Ref ID;       // number of the object in pool
    void *start;  // pointer to the object
    int size;     // size of object in bytes
    unsigned int refCount;   // number of references currently in use
    index_item *next;  // pointer to the next index_item, NULL if the last
};

#define poolSize 1024

//-------------------------------------------------------------------------------------
// FILE-SCOPE VARIABLES
//-------------------------------------------------------------------------------------

// pools of objects in the heap
void *pool[2] = {NULL, NULL};
int active_pool = 0;

// pointer to the first index_item
index_item *top  = NULL;
index_item *last = NULL;

// number of currently created objects
int objectCount = 0;

// pointer to the beginning of the next unused address
void *freePtr = NULL;


//-----------------------------------------------------------------------------
// IMPLEMENTATION
//-----------------------------------------------------------------------------

index_item *createIndexItem (const int size)
{
    index_item *newindex_item = malloc (sizeof (index_item));
    
    assert (newindex_item != NULL);
   
    if (newindex_item)
    {
        newindex_item->ID = ++objectCount;
        newindex_item->refCount = 1;
        newindex_item->size = size;
        newindex_item->next = NULL;
    }
    return newindex_item;
}

// search for given number, return pointer to index_item if found or NULL otherwise
index_item *searchIndex (const Ref ref)
{
    index_item *ind = top;
    while ( (ind) && (ind->ID != ref) )
        ind = ind->next;
    return ind;
}


void removeIndexItem (const Ref ref)
{
    index_item *ind = top;
    index_item *prev = top;
    while ( (ind) && (ind->ID != ref) )
    {
        prev = ind;
        ind = ind->next;
    }
    prev->next = ind->next;
    free(ind);
}


Ref insertObject (const int size)
{
    Ref result = NULL_REF;
    index_item *newindex_item = createIndexItem (size);
    
    unsigned long memAvailable = pool[active_pool] + poolSize - freePtr;
    
    // if not enough memory, try to GC the pool and check again
    if ( (memAvailable < size) && (memAvailable + compact() < size) )
        return NULL_REF;
    else if (newindex_item)
    {
        if (!top)
            top = newindex_item;
        else
            last->next = newindex_item;
        last = newindex_item;
        newindex_item->start = freePtr;
        freePtr += size*sizeof(char);
        result = newindex_item->ID;
    }
    
//    assert ( ((result == true) && (newindex_item != NULL)) || (result == false && newindex_item == NULL) );
//    assert (search(item));
    
    return result;
}


void *retrieveObject (const Ref ref)
{
    void *result = NULL;
    index_item *ind = searchIndex (ref);
    
    if (ind)
        result = ind->start;
    return result;
}


void addReference (const Ref ref)
{
    index_item *ind = searchIndex (ref);
    if (ind)
        ind->refCount++;
}


void dropReference (const Ref ref)
{
    index_item *ind = searchIndex (ref);
    if (ind && (ind->refCount > 0))
        ind->refCount--;
}


void initPool()
{
    pool[0] = malloc(sizeof(char)*poolSize);
    pool[1] = malloc(sizeof(char)*poolSize);
    freePtr = pool[0];
}


void destroyPool()
{
    if (top)
    {
        index_item *temp = NULL;
        
        while (top != NULL)
        {
            temp = top;
            top = top->next;
            free (temp);
            objectCount--;
        }
        top = NULL;
    }
    free (pool[0]);
    free (pool[1]);
    assert (objectCount == 0);
}



void dumpPool()
{
    index_item *ind = top;
    unsigned long offset = 0;
    
    printf ("\n==================================");
    printf ("\n|         Pool information:       |");
    printf ("\n==================================\n");
    while (ind)
    {
        assert ( pool[active_pool] + offset == ind->start );
        offset += ind->size;
        printf ("\nID:    %lu\nStart: %#011x\nSize:  %d\n", ind->ID, (unsigned)ind->start, ind->size);
        ind = ind->next;
    }
}


static unsigned long compact()
{
    unsigned long memFreed = 0;
    index_item *ind = top;
    freePtr = pool[!active_pool];
    while (ind)
    {
        if (ind->refCount)
        {
            memcpy(freePtr, ind->start, ind->size);
            ind->start = freePtr;
            freePtr += ind->size;
        }
        else
        {
            memFreed += ind->size;
            removeIndexItem(ind->ID);
            objectCount--;
        }
        ind = ind->next;
    }
    active_pool = !active_pool;   //toggle
    return memFreed;
}
