//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: 1
//
// REMARKS: remake of Biggest.java
//
//-----------------------------------------


#include <stdio.h>
#include <string.h>
#include <ctype.h>

const int MAXLEN = 256;

int testsExecuted = 0;
int testsFailed = 0;

int charcat (char *string, char ch)
{
    size_t cur_len = strlen(string);
    if(cur_len < 254)
    {
        string[cur_len] = ch;
        string[cur_len+1] = '\0';
    }
    else
        return 1;
    return 0;
}

char *longestWord(char *string)
{
    int pos = 0;
    char longest[MAXLEN] = "";
    int maxLength = 0;
    char current[MAXLEN] = "";
    int currentLength = 0;
    char ch = '\0';
    char line[MAXLEN] = "";
    
    strcpy(line, string);
    strcat (line, " ");
    while (pos < strlen(line))
    {
        ch = line[pos];
        if ((ch == '\'' || ch == '-') && pos > 0 &&
            isalpha (line[pos - 1]) &&
            isalpha (line[pos + 1]))
        {
            charcat (current, ch);
        }
        else if (isalnum(ch))
        {
            
            charcat (current, ch);
            currentLength++;
        }
        else
        {
            if (currentLength > maxLength)
            {
                strcpy (longest, current);
                maxLength = currentLength;
            }
            
            strcpy(current, "");
            currentLength = 0;
        }
        
        pos++;
    }
    
    return longest;
}


void testLongestWord(char *line, char *expected)
{
    char result[MAXLEN] = "";
    strcpy(result, longestWord(line));
    
    if (0 == strcmp(result, expected))
    {
        printf("Passed: '%s' from '%s'\n", result, line);
    }
    else
    {
        printf("FAILED: '%s' instead of '%s' from '%s'\n", result, expected, line);
        testsFailed++;
    }
    
    testsExecuted++;
}


int main(int argc, const char * argv[]) {
    printf("Testing typical cases, including punctuation\n\n");
    testLongestWord("the quick brown foxes jumped over the lazy dogs", "jumped");
    testLongestWord("hello world she said", "hello");
    testLongestWord("Hello\tworld\tshe\tsaid", "Hello");
    testLongestWord("HELLO, world she said", "HELLO");
    testLongestWord("hello world! she said???", "hello");
    testLongestWord("\"hello world!\", she said.", "hello");
    testLongestWord("easy as abc123", "abc123");
    testLongestWord("easy as abc,123", "easy");
    
    printf("\nTesting empty cases\n\n");
    testLongestWord("", "");
    testLongestWord("!", "");
    testLongestWord(" ", "");
    testLongestWord("\t", "");
    testLongestWord("      ", "");
    testLongestWord("# $ ? % !", "");
    
    printf("\nTesting edge cases\n\n");
    testLongestWord("a", "a");
    testLongestWord("abc", "abc");
    testLongestWord("abc d e f ghi", "abc");
    testLongestWord("a a b cc dd abc", "abc");
    testLongestWord("\"a a b cc dd abc.\"", "abc");
    
    printf("\nTesting apostrophes and dashes\n\n");
    testLongestWord("this isn't five chars", "chars");
    testLongestWord("this should've been eight chars said the computer", "should've");
    testLongestWord("'this should've been eight chars', said the computer", "should've");
    testLongestWord("'hello world!', she said softly.", "softly");
    testLongestWord("topsy-turvy is a tenletter word", "topsy-turvy");
    testLongestWord("topsy-turvy should not be incorrectly eleven characters", "incorrectly");
    testLongestWord("---in-between-these---", "in-between-these");
    testLongestWord("---in---between---these---", "between");
    testLongestWord("here-is-an-edge-case but a muchmuchlongerword", "muchmuchlongerword");
    testLongestWord("d-o-n't-g-o-o-v-e-r-t-h-e-e-d-g-e with muchmuchlongerwords", "muchmuchlongerwords");
    testLongestWord("two=five-3 isn't three", "three");
    
    printf("\nThese tests will be opposite in the C version\n\n");
    testLongestWord("the word antidisestablishmentarianism is very long but not as long as 'Llanfairpwllgwyngyllgogerychwyrndrobwyll-llantysiliogogogoch'.", "Llanfairpwllgwyngyllgogerychwyrndrobwyll-llantysiliogogogoch");
    testLongestWord("the word antidisestablishmentarianism is very long but not as long as 'Llanfairpwllgwyngyllgogerychwyrndrobwyll-llantysiliogogogoch'.", "antidisestablishment");
    testLongestWord("Java strings may contain \0 in the interior", "interior");
    testLongestWord("C strings cannot contain \0 in the interior", "strings");
    
    printf("\nTotal number of tests executed: %d\n", testsExecuted);
    printf("Number of tests passed:         %d\n", (testsExecuted - testsFailed));
    printf("Number of tests failed:         %d\n", testsFailed);
    return 0;
}
