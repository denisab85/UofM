#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_LENGTH 128
#define FIELD_LENGTH 30
#define NUM_FIELDS   9

int main( int argc, char *argv[] )
{
  FILE *template = NULL;
  FILE *data = NULL;

  char input[INPUT_LENGTH];                     // a string for reading from a file
  char customerData[NUM_FIELDS][FIELD_LENGTH];  // an array of strings for storing the results of a "split"
  
  // these 3 variables are useful for processing the text input
  int  element = 0;
  char *next;
  char ch;

  // yes, we're hard-coding file names... note that this is *unacceptable*, unless we explicitly tell you to do it
  template = fopen( "template.txt", "r" );
  if ( template != NULL )
  {
    // read in the customers until we're done
    data = fopen( "data.txt", "r" );
    if ( data != NULL )
    {
      while ( fgets( input, INPUT_LENGTH, data ) != NULL )
      {
        // INSERT PARSING CODE HERE! 
        // You're tokenizing the line and putting the data into the customerData array ala split in Java

        // generate the output by reading and parsing the template
        // instead of reading it into a buffer it just re-reads the file each time
        rewind( template );
        while ( fgets( input, INPUT_LENGTH, template ) != NULL )
        {
          // INSERT PARSING CODE HERE!
          // Process the input one character at a time looking for variables to replace with customerData
        }
      }
      
      fclose( data );
    }

    fclose( template );
  }

  return EXIT_SUCCESS;
}