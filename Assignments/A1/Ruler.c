//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: 1
//
// REMARKS: remake of Ruler.java
//
//-----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <limits.h>

const int MAXLEN = 256;

int rule(int left, int right, int height, int theRuler[])
{
    assert(left <= right);
    assert(right >= 0);
    assert(height >= 0);
    
    int mid = 0;
    
    if (height == 1)
    {
        mid = (right+left)/2;
        theRuler[mid] = height;
    }
    else
    {
        mid = (right+left)/2;
        theRuler[mid] = height;
        
        rule(left,mid,height-1, theRuler);
        rule(mid,right,height-1, theRuler);
    }
    
    return 0;
}


double log2(double val)
{
    assert(val > 0);
    return (log(val)/log(2));
}


int printRuler(int length)
{
    assert(length > 0);
    
    int theRuler[length+1];
    int i;
    
    for (i = 0; i <=length; i++)
    {
        theRuler[i] = 0;
    }
    
    rule(0,length,(int)(log2(length)),theRuler);
    
    for (i = 0; i <=length; i++)
    {
        printf("%d", theRuler[i]);
    }
    printf("\n");
    
    for (i = 0; i <=length; i++)
    {
        printf("%d", i % 10);
    }
    printf("\n");
    return 0;
}


int isNum(const char *string) {
    size_t length = strlen (string);
    int i = 0;
    if ( (string[0] != '-') && (string[0] != '+') && (! isdigit(string[0])) )
        return 0;
    for (i=1; i < length; i++)
        if (! isdigit(string[i]))
            return 0;
    return 1;
}


int main(int argc, const char * argv[])
{
    char done = 0;
    int length;
    char inputLine[MAXLEN] = "";
    
    do
    {
        printf("Enter a power of 2, which is at least 2, or q to quit: ");
        if (fgets(inputLine, sizeof(inputLine), stdin))
        {
            inputLine[strlen(inputLine)-1] = '\0';
            if (strlen(inputLine) > 0)
            {
                if (inputLine[0] == 'q')
                {
                    done = 1;
                }
                else if ( ! isNum(inputLine) )
                {
                    printf("Hey, that's not a number!\n");
                }
                else
                {
                    length = atoi(inputLine);
                    if ( (length < 2) || (ceil(log2(length)) != (int)(log2(length)) ) )
                    {
                        printf("Please be sure to enter a power of 2 that is >= 2.\n");
                    }
                    else
                    {
                        printRuler(length);
                    }
                }
            }
        }
        else
        {
            clearerr(stdin);
            printf("\n");
        }
        
    }
    while (!done);
    
    printf("End of Processing\n");
    return 0;
}

