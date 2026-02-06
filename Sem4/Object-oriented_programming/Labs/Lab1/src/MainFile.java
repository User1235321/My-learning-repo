package Lab1;

import java.time.Duration;
import Lab1.AbstractProgramm.Condition;

public class MainFile
{
  public static void main(String[] args)
  {
    AbstractProgramm program = new AbstractProgramm(Duration.ofSeconds(1));
    Thread superVisor = new Thread(() -> {
      program.start();
      synchronized (program)
      {
        try
        {
          while (program.getIsAlive())
          {
            program.wait();
            Condition cond = program.getCond();
            System.out.print("Condision = ");
            System.out.println(cond);
            switch (cond)
            {
              case STOPPING:
                program.restart();
                System.out.println("Program was Restarted!");
                break;
              case FATAL_ERROR:
                program.stop();
                System.out.println("Error in programm!");
                break;
              default:
                break;
            }
            program.notifyAll();
          }
        }
        catch (InterruptedException e)
        {
          Thread.currentThread().interrupt();
          System.err.println(e);
        }
      }
    });
    try
    {
      superVisor.start();
      superVisor.join();
    } catch (InterruptedException e) {
      Thread.currentThread().interrupt();
      System.err.println(e);
    }
  }
}
