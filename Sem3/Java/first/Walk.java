package first;

public class Walk implements MoveStrategy
{
  @Override
  public double move(Point start, Point end)
  {
    double distance = Math.sqrt(Math.pow(start.X() - end.X(), 2) + Math.pow(start.Y() - end.Y(), 2));
    return distance;
  }
  public String getName()
  {
    return new String("walk");
  }
}
