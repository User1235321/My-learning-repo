package eleventh;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.ExecutionException;

public class App
{
  public static class SimpleTask implements Runnable
  {
    @Override
    public void run()
    {
      System.out.print("/\\");
      try
      {
        TimeUnit.MILLISECONDS.sleep(100);
      }
      catch(InterruptedException e)
      {
        Thread.currentThread().interrupt();
      }
    }
  }
  public static void runTaskViaExecutor()
  {
    try(ExecutorService executor = Executors.newCachedThreadPool())
    {
      for (int i = 0; i < 100000; ++i)
      {
        executor.submit(new SimpleTask());
      }
    }
  }
  public static void runTaskViaVirtualThread()
  {
    try(ExecutorService executor = Executors.newVirtualThreadPerTaskExecutor())
    {
      for (int i = 0; i < 100000; ++i)
      {
        executor.submit(new SimpleTask());
      }
    }
  }
  public static void calculatePi(ExecutorService executor)
  {
    int nTasks = Runtime.getRuntime().availableProcessors();
    long totalPoints = 10000000;
    long pointsPerTask = totalPoints / nTasks;
    long insideCircle = 0;
    try(executor)
    {
      Future< Long >[] futures = new Future[nTasks];
      for (int i = 0; i < nTasks; ++i)
      {
        futures[i] = executor.submit(() ->
          {
            long count = 0;
            for (int j = 0; j < pointsPerTask; ++j)
            {
              double x = Math.random();
              double y = Math.random();
              if ((x * x + y * y) <= 1)
              {
                ++count;
              }
            }
            return count;
          }
        );
      }
      for (Future< Long > f : futures)
      {
        try
        {
          insideCircle += f.get();
        }
        catch(InterruptedException e)
        {
          System.out.println("Err");
        }
        catch(ExecutionException e)
        {
          System.out.println("Err");
        }
      }
    }
    double pi = 4.0 * insideCircle / totalPoints;
    System.out.println("Pi = " + pi);
  }
  public static void calculatePiVirtual()
  {
      calculatePi(Executors.newVirtualThreadPerTaskExecutor());
  }
  public static void calculatePiExecutor()
  {
    calculatePi(Executors.newCachedThreadPool());
  }
  public static void main(String[] args)
  {
    runTaskViaExecutor();
    runTaskViaVirtualThread();
  }
}
