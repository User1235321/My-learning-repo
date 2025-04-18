package tenth;

import java.lang.InterruptedException;

public class Customer implements Runnable
{
  private Materials mat;
  private WorkProcess wp;
  private CountDownLatch latch;
  public Customer(Materials mat, WorkProcess wp, CountDownLatch latch)
  {
    this.mat = mat;
    this.wp = wp;
    this.latch = latch;
  }
  @Override
  public void run()
  {
    while (!mat.status())
    {
      mat.newMaterial();
      System.out.println("Add new material");
      try
      {
        Thread.sleep(10);
      }
      catch(InterruptedException e)
      {
        System.err.println(e);
      }
    }
    System.out.println("All materials are ready");
    while (!wp.status());
    System.out.println("Customer is happy");
  }
}
