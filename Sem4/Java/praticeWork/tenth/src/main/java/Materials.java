package tenth;

import java.util.Random;

public class Materials
{
  private int materialsNow;
  private int needMaterials;
  public Materials()
  {
    needMaterials = new Random().nextInt(100);
    materialsNow = 0;
  }
  public synchronized void newMaterial()
  {
    if (materialsNow < needMaterials)
    {
      ++materialsNow;
    }
  }
  public synchronized boolean status()
  {
    return (materialsNow >= needMaterials);
  }
}
