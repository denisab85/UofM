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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "set.h"


// max string length handled
static const int MAX_STR  = 1024;

// max number of set entries read from input file
static const int MAX_NUM  = 256;

// testing finishes after max empty lines read from input file
static const int MAX_EMPTY_LINES = 2;


int readln(char* line, FILE * input)
{
    int result = -1;
    if ((fgets (line, MAX_STR, input)) && strlen (line))
    {
        if (line[strlen(line)-1] == '\n')   // trim trailing LF
            line[strlen(line)-1] = '\0';
        result = (int) strlen (line);
    }
    return result;
}

void parseParams (char * str, int numbers[MAX_NUM])
{
    char * delim = ",";
    char * token = strtok (str, delim);
    int count = 1;
    
    while ( (token != NULL) && (count <= MAX_NUM) )
    {
        numbers[count++] = atoi (token);
        token = strtok (NULL, delim);
    }
    numbers[0] = count-1;
}


Boolean setFromArray(void * set, int numbers[MAX_NUM])
{
    Boolean result = true;
    Boolean res = false;
    int i = 0;
    int n = 0;
    
    for (i=1; i<=numbers[0]; i++)
    {
        n = numbers[i];
        res = insertItem (set, n);
        if (! res)
            printf("     Error: '%d' not inserted.\n", numbers[i]);
        result = ( result && res );
    }
    assert(set);
    return result;
}


void testDeletion (void * set)
{
    // test deletion
    if (set)
    {
        void * deleted = deleteSet(set);
        assert (deleted == NULL);
        if (deleted)
            printf("Error: set was not properly deleted!");
        
    }
}


Boolean runTest (int test, int operands[3][MAX_NUM])
{
    int result = -1;
    void * set1 = newSet();
    void * set2 = newSet();
    void * expected = newSet();
    void * result_set = NULL;
    setFromArray(set1, operands[0]);
    setFromArray(set2, operands[1]);
    setFromArray(expected, operands[2]);
    
    switch (test) {
        case 1:   // Comparison   [c]
            result = ( equals (set1, set2) == (Boolean) operands[2][1]);
            break;
        case 2:   // Union        [u]
            result_set = unionOf(set1, set2);
            result = equals(result_set, expected);
            break;
        case 3:   // Intersection [i]
            result_set = intersectionOf(set1, set2);
            result = equals(result_set, expected);
            break;
        case 4:   // Difference   [d]
            result_set = differenceOf(set1, set2);
            result = equals(result_set, expected);
            break;
        default:
            printf(": UNKNOWN OPERATOR.\n");
            break;
    }
    
    testDeletion(set1);
    testDeletion(set2);
    testDeletion(expected);
    testDeletion(result_set);
    
    return result;
}


int main(int argc, const char * argv[]) {
    FILE * input = NULL;
    char inputPath[MAX_STR];          // path to the input file
    char line[MAX_STR] = "";
    int test = 0;
    int i = 0;
    int operands[3][MAX_NUM];
    char testName[MAX_STR] = "";
    char testDescr[MAX_STR] = "";
    int lineEmpty = 0;
    Boolean result;
    
    if (argc > 1)
    {
        strcpy (inputPath, argv[1]);
        input = fopen(inputPath, "r" );
        assert ( input != NULL );
        
        // test validateMemUse()
        if (! validateMemUse())
            printf("Error: function validateMemUse() returned wrong results!");
        
        if (input != NULL)
        {
            while ( (readln (line, input) >= 0) && (test >= 0) && (lineEmpty < MAX_EMPTY_LINES) )
            {
                if (strlen(line))
                {
                    lineEmpty = 0;
                    switch (line[0])
                    {
                        case 'c':
                            strcpy (testName, "Compare  ");
                            test = 1;
                            break;
                        case 'u':
                            strcpy (testName, "Union    ");
                            test = 2;
                            break;
                        case 'i':
                            strcpy (testName, "Intersect");
                            test = 3;
                            break;
                        case 'd':
                            strcpy (testName, "Diff     ");
                            test = 4;
                            break;
                        default:  // finish testing on any unrecognized character
                            test = -1;
                            break;
                    }
                    
                    if ( (4 >= test > 0) )
                    {
                        strcpy(testDescr, line);
                        i = 0;
                        while ((i < 3) && readln (line, input) >= 0)
                        {
                            parseParams (line, operands[i]);
                            i++;
                        }
                        
                        result = runTest (test, operands);
                        printf ( "%s: %s  - %s\n", testName, (result?"PASS":"FAIL"), testDescr );
                    }
                }
                else
                    lineEmpty++;
            }
            
            fclose(input);
            
            // test validateMemUse()
            if (! validateMemUse())
                printf("Error: function validateMemUse() returned wrong results!");
            
            printf ("\nAll tests finished.\n\n");
        }
    }
    
    return 0;
    
}
