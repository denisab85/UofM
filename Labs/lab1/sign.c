//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: Lab 1, QUESTION: exercise 2
// 
// REMARKS: sign each word from std input
//
//-----------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int charcomp(char *x, char *y)
{
	return *x - *y;
}

#define WORDMAX 100

void tolowerstr (char str[WORDMAX])
{
	for ( ; *str; ++str) *str = tolower(*str);
}


int main(void)
{
	char word[WORDMAX];
	char sig[WORDMAX];
	while (EOF != scanf("%s", word))
	{
		tolowerstr(word);
		strcpy(sig, word);
		qsort(sig, strlen(sig), sizeof(char), charcomp);
		printf("%s %s\n", sig, word);
	}
	return 0;
}
