//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: Lab 1, QUESTION: exercise 2
// 
// REMARKS: print all equal members of a sorted list on a single line
//
//-----------------------------------------

#include <stdio.h>
#include <string.h>

#define WORDMAX 100

int main(void)
{
	char word[WORDMAX];
	char sig[WORDMAX];
	char oldsig[WORDMAX];
	int linenum = 0;
	strcpy(oldsig, "");
	while (EOF != scanf("%s %s", sig, word))
	{
		if ((0 != strcmp(oldsig, sig)) && (linenum > 0))
		{
			printf("\n");
		}
		strcpy(oldsig, sig);
		linenum++;
		printf("%s ", word);
	}
	printf("\n");
	return 0;
}
