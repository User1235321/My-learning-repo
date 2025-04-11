package inineth;

import java.util.Scanner;
import java.lang.Thread;

public class App
{
  public static void main(String[] args)
  {
    Scanner in = new Scanner(System.in)
    String input = new String;
    Queue< Order > custWait = new Queue< Order >, waitCook = new Queue< Order >, cookWaitCust = new Queue < Order >;
    public static Thread custTheard;
    public static Thread waitTheard;
    public static Thread cookTheard;

    while (!input.equals("End"))
    {
      input = in.nextLine();
      if (input.equals("start"))
      {
        custTheard = new Thread(CustomerThread);
        waitTheard = new Thread(WaiterThread);
        cookTheard = new Thread(CookThread);
      }
      else if (input.equals("stop"))
      {
        custWait.clear();
        waitCook.clear();
        cookWaitCust.clear();

        custTheard.stop();
        waitTheard.stop();
        cookTheard.stop();
        //Останавливаем все потоки, обнуляем очереди
      }
    }
  }
}
