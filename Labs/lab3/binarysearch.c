//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: Lab 3
//
// REMARKS: binary search
//
//-----------------------------------------


#include <stdio.h>
#include <time.h>
#include <stdlib.h>


//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

#define MAX_SIZE 1024
#define MAX_TESTS 10240

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

int testsExecuted = 0;
int testsFailed = 0;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

int binarysearch(int sorted_array[], int size, int num);

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------


int main(int argc, const char * argv[]) {
    int sorted_array[MAX_SIZE];
    int size = 0;
    int num = -1;
    int i = 0;
    int elem = 0;
    int result_expected = -2;
    int negative = -1;
    int result = -2;
    
    while ( testsExecuted < MAX_TESTS )
    {
        testsExecuted++;
        i = 0;
        size = 0;
        elem = 0;
        num  = 0;
        result_expected = 0;
        negative = -1;
        srand((unsigned)time(NULL));
        size = rand() % MAX_SIZE;
        while (i < size)
        {
            elem = elem + rand() % 10 + 1;
            // somewhere in the middle of the array eliminate one element to use it in negative testing
            if ((negative == -1) && (i*2 >= size))
            {
                negative = elem;
            }
            else
            {
                sorted_array[i] = elem;
                i++;
            }
        }
        result_expected = rand() % size;
        // provide decent number of 'boundary' and negative test cases
        switch (result_expected % 7)
        {
            case 0:
                result_expected = 0;
                break;
            case 1:
                result_expected = size - 1;
                break;
            case 2:
                result_expected = -1;
                num = negative;
                break;
        }
        
        if (result_expected != -1)
        {
            num = sorted_array[result_expected];
        }
        
        result = binarysearch(sorted_array, size, num);
        
        if (result == result_expected)
        {
           // printf(".");
        }
        else
        {
            testsFailed++;
            for(i = 0; i < size; i++)
            {
                printf("%d:%d ", i, sorted_array[i]);
            }
            printf("\n\nTest failed with result: %d\n", result);
            printf("\nsize = %d\nnum = %d\nresult expected = %d\n", size, num, result_expected);

        }
    }
    printf("Tests executed: %d\n", testsExecuted);
    printf("Tests failed:   %d\n\n", testsFailed);
    return 0;
}


int binarysearch(int sorted_array[], int size, int num)
{
    /* return (any) position if t in sorted x[0..n-1]
     or -1 if t is not present */
    int lower = 0;
    int upper = 0;
    int mid = 0;
    int result = -1;
    
    lower = 0;
    upper = size - 1;
    while ((lower <= upper) && (result == -1))
    {
        mid = (lower + upper) / 2;
        if (sorted_array[mid] < num)
        {
            lower = mid + 1;
        }
        else if (sorted_array[mid] == num)
        {
            result = mid;
        }
        else
        {
            upper = mid - 1;
        }
    }
    return result;
}