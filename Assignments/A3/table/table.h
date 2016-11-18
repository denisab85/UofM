//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: 3
// QUESTION: 1
//
// REMARKS: Implementation of a table
//
//-----------------------------------------


#ifndef _TABLE_H
#define _TABLE_H

//-----------------------------------------------------------------------------
// CONSTANTS AND TYPES
//-----------------------------------------------------------------------------

typedef enum BOOL { false, true } Boolean;

// Linked list node definition
typedef struct Node node;

struct Node
{
    int   number;
    node  *next;
    node  *prev;
};


//-------------------------------------------------------------------------------------
// FILE-SCOPE VARIABLES
//-------------------------------------------------------------------------------------

static node *top = NULL;

// used to track where we are for the list traversal methods
static node *traverseNode = NULL;

static int size = 0;


//-----------------------------------------------------------------------------
// PUBLIC PROTOTYPES
//-----------------------------------------------------------------------------

// add an element to the table 
Boolean insertItem( int item );

// removes the int from the table
Boolean removeItem( int item );

// empty the table so that we clear all memory and can start a fresh table
void clearTable( );

// tells us whether or not the given item is in the table
Boolean search( int item );

// table iterators
Boolean firstItem( int * const item );

Boolean nextItem( int * const item );

void build( int size_ );
void print();


//-----------------------------------------------------------------------------
// STATIC PROTOTYPES
//-----------------------------------------------------------------------------

// search for given number, return pointer to node if found or NULL otherwise
static node * _search_ (const int number);

#endif
