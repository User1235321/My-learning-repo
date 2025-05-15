package eight;

import java.io.PrintWriter;
import java.io.FileNotFoundException;

public class BankAccount
{
  private int age;
  private String name;
  private long sum;

  public void validateUserAge(int age) throws BankException
  {
    if ((age >= 18) && (age <= 130))
    {
      this.age = age;
    }
    else
    {
      throw (new BankException("Incorrect user age: " + age));
    }
  }
  public void deposite(long amount) throws BankException
  {
    if (amount > 0)
    {
      sum += amount;
    }
    else if (amount < 0)
    {
      throw (new BankException("Can't deposite zero!"));
    }
    else
    {
      throw (new BankException("Can't deposite negative value!"));
    }
  }
  public void withdraw(int amount) throws BankException, InsuffitionFundsException
  {
    if (amount > sum)
    {
      throw (new InsuffitionFundsException("Can't draw this amount!"));
    }
    else if (amount == 0)
    {
      throw (new BankException("Can't draw zero!"));
    }
    else
    {
      sum -= amount;
    }
  }
  public void printAccountInfo()
  {
    System.out.println("Account name: " + name + ";\t User age: " + age + ";\t Sum at the moment: " + sum);
  }
  public void printAccountInfoToFile() throws FileNotFoundException
  {
    PrintWriter writer = new PrintWriter("SecretAccountInfo");
    writer.println("Account name: " + name + ";\t User age: " + age + ";\t Sum at the moment: " + sum);
    writer.close();
  }
}
