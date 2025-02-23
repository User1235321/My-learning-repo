package second;

import java.lang.Thread;

public class MainFile
{
  public static Object obj = new Object();
  public static Thread thread1 = new Thread(MainFile::printNumbers);
  public static Thread thread2 = new Thread(MainFile::printLetters);

  public static void main(String[] args)
  {
    thread1.start();
    thread2.start();
  }

  private static void printNumbers()
  {
    for (int i = 0; i < 10; ++i)
    {
//      try
      {
        synchronized(obj)
        {
          System.out.println(i);
          if (thread2.getState().toString() == "BLOCKED")
          {
            System.out.println("Second thread is blocked");
            obj.notify();
          }
        }
      }
//      catch (InterruptedException e)
      {
        System.out.println("Programm is dead");
      }
    }
  }
  private static void printLetters()
  {
    for (char a = 'A'; a < 'F'; ++a)
    {
      try
      {
        synchronized(obj)
        {
          obj.wait();
          System.out.println(a);
        }
      }
      catch (InterruptedException e)
      {
        System.out.println("Programm is dead");
      }
    }
  }
}
