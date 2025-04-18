package tenth;

import java.lang.Thread;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class App
{
  public static void main(String[] args)
  {
    String fileName = "nFile";
    int num = 0;
    File file = new File(fileName);
    try
    {
      Scanner scanner = new Scanner(file);
      num = scanner.nextInt();
      scanner.close();
    }
    catch (FileNotFoundException e)
    {
      System.out.println("App.java:main:string 22 can't read number from file: " + fileName);
    }

    Materials material = new Materials();
    WorkProcess workProcess = new WorkProcess();

    CountDownLatch latch = new CountDownLatch(1);
    Thread customer = new Thread(new Customer(material, workProcess, latch));
    Thread workers[] = new Thread[num];
    for (int i = 0; i < num; ++i)
    {
      workers[i] = new Thread(new Worker(material, workProcess, latch));
      workers[i].start();
    }
    customer.start();
  }
}
