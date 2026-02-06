package tenth;

import java.lang.Thread;
import java.util.concurrent.*;
import java.util.Random;
import java.util.concurrent.atomic.*;

public class App
{

  static ConcurrentHashMap< String, AtomicInteger > pageMap = new ConcurrentHashMap<>();
  private static void visitPage(String pageName)
  {
    if (pageMap.containsKey(pageName))
    {
      pageMap.get(pageName).incrementAndGet();
    }
    else
    {
      System.out.println("Can't find page: " + pageName);
    }
  }
  public static void main(String[] args)
  {
    var executor = Executors.newFixedThreadPool(10);
    String [] pageNames = new String[5];
    for (int i = 0; i < pageNames.length; ++i)
    {
      Random random = new Random();
      pageNames[i] = "page" + random.nextInt(pageNames.length * 100);
      pageMap.put(pageNames[i], new AtomicInteger());
    }

    try (executor)
    {
      for (int i = 0; i < 1000; ++i)
      {
        executor.execute(() -> {
          Random random = new Random();
            visitPage(pageNames[random.nextInt(pageNames.length)]);
          }
        );
      }
    }

    for (int i = 0; i < pageNames.length; ++i)
    {
      System.out.println("Page \"" + pageNames[i] + "\" visited " + pageMap.get(pageNames[i]) + " times");
    }
  }
}
