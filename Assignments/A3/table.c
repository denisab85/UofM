#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "table.h"

//-----------------------------------------------------------------------------
// IMPLEMENTATION
//-----------------------------------------------------------------------------


// empty the table so that we clear all memory and can start a fresh table
void clearTable ()
{
    if (top)
    {
        node *temp = NULL;
        
        while (top != NULL)
        {
            temp = top;
            top = top->next;
            free (temp);
            size--;
        }
        top = NULL;
    }
}


node * createNode (int num)
{
    node * newNode = malloc ( sizeof (node) );
    newNode->number = num;
    newNode->prev = NULL;
    newNode->next = NULL;
    
    assert (newNode != NULL);
    assert (newNode->number == num);
    
    return newNode;
}


// add an element to the Table
Boolean insertItem (int item)
{
    Boolean result = false;
    node * temp = top;
    node * newNode = createNode (item);

    if (!top)
    {
        top = newNode;
        result = true;
    }
    else
    {
        while ( (temp->next) && (item > temp->number) )
        {
            temp = temp->next;
        }
        if (item < temp->number)
        {
            result = true;
            newNode->next = temp;
            newNode->prev = temp->prev;
            
            if (temp->prev)
                temp->prev->next = newNode;
            else
                top = newNode;
            
            temp->prev = newNode;
        }
        else if (item > temp->number)
        {
            result = true;
            newNode->prev = temp;
            temp->next = newNode;
        }
    }
    
    if (result)
        size++;
    else
        free (newNode);

    return result;
}


// removes the item from the Table
Boolean removeItem (int item)
{
    Boolean result = false;
    node * temp = _search_(item);
    
    if (temp)
    {
        if (temp->prev)
            temp->prev->next = temp->next;
        else
            top = temp->next;
        if (temp->next)
            temp->next->prev = temp->prev;
        free (temp);
        result = true;
        size--;
    }

    return result;
}


// search for given number, return pointer to node if found or NULL otherwise
static node * _search_ (const int number)
{
    node * result = top;
    
    while ( (result) && (result->number != number) )
        result = result->next;
    
    return result;
}


// whether or not the given item is in the Table
Boolean search (int item)
{
    return (Boolean)_search_(item);
}


// table iterators

// starts a list traversal by getting the data at top.
// returns false if top == NULL.
Boolean firstItem (int * const item)
{
    Boolean result = false;
    
    if ( top )
    {
        *item = top->number;
        traverseNode = top->next;
        result = true;
    }
    
    return result;
}

// gets the data at the current traversal node and increments the traversal.
// returns false if we're at the end of the list.
Boolean nextItem (int * const item)
{
    Boolean result = false;
    
    if ( traverseNode )
    {
        *item = traverseNode->number;
        traverseNode = traverseNode->next;
        result = true;
    }
    
    return result;
}



