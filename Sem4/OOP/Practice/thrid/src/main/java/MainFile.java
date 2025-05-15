package thrid;

public class MainFile
{
  public static void main(String[] args)
  {
    System.out.println("Start programm");

    DwarvesBand band = BandUtil.createBand();
    Gson gson = new GsonBuilder()
			.setPrettyPrinting()
			.create();
    String json = gson.toJson(band);

    System.out.println(json);
    System.out.println("End programm");
  }
}

