package first;

public class Horse implements MoveStrategy
{
  @Override
  public double move(Point start, Point end)
  {
    double distance = Math.abs(start.X() - end.X()) + Math.abs(start.Y() - end.Y());
    return (distance / 5);
  }
  public String getName()
  {
    return new String("horse");
  }
}
