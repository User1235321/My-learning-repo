package second;

import java.util.TreeMap;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.Map;
import java.util.regex.Pattern;

public class Translator
{
  public Translator(TreeMap< String, String > dict)
  {
    dictionary = dict;
  }
  public void translate(String fileName) throws FileReadException
  {
    FileReader file;
    try
    {
      file = new FileReader(fileName);
    }
    catch(FileNotFoundException e)
    {
      throw new FileReadException(e.getMessage());
    }
    BufferedReader reader = new BufferedReader(file);
    String str;
    try
    {
      str = reader.readLine();
    }
    catch (IOException e)
    {
      throw new FileReadException(e.getMessage());
    }

    while (str != null)
    {
      for(Map.Entry< String, String > entry : dictionary.entrySet())
      {
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
      catch (IOException e)
      {
        throw new FileReadException(e.getMessage());
      }
    }
  }
  private TreeMap< String, String > dictionary;
}
