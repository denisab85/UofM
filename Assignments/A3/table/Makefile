#-----------------------------------------
# NAME: Denis Abakumov
# STUDENT NUMBER: 7823110
# COURSE: COMP 2160, SECTION: A02
# INSTRUCTOR: Michael Zapp
# ASSIGNMENT: 3
# QUESTION: 1
#
# REMARKS: Makefile for the table project
#
#-----------------------------------------


CC=clang
CFLAGS=-I.
DEPS = table.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

table: table.o main.o
	$(CC) -o table table.o main.o -I.

clear:
	rm -f ./*.o ./table