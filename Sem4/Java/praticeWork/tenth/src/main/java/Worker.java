package tenth;

import java.lang.InterruptedException;

public class Worker implements Runnable
{
  private Materials mat;
  private WorkProcess wp;
  private CountDownLatch latch;
  public Worker(Materials mat, WorkProcess wp, CountDownLatch latch)
  {
    this.mat = mat;
    this.wp = wp;
    this.latch = latch;
  }
  public void run()
  {
    while (!mat.status());
    System.out.println("Workers start building");
    while (!wp.status())
    {
      System.out.println("Another day of hard work");
      try
      {
        Thread.sleep(10);
      }
      catch (InterruptedException e)
      {
        System.err.println(e);
      }
      wp.work();
    }
    System.out.println("Worker is happy");
  }
}
