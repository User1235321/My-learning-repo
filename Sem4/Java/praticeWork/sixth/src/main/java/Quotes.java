package sixth;

public class Quotes
{
  private String author;
  private String quote;

  public Quotes(String author, String quote)
  {
    this.author = author;
    this.quote = quote;
  }
  public String getQuote()
  {
    return this.quote;
  }
  public String getAuthor()
  {
    return this.author;
  }
}
