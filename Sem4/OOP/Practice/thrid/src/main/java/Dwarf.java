package thrid;

public class Dwarf
{
	private String name;
	private FacialHair facialHair;
	private List<Weapon> weapons = new LinkedList<>();
	private String lunch;
	private int dwarfAge;

	public Dwarf()
	{
	}

	public Dwarf(String name, int dwarfAge)
	{
		this.name = name;
		this.dwarfAge = dwarfAge;
	}

	// getters & setters
}
