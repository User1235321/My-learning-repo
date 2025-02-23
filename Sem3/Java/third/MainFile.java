package third;

import java.util.*;
import java.util.stream.Collectors;

public class MainFile
{
  public static double average(List< Integer > numbers)
  {
    return numbers.stream()
      .mapToInt(Integer::intValue)
      .average()
      .orElse(0);
  }

  public static List< String > prefixStrings(List< String > strings)
  {
    return strings.stream()
      .map(String::toUpperCase)
      .map(s -> "_new_" + s)
      .collect(Collectors.toList());
  }

  public static List< Integer > squaresOfUnique(List< Integer > numbers)
  {
    return numbers.stream()
      .filter(n -> Collections.frequency(numbers, n) == 1)
      .map(n -> n * n)
      .collect(Collectors.toList());
  }

  public static < T > T getLast(Collection< T > collection)
  {
    return collection.stream()
    .reduce((first, second) -> second)
    .orElse(null);
  }

  public static int sumOfEven(List < Integer > numbers)
  {
    return Arrays.stream(numbers.stream()
        .mapToInt(i -> i)
        .toArray())
      .filter(n -> ((n % 2) == 0))
      .sum();
  }

  public static Map< Character, String > stringToMap(List< String > strings)
  {
    return strings.stream()
      .collect(Collectors.toMap(
      s -> s.charAt(0),
      s -> s.substring(1)
    ));
  }

  public static void main(String[] args)
  {
    List< Integer > numbers = Arrays.asList(0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55);
    System.out.println("Среднее значение: " + average(numbers));

    List< String > strings = Arrays.asList("first", "second", "third", "fourth");
    System.out.println("Префикс строк: " + prefixStrings(strings));

    numbers = Arrays.asList(100, 100, 100, 100, 12, 12, 12, 7, 7, 2, 0);
    System.out.println("Квадраты уникальных чисел: " + squaresOfUnique(numbers));

    numbers = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9);
    System.out.println("Последний элемент (List): " + getLast(numbers));

    numbers = Arrays.asList(1001, 1001, 1001, 1000, 100, 10, 1);
    System.out.println("Сумма четных чисел: " + sumOfEven(numbers));

    strings = Arrays.asList("One", "Two", "LikeAThree", "Fourh");
    System.out.println("Строки в Map: " + stringToMap(strings));
  }
}
