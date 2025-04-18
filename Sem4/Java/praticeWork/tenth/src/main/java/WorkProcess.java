package tenth;

import java.util.Random;

public class WorkProcess
{
  private int numOfWork;
  private int needWork;
  public WorkProcess()
  {
    needWork = new Random().nextInt(100);
    numOfWork = 0;
  }
  public synchronized void work()
  {
    if (numOfWork < needWork)
    {
      ++numOfWork;
    }
  }
  public synchronized boolean status()
  {
    return (numOfWork >= needWork);
  }
}
