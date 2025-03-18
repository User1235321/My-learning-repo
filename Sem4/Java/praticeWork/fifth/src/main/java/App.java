package fifth;

import java.io.FileNotFoundException;

public class App
{
  public static void main(String[] args)
  {
    StudentManager sM = new StudentManager();
    sM.addStudent(new Student(0, "Ivan", 18, 1, 4));
    sM.addStudent(new Student(1, "Igor", 21, 3, 3));
    sM.addStudent(new Student(2, "Innokentii", 40, 1, 5));

    try
    {
      sM.saveToJson("Student.json");
    }
    catch (FileNotFoundException e)
    {
      System.err.println("Can't open json file to write");
    }
  }
}
