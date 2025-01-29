package second;

import java.util.Scanner;
import java.util.TreeMap;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.Map;
import java.util.regex.Pattern;

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
      catch (InvalidFileFormatException err)
      {
        System.out.println("Invalid input: " + err.getMessage());
      }
      str = scan.nextLine();
    }

    System.out.println("You stop adding words. Now print name of file with text");
    str = scan.nextLine();
    FileReader file;
    try
    {
      file = new FileReader(str);
    }
    catch(FileNotFoundException err)
    {
      System.out.println("Uncorrect file name");
      return;
    }
    BufferedReader reader = new BufferedReader(file);
    try
    {
      str = reader.readLine();
    }
    catch (IOException err)
    {
      System.out.println("Error reading from file, programm ended!");
      return;
    }

    while (str != null)
    {
      for(Map.Entry< String , String > entry : dictionary.entrySet()) {
        String word = entry.getKey().toLowerCase();
        if (str.toLowerCase().contains(word))
        {
          str = str.replaceAll("(?i)" + Pattern.quote(word), entry.getValue());
        }
      }
      System.out.println(str);
      try
      {
        str = reader.readLine();
      }
      catch (IOException err)
      {
        System.out.println("Error reading from file, programm ended!");
        return;
      }
    }
    System.out.println("Programm ended");
  }
}
