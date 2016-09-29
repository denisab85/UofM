//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: Lab 2
//
// REMARKS: mail merge
//
//-----------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_LENGTH 128
#define FIELD_LENGTH 30
#define NUM_FIELDS   9



int split(char *str, char output[NUM_FIELDS][FIELD_LENGTH])
{
    char * delim = "|";
    char * token = strtok (str, delim);
    int count = 0;
    
    while (token != NULL)
    {
        strcpy(output[count++], token);
        token = strtok (NULL, delim);
    }
    return 0;
}



int main( int argc, char *argv[] )
{
    FILE *template = NULL;
    FILE *data = NULL;
    
    char input[INPUT_LENGTH];                     // a string for reading from a file
    char customerData[NUM_FIELDS][FIELD_LENGTH];  // an array of strings for storing the results of a "split"
    
    // these 3 variables are useful for processing the text input
    int element = 0;
    //char *next;
    char ch = '\0';
    
    char *templatePath = '\0';
    char *dataPath = '\0';
    
    
    if (argc > 2)
    {
        templatePath = argv[1];
        dataPath = argv[2];
    }
    else
    {
        printf("Usage: %s [template.txt] [data.txt]\n", argv[0]);
        exit(1);
    }
    // yes, we're hard-coding file names... note that this is *unacceptable*, unless we explicitly tell you to do it
    template = fopen(templatePath, "r" );
    if ( template != NULL )
    {
        // read in the customers until we're done
        data = fopen(dataPath, "r" );
        if ( data != NULL )
        {
            while ( fgets( input, INPUT_LENGTH, data ) != NULL )
            {
                // Tokenizing the line and putting the data into the customerData array ala split in Java
                split(input, customerData);
                
                // generate the output by reading and parsing the template
                // instead of reading it into a buffer it just re-reads the file each time
                rewind( template );
                while ( fgets( input, INPUT_LENGTH, template ) != NULL )
                {
                    // Process the input one character at a time looking for variables to replace with customerData
                    element = 0;
                    ch = 0;
                    while (ch != '\n')
                    {
                        ch = input[element];
                        if (ch == '$')
                        {
                            element++;
                            ch = input[element];
                            if (ch == '$')
                                printf("%c", ch);
                            else
                                if ((ch >= '0') && (ch <= '9'))
                                {
                                    int index = ch - '0';
                                    printf("%s", customerData[index]);
                                }
                        }
                        else
                        {
                            printf("%c", ch);
                        }
                        
                        element++;
                    }
                }
            }
            
            fclose( data );
        }
        
        fclose( template );
    }
    
    return EXIT_SUCCESS;
}