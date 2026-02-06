package first;

import java.util.Scanner;

public class Point
{
  public void input(Scanner scan)
  {
    this.x = scan.nextDouble();
    this.y = scan.nextDouble();
  }
  public double X()
  {
    return this.x;
  }
  public double Y()
  {
    return this.y;
  }
  private double x, y;
}
