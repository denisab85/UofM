//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: 3
// QUESTION: 1
//
// REMARKS: Testing framework for the table project
//
//-----------------------------------------


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "table.h"

static const int MAX_LINE = 80;
static const int MAX_STR  = 1024;


char * readln(char* line, FILE * input)
{
    char * result = NULL;
    if ((result = fgets (line, MAX_LINE, input)))
    {
        if (line[strlen(line)-1] == '\n')   // trim trailing LF
            line[strlen(line)-1] = '\0';
    }
    return result;
}


int main(int argc, const char * argv[]) {
    FILE * input = NULL;
    char inputPath[MAX_STR];          // path to the input file
    char line[MAX_LINE] = "";
    int num = 0;
    int stage = 0;
    char tempStr[MAX_STR] = "";
    
    if (argc > 1)
    {
        strcpy (inputPath, argv[1]);
        input = fopen(inputPath, "r" );
        assert ( input != NULL );
        if ( input != NULL )
        {
            while (readln (line, input))
            {
                if (line[0] == '@')   // detect next test
                {
                    printf ("\nStarting: %s.\n", strcpy(tempStr, &line[1]));
                    stage = 0;
                }
                else
                {
                    if (line[0] == '#')   // detect next stage
                    {
                        printf ("   stage %d: %s\n", stage, strcpy(tempStr, &line[1]));
                        readln (line, input);
                        stage++;
                    }
                    num = atoi(line);
                }
                
                switch (stage) {
                    case 0:     // initialize table
                        clearTable ();
                        stage++;
                        break;
                    case 1:
                        insertItem (num);
                        break;
                    case 2:     // negative search
                        if (search(num))
                            printf("     Error: '%d' found.\n", num);
                        break;
                    case 3:     // positive search
                        if (! search(num))
                            printf("     Error: '%d' not found.\n", num);
                        break;
                    case 4:     // negative insert
                        if (insertItem(num))
                            printf("     Error: '%d' inserted.\n", num);
                        break;
                    case 5:     // positive insert
                        if (! insertItem(num))
                            printf("     Error: '%d' not inserted.\n", num);
                        break;
                    case 6:     // negative deletion
                        if (removeItem(num))
                            printf("     Error: '%d' deleted.\n", num);
                        break;
                    case 7:     // positive deletion
                        if (! removeItem(num))
                            printf("     Error: '%d' not deleted.\n", num);
                        break;
                    case 8:     // negative search
                        if (search(num))
                            printf("     Error: '%d' found.\n", num);
                        break;
                    case 9:     // positive search
                        if (! search(num))
                            printf("     Error: '%d' not found.\n", num);
                        break;

                    default:
                        break;
                }
                
                
            }
            printf ("\nAll tests finished.\n\n");
            fclose(input);
        }
    }
    
    return 0;
    
}
