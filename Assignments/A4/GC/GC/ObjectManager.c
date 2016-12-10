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



//-------------------------------------------------------------------------------------
// FILE-SCOPE VARIABLES
//-------------------------------------------------------------------------------------

// pools of objects in the heap
char *pool[2] = {NULL, NULL};
int active_pool = -1;

// pointer to the first and last index_item
index_item *top  = NULL;
index_item *last = NULL;

// number of currently created objects
int objectCount = 0;

// pointer to the beginning of the next unused address
char *freePtr = NULL;



//-----------------------------------------------------------------------------
// IMPLEMENTATION
//-----------------------------------------------------------------------------

// count memory cyrrently in use
unsigned long memInUse()
{
    return freePtr - pool[active_pool];
}


index_item *createIndexItem (const int size)
{
    index_item *newindex_item = (index_item*)malloc (sizeof (index_item));
    
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
    index_item *prev = NULL;
    
    assert (top);
    while ( (ind) && (ind->ID != ref) )
    {
        prev = ind;
        ind = ind->next;
    }
    
    // if the top item is being deleted
    if (ind == top)
        top = top->next;
    else
        prev->next = ind->next;
    
    // if the top item is being deleted
    if (ind == last)
        last = prev;
    free(ind);
}


// Mark and Sweep Defragmenting garbage collector
static unsigned long compact()
{
    unsigned long memFreed = 0;
    index_item *ind = top;
    
    assert ( (active_pool == 0) || (active_pool == 1) );
    
    if (( (active_pool == 0) || (active_pool == 1) ) && top)
    {
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
        active_pool = !active_pool;   //toggle pools
        
        printf("Memory state after garbage collection:\n");
        printf("The number of objects that exist:   %d\n", objectCount);
        printf("The current number of bytes in use: %lu\n", memInUse());
        printf("The number of bytes collected:      %lu\n\n", memFreed);
    }

    return memFreed;
}


Ref insertObject (const int size)
{
    Ref result = NULL_REF;
    index_item *newindex_item = NULL;
    unsigned long memAvailable = 0;
    
    assert ( (active_pool == 0) || (active_pool == 1) );
    
    if ( (active_pool == 0) || (active_pool == 1) )
    {
        memAvailable = MEMORY_SIZE - memInUse();
        
        // if not enough memory, try to GC the pool and check again
        if ( (memAvailable < size) && (memAvailable + compact() < size) )
            return NULL_REF;
        else
        {
            newindex_item = createIndexItem (size);
            if (newindex_item)
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
        }
    }
    assert ( ((result) && (newindex_item)) || (!result && !newindex_item) );
    
    return result;
}


void *retrieveObject (const Ref ref)
{
    void *result = NULL;
    index_item *ind = NULL;
    
    assert ( (active_pool == 0) || (active_pool == 1) );
    
    if (( (active_pool == 0) || (active_pool == 1) ) && top)
    {
        ind = searchIndex (ref);
        
        if (ind)
            result = ind->start;
    }
    return result;
}


void addReference (const Ref ref)
{
    assert ( (active_pool == 0) || (active_pool == 1) );
    
    if (( (active_pool == 0) || (active_pool == 1) ) && top)
    {
        index_item *ind = searchIndex (ref);
        if (ind)
            ind->refCount++;
    }
}


void dropReference (const Ref ref)
{
    
    //assert ( (active_pool == 0) || (active_pool == 1) );
    
    if ( (active_pool == 0) || (active_pool == 1) )
    {
        index_item *ind = searchIndex (ref);
        if (ind)
            ind->refCount = 0;
    }
}


void initPool()
{
    assert (active_pool == -1);
    
    if (active_pool == -1)
    {
        pool[0] = (char*) malloc(sizeof(char)*MEMORY_SIZE);
        pool[1] = (char*) malloc(sizeof(char)*MEMORY_SIZE);
        active_pool = 0;
        freePtr = pool[active_pool];
    }
    
    assert (pool[0]);
    assert (pool[1]);
    assert (freePtr);
    
}


void destroyPool()
{
    assert ( (active_pool == 0) || (active_pool == 1) );
    
    if (( (active_pool == 0) || (active_pool == 1) ) && top)
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
    active_pool = -1;
    
    assert (objectCount == 0);
}



void dumpPool()
{
    index_item *ind = top;
    unsigned long offset = 0;
    unsigned long count = 0;
    
    assert ( (active_pool == 0) || (active_pool == 1) );
    
    if ( (active_pool == 0) || (active_pool == 1) )
    {
        printf ("\n==================================");
        printf ("\n|         Pool information:       |");
        printf ("\n==================================\n");
        while (ind)
        {
            count++;
            assert ( pool[active_pool] + offset == ind->start );
            offset += ind->size;
            printf ("\nID:    %lu\nStart: %p\nSize:  %d\n", ind->ID, ind->start, ind->size);
            ind = ind->next;
        }
        fprintf(stderr, "\nObject count = %d\n", objectCount);
    }
    
    assert(count == objectCount);
}

