public class Biggest 
{
  private static int testsExecuted = 0;
  private static int testsFailed = 0;
  
  public static void main(String[] args) 
  {
    System.out.println("Testing typical cases, including punctuation\n");
    testLongestWord("the quick brown foxes jumped over the lazy dogs", "jumped");
    testLongestWord("hello world she said", "hello");
    testLongestWord("Hello\tworld\tshe\tsaid", "Hello");
    testLongestWord("HELLO, world she said", "HELLO");
    testLongestWord("hello world! she said???", "hello");
    testLongestWord("\"hello world!\", she said.", "hello");
    testLongestWord("easy as abc123", "abc123");
    testLongestWord("easy as abc,123", "easy");
    
    System.out.println("\nTesting empty cases\n");
    testLongestWord("", "");
    testLongestWord("!", "");
    testLongestWord(" ", "");
    testLongestWord("\t", "");
    testLongestWord("      ", "");
    testLongestWord("# $ ? % !", "");
    
    System.out.println("\nTesting edge cases\n");
    testLongestWord("a", "a");
    testLongestWord("abc", "abc");
    testLongestWord("abc d e f ghi", "abc");
    testLongestWord("a a b cc dd abc", "abc");
    testLongestWord("\"a a b cc dd abc.\"", "abc");
    
    System.out.println("\nTesting apostrophes and dashes\n");
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
    
    System.out.println("\nThese tests will be opposite in the C version\n");
    testLongestWord("the word antidisestablishmentarianism is very long but not as long as 'Llanfairpwllgwyngyllgogerychwyrndrobwyll-llantysiliogogogoch'.", "Llanfairpwllgwyngyllgogerychwyrndrobwyll-llantysiliogogogoch");
    testLongestWord("the word antidisestablishmentarianism is very long but not as long as 'Llanfairpwllgwyngyllgogerychwyrndrobwyll-llantysiliogogogoch'.", "antidisestablishment");
    testLongestWord("Java strings may contain \0 in the interior", "interior");
    testLongestWord("C strings cannot contain \0 in the interior", "strings");
    
    System.out.println("\nTotal number of tests executed: " + testsExecuted);
    System.out.println("Number of tests passed:         " + (testsExecuted - testsFailed));
    System.out.println("Number of tests failed:         " + testsFailed);
  }
  
  public static void testLongestWord(String line, String expected) 
  {
    String result = longestWord(line);
    
    if (result.equals(expected)) 
    {
      System.out.println("Passed: '" + result + "' from '" + line + "'");
    } 
    else 
    {
      System.out.println("FAILED: '" + result + "' instead of '" + expected + "' from '" + line + "'");
      testsFailed++;
    }
    
    testsExecuted++;
  }
  
  public static String longestWord(String line) 
  {
    int pos = 0;
    String longest = "";
    int longestLength = 0;
    String current = "";
    int currentLength = 0;
    char ch;
    
    line += ' ';
    while (pos < line.length()) 
    {
      ch = line.charAt(pos);
      
      if ((ch == '\'' || ch == '-') && pos > 0 && 
          Character.isLetter(line.charAt(pos - 1)) && 
          Character.isLetter(line.charAt(pos + 1))) 
      {
        
        current += ch;
        
      } 
      else if (Character.isLetterOrDigit(ch)) 
      {
        
        current += ch;
        currentLength++;
        
      }
      else 
      {
        
        if (currentLength > longestLength) 
        {
          longest = current;
          longestLength = currentLength;
        }
        
        current = "";
        currentLength = 0;
        
      }
      
      pos++;
    }
    
    return longest;
  }
}