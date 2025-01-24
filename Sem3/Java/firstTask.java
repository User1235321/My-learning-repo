import java.util.Scanner;
import java.lang.Math;
import java.util.TreeMap;
import java.util.InputMismatchException;

class Point
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

interface moveStrategy
{
  public double move(Point start, Point end);
  public String getName();
}
class Walk implements moveStrategy
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
class Horse implements moveStrategy
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
class Fly implements moveStrategy
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

class Hero
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
  public void changeTransport(moveStrategy newTransport)
  {
    this.transport = newTransport;
  }

  private String name;
  private moveStrategy transport;
}

public class firstTask
{
  public static void main(String[] args)
  {
    TreeMap< String, moveStrategy > transportMap = new TreeMap<>();
    transportMap.put("walk", new Walk());
    transportMap.put("horse", new Horse());
    transportMap.put("fly", new Fly());

    Scanner scan = new Scanner(System.in);
    System.out.println("Add hero name:");
    Hero ourHero = new Hero(scan.nextLine());

    System.out.println("Start and end points. format: x y");
    Point start = new Point();
    Point end = new Point();
    try
    {
      start.input(scan);
      end.input(scan);
    }
    catch (InputMismatchException err)
    {
      System.out.println("Bad point input. Programm ended");
      return;
    }
    System.out.println("All points are input, you can move hero by commands: walk, horse, fly. To end programm print exit");

    String str = scan.nextLine();
    str = scan.nextLine();
    while (!str.contains("exit"))
    {
      moveStrategy newTransport = transportMap.get(str);
      if (newTransport == null)
      {
        System.out.println("Unknown type of transport. There next types of transport: walk, horse, fly");
      }
      else
      {
        ourHero.changeTransport(newTransport);
        ourHero.move(start, end);
      }
      str = scan.nextLine();
    }
    System.out.println("Programm is ended");
  }
}
