package first;

public class myCoolFunc implements Runnable
{
  public myCoolFunc()
  {
    number = 0;
  }
  @Override
  public void run()
  {
    try {
      Thread.sleep(1000 * 10);
    } catch (Exception e) {
    }
  }
  static int number;
}
