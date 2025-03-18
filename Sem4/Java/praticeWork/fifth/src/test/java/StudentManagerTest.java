package fifth;

import java.io.PrintWriter;
import java.io.FileNotFoundException;

import org.junit.Test;
import static org.junit.Assert.*;

public class StudentManagerTest
{
  @Test(expected = EmptyStudentException.class)
  public void testRemoveStudentFromEmpty() throws EmptyStudentException
  {
    StudentManager sM = new StudentManager();
    sM.removeStudent(15);
  }
  @Test(expected = EmptyStudentException.class)
  public void testFindStudentFromEmpty() throws EmptyStudentException
  {
    StudentManager sM = new StudentManager();
    System.out.println(sM.findStudent(15));
  }
  @Test(expected = EmptyStudentException.class)
  public void testRemoveUnrealStudent() throws EmptyStudentException
  {
    StudentManager sM = new StudentManager();
    sM.addStudent(new Student(0, "Ivan", 18, 1, 4));
    sM.addStudent(new Student(1, "Igor", 21, 3, 3));
    sM.addStudent(new Student(2, "Innokentii", 40, 1, 5));
    sM.removeStudent(15);
  }
  @Test(expected = EmptyStudentException.class)
  public void testFindUnrealStudenty() throws EmptyStudentException
  {
    StudentManager sM = new StudentManager();
    sM.addStudent(new Student(0, "Ivan", 18, 1, 4));
    sM.addStudent(new Student(1, "Igor", 21, 3, 3));
    sM.addStudent(new Student(2, "Innokentii", 40, 1, 5));
    System.out.println(sM.findStudent(15));
  }
  @Test
  public void testFindAndRemoveRealStudent() throws EmptyStudentException, FileNotFoundException
  {
    StudentManager sM = new StudentManager();
    sM.addStudent(new Student(0, "Ivan", 18, 1, 4));
    sM.addStudent(new Student(1, "Igor", 21, 3, 3));
    sM.addStudent(new Student(2, "Innokentii", 40, 1, 5));
    PrintWriter writer = new PrintWriter("Test.txt");
    writer.println(sM.findStudent(2));
    writer.close();
    sM.removeStudent(1);
    sM.removeStudent(0);
  }
}
