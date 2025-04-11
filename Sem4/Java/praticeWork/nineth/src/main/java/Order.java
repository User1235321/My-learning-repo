package nineth;

class Order
{
  private String dishName;
  private dishStatus;
  private customerId;

  public Order(String name, int id)
  {
    customerId = id;
    dishName = name;
    dishStatus = "start";

  }
  public void startOrder()
  {
    dishStatus = "decorated";
  }
  public void startCooking()
  {
    dishStatus = "cooking";
  }
  public void attributeToHall()
  {
    dishStatus = "attribute";
  }
  public void giveAwayOrder()
  {
    customerId = 0;
    dishStatus = "complete";
  }
  public String getStatus()
  {
    return dishStatus;
  }
  public int getCustomerId()
  {
    return customerId;
  }
}
