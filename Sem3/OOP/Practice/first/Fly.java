package first;

class Fly implements MoveStrategy
{
  @Override
  public double move(Point start, Point end)
  {
    double distance = Math.sqrt(Math.pow(start.X() - end.X(), 2) + Math.pow(start.Y() - end.Y(), 2));
    return (distance / 10);
  }
  public String getName()
  {
    return new String("fly");
  }
}
