import java.util.*;
import java.io.*;

public class Ruler
{
  public static void rule(int left, int right, int height, int theRuler[])
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
    
    return;
  }
  
  
  public static double log2(double val)
  {
    assert(val > 0);
    return (Math.log(val)/Math.log(2));
  }
  
  
  public static void printRuler(int length)
  {
    assert(length > 0);
    
    int theRuler[] = new int[length+1];
    int i;
    
    for (i = 0; i <=length; i++)
    {
      theRuler[i] = 0;
    }
    
    rule(0,length,(int)(log2(length)),theRuler);
    
    for (i = 0; i <=length; i++)
    {
      System.out.print(theRuler[i]);
    }
    System.out.println("");
    
    for (i = 0; i <=length; i++)
    {
      System.out.print(i % 10);
      
    }
    System.out.println("");
  }
  
  public static void main(String args[])
  {
    boolean done = false;
    int length;
    String inputLine;
    Scanner in;
    
    in = new Scanner(System.in);
    
    do
    {
      System.out.print("Enter a power of 2, which is at least 2, or q to quit: ");
      inputLine = in.nextLine();
      
      if (inputLine.length() > 0)
      {
        if (inputLine.charAt(0) == 'q')
        {
          done = true;
        }
        else
        {
          length = -1;
          try
          {
            length = Integer.parseInt(inputLine);
          }
          catch(NumberFormatException e)
          {
            System.out.println("Hey, that's not a number!");
          }
          
          if (length != -1)
          {
            if ( (length < 2) || ( Math.ceil(log2(length)) != (int)(log2(length)) ) )
            {
              System.out.println("Please be sure to enter a power of 2 that is >= 2.");
            }
            
            // finally!!!
            else
            {
              printRuler(length);
            }
          }
        }
      }
    }
    while (!done);

    System.out.println("End of Processing");
    System.exit(0);
  }
}
