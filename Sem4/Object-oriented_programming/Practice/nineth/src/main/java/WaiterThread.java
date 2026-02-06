package nineth;

import java.util.Random;

class WaiterThread implements Runnable
{
  public void run()
  {
    int num = (Random() % 9 + 1);
    String status[num];
    for (int i = 0; i < num; ++i)
    {
      status[i] = "watching anime";
    }
    while (1)
    {
      for (int i = 0; i < num; ++i)
      {
        if (cookWait.size() > 0)
        {
          status[i] = "attribute to kitchen";
          Order dish = cookWait.peek();
          System.out.println(i + " waiter attribute dish to customer " + cookWait.peek().getId());
           
        }
        else if (custWait.size() > 0)
        {
          status[i] = "attribute to waiter";
          System.out.println(i + " waiter " );
        }
        else
        {
          System.out.println(i + " waiter now just wathing anime, because there isn't work");
        }
      }
      Thread.sleep(Random() % 200);
    }
  }
}
