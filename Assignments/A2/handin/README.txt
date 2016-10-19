//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: 2
// QUESTION: 1
//
// REMARKS: inplementation of Conway's Game of Life
//
//-----------------------------------------

Description:
A c implementation of Conway's Game of Life (cellular automaton).
Loads the initial state of the universe from an input file or standard I/O.
Calculates and outputs to the std I/O each of the following generations.
Determines a cycle by comparing every every generation to all the previous.
Runs for 250 generations or until a cycle is detected, whichever comes first

Input:
Supports input from a file (as 1st parameter to the executable) or standard I/O, including redirection.
In the input, several games may go one after another in the following format:
1st line: "*" (asterisk) followed by the name of the game (max 80 characters).
2nd line: 2 dimensions of the game's field separated by a space (1 to 60). First - height, then width.
3rd line and further: the initial state of the field marked-up with spaces (" ") and "X"s.
Space - for a 'dead' cell, X - for an alive.
Empty lines are allowed at any place, they will be skipped.

Output:
All output is done to std I/O in the following order:
	- the name of the game, as read from the input;
	- the initial universe;
	- if a cycle has been determined, a message is printed showing the numbers of 2 equal generations;
	- last 10 generations printed, using "." (dot) and "*" (asterisk) characters and preceded with generation number.

Restrictions:
A line representing the field should be at least the number of characters (spaces and "X"s) equal to the 2nd dimension.
The maximum dimension af a field (height or width) is 60.
The maximum length of the game's name is 80 characters.
The maximum number of geberations calculated = (1 initial + 250).

Compilation:

clang -o life ./life.c
