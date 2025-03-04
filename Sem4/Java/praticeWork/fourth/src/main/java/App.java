package fourth;

import java.util.*;
import java.io.PrintWriter;
import java.io.FileNotFoundException;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import org.apache.commons.csv;

public class App
{
  public static void main(String[] args)
  {
    List< Student > studentList = Arrays.asList(
      new Student(0, "Ivan", 18, 1, 4),
      new Student(1, "Igor", 21, 3, 3),
      new Student(2, "Innokentii", 40, 1, 5));

    try(PrintWriter writer = new PrintWriter("Students.json");)
    {
    Gson gson = new GsonBuilder().setPrettyPrinting().create();
    //JSONObject jsonObject = new JSONObject(gson.toJson(studentList));
    //writer.println(jsonObject.toString(4));
    writer.println(gson.toJson(studentList));
    writer.close();
    }
    catch (FileNotFoundException e)
    {
      System.err.println("Can't open json file to write");
    }
  }
}
