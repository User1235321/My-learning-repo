package fifth;

public class Student extends Object
{
  private int id_;
  private String name_;
  private int age_;
  private int course_;
  private int grade_;

  public Student()
  {
    id_ = 0;
    name_ = "Noname";
    age_ = 0;
    course_ = 0;
    grade_ = 0;
  }
  public Student(int id, String name, int age, int course, int grade)
  {
    id_ = id;
    name_ = name;
    age_ = age;
    course_ = course;
    grade_ = grade;
  }
  public int getId()
  {
    return id_;
  }
  @Override
  public String toString()
  {
    return new String("ID: " + id_ + "; Name: " + name_ + "; Age: " + age_ + "; Course: " + course_ + "; Grade: " + grade_);
  }
}
