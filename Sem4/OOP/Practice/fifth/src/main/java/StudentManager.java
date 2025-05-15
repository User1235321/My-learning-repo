package fifth;

import java.util.*;
import java.io.PrintWriter;
import java.io.FileNotFoundException;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

public class StudentManager extends Object
{
  private List< Student > students_;

  public StudentManager()
  {
    students_ = new ArrayList();
  }
  public void addStudent(Student newStud)
  {
    students_.add(newStud);
  }
  public void removeStudent(int id) throws EmptyStudentException
  {
    students_.remove(findStudent(id));
  }
  public Student findStudent(int id) throws EmptyStudentException
  {
    for (Student student : students_)
    {
      if (student.getId() == id)
      {
        return student;
      }
    }
    throw new EmptyStudentException("Student now found!");
  }
  public void saveToJson(String fileName) throws FileNotFoundException
  {
    PrintWriter writer = new PrintWriter(fileName);
    Gson gson = new GsonBuilder().setPrettyPrinting().create();
    writer.println(gson.toJson(students_));
    writer.close();
  }
  public void loadFromJson(String fileName)
  {
    //
  }
}
