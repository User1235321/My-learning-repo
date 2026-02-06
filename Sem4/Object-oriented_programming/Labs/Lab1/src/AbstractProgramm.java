package Lab1;

import java.util.Arrays;
import java.util.Random;
import java.lang.Thread;
import java.time.Duration;

public class AbstractProgramm
{
  public static enum Condition
  {
    UNKNOWN,
    STOPPING,
    RUNNING,
    FATAL_ERROR
  }
  private Condition currentCondition;
  private Thread daemonThread;
  private boolean isAlive;
  private static final Condition[] conditions = Arrays.stream(Condition.values())
      .filter(condition -> (condition != Condition.UNKNOWN))
      .toArray(Condition[]::new);
  private static final Random rand = new Random();

  public AbstractProgramm(Duration dur)
  {
    currentCondition = Condition.UNKNOWN;
    isAlive = false;
    daemonThread = new Thread(() -> changeState(dur));
    daemonThread.setDaemon(true);
  }

  private synchronized void changeState(Duration dur)
  {
    try
    {
      for(;;)
      {
        notifyAll();
        wait();
        Thread.sleep(dur);
        currentCondition = conditions[rand.nextInt(conditions.length)];
        if (currentCondition == Condition.STOPPING)
        {
          System.out.println("Programm is stopping!");
        }
      }
    }
    catch (InterruptedException e)
    {
      Thread.currentThread().interrupt();
    }
  }

  public void start()
  {
    daemonThread.start();
    isAlive = true;
  }

  public void restart()
  {
    currentCondition = Condition.RUNNING;
  }

  public void stop()
  {
    daemonThread.interrupt();
    isAlive = false;
  }

  public boolean getIsAlive()
  {
    return isAlive;
  }

  public Condition getCond()
  {
    return currentCondition;
  }
}
