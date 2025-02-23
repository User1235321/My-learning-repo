package second;

import java.util.Scanner;
import java.util.TreeMap;

public class MainFile
{
  public static void main(String[] args)
  {
    System.out.println("Programm start. Add your words and it's translate in next format: word | translate");
    System.out.println("To stop adding words print stop");

    Scanner scan = new Scanner(System.in);
    String str = scan.nextLine();
    TreeMap< String, String > dictionary = new TreeMap<>((s1, s2) -> Integer.compare(s2.length(), s1.length()));

    while (!str.equals("stop"))
    {
      try
      {
        String [] parts = str.split(" \\| ");
        if (str.equals(parts[0]))
        {
          throw new InvalidFileFormatException("Bad input format! May be you forgot | or spaces. Remind format: word | translate");
        }
        else
        {
          dictionary.put(parts[0], parts[1]);
        }
      }
      catch (InvalidFileFormatException e)
      {
        System.err.println("Invalid input: " + e.getMessage());
      }
      str = scan.nextLine();
    }

    System.out.println("You stop adding words. Now print name of file with text");
    str = scan.nextLine();
    Translator trns = new Translator(dictionary);
    try
    {
      trns.translate(str);
    }
    catch (FileReadException e)
    {
      System.err.println("Error: " + e.getMessage());
    }
    System.out.println("Programm ended");
  }
}
