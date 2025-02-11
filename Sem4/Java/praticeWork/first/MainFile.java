package first;

import java.lang.Thread;

public class MainFile
{
  public static void main(String[] args) throws InterruptedException
  {
    Thread newThread = new Thread(new myCoolFunc());
    System.out.println("Start status: " + newThread.getState());

    newThread.start();
    System.out.println("Just start: " + newThread.getState());

    Thread waitThread = new Thread(new Runnable() {
      public void run() {
        try {
        newThread.join();
        } catch (Exception e) {
        }
      }
    });

    waitThread.start();
    System.out.println("Join status: " + newThread.getState());

    System.out.println("End status: " + newThread.getState());
  }
}
