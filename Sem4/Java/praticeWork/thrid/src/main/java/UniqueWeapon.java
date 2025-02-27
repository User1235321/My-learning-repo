package thrid;

public class UniqueWeapon extends Weapon
{
	private String name;
	private String origin;

	public UniqueWeapon()
	{
		super();
	}

	public UniqueWeapon(String type, String name, String origin)
	{
		super(type);
		this.name = name;
		this.origin = origin;
	}

	// getters & setters
}
