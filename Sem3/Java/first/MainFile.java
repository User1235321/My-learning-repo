package first;

import java.util.Scanner;

import java.util.TreeMap;
import java.util.InputMismatchException;

public class MainFile
{
  public static void main(String[] args)
  {
    TreeMap< String, MoveStrategy > transportMap = new TreeMap<>();
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
      MoveStrategy newTransport = transportMap.get(str);
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
