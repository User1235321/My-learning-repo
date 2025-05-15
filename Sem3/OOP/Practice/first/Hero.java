package first;

public class Hero
{
  public Hero()
  {
    this.name = "Noname";
    this.transport = new Walk();
  }
  public Hero(String str)
  {
    this.name = str;
    this.transport = new Walk();
  }
  public void printInfo()
  {
    System.out.println("Hero name: " + this.name + "; transport: " + this.transport.getName());
  }
  public void move(Point start, Point end)
  {
    System.out.println(this.name + " goes from start to end points in time: " + this.transport.move(start, end));
  }
  public void changeTransport(MoveStrategy newTransport)
  {
    this.transport = newTransport;
  }

  private String name;
  private MoveStrategy transport;
}
