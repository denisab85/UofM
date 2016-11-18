#ifndef _SET_H
#define _SET_H

typedef enum BOOL { false, true } Boolean;

typedef struct SET Set;

// Returns true if there are currently *no* set objects in existence.
Boolean validateMemUse();

// Set constructor
Set * newSet();
//Set destructor
Set * deleteSet(Set * theSet);

// Insert an item into a set, noting that sets do not contain duplicate items.
// Returns true if the item was added to the set.
Boolean insertItem(Set * const theSet, const int newItem);
// Remove a given item, if it appears in the set.
// Returns true if the item was removed from the set.
Boolean removeItem(Set * const theSet, const int givenItem);

// Returns true if the two sets contain the same items.
Boolean equals(Set const * const setA, Set const * const setB);

// Return a new set that is the union of two sets (the two sets should be 
// unchanged by the operation).
//
// The union of two sets A and B is defined to be another set
// containing all the items in A and all the items in B.
// (Note: Any item that is in BOTH A and B appears only once
// in the union, because no set contains duplicate items.)
// Example: If A = { 1, 2, 3 } and B = { 2, 4, 5, 6 }, then
// the union of A and B is the set { 1, 2, 3, 4, 5, 6 }.
Set * unionOf(Set const * const setA, Set const * const setB);

// Return a new set that is the intersection of two sets (the two sets should be 
// unchanged by the operation).
//
// The intersection of two sets A and B is defined to be another set
// containing all the items in both A and B.
// Example: If A = { 1, 2, 3, 4 } and B = { 2, 3, 5 }, then
// the intersection of A and B is the set { 2, 3 }.
Set * intersectionOf(Set const * const setA, Set const * const setB);

// Return a new set that is the difference of two sets (the two sets should be 
// unchanged by the operation).
//
// The difference (A\B) of two sets A and B is defined to be
// another set containing all the items in A that are NOT in B.
// Example: If A = { 1, 2, 3 } and B = { 2, 4, 5, 6 }, then
// the difference A\B of A and B is the set { 1, 3 }.
Set * differenceOf(Set const * const setA, Set const * const setB);

#endif