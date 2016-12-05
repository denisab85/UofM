//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: 3
// QUESTION: 2
//
// REMARKS: Testing framework for the set project
//
//-----------------------------------------


To compile the set project, run:
> make

To start a test, run:
> ./main ./sequences.txt

Or, rather, to run a series of tests against all different set implementations
1. put all set-trial*.zip files in the same directory and
2. use compile.sh (w/o arguments)


=================================

TESTING REPORT:

Set Trial 1
	Does not handle maximum integer of 2147483647

Set Trial 2
	Does not handle maximum integer of 2147483647
	Intersection creates a private object that is never freed


Set Trial 3
	Does not handle maximum integer of 2147483647
	Performs incorrect union if there are common items in the sets

Set Trial 4
	Does not handle maximum integer of 2147483647
	Comparing 2 equal sets gives "False" instead of "True"
	Results for other operations are shadowed by incorrect 'equals'

Set Trial 5
	Trying to handle maximum integer of 2147483647 leads to infinite loop

Set Trial 6
	Does not handle maximum integer of 2147483647
	Performs incorrect diff if there are common items in the sets
	Performs incorrect intersetcion if the sets are not equal or not both empty.

Set Trial 7
	Does not handle maximum integer of 2147483647
	Union gives incorrect results if at least 1 of the sets is not empty (i.e. only correctly unites 2 empty sets)
	Performs incorrect diff if the sets are not equal or not both empty.
	Performs incorrect intersection if there are common items in the sets.
