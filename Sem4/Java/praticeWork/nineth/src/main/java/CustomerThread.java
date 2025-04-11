package nineth;

import java.util.Random;

class CustomerThread implements Runnable
{
  public void run()
  {
    int num = (Random() % 9 + 1);
    String status[num];
    for (int i = 0; i < num; ++i)
    {
      status[i] = "new order";
    }
    while (1)
    {
      for (int i = 0; i < num; ++i)
      {
        if (status[i].equals("new order"))
        {
          custWait.add(new Order("Water", i));
          System.out.println(id + " customer make order");
          status[i] = "wait";
        }
        else
        {
          boolean findOrder = false;
          for (int j = 0; j < waitCust.size(); ++j)
          {
            if (waitCust[j].getStatus().equals("attribute") && (waitCust[j].getId() == i))
            {
              waitCust[j].giveAwayOrder();
              System.out.println(id + " customer get his dish");
              status[i] = "new order";
              findOrder = true
              j = waitCust.size();
            }
          }
          if (!findOrder)
          {
            System.out.println(id + " waiting his order");
          }
        }
      }
      Thread.sleep(Random() % 600);
    }
  }
}
