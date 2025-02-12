library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity goodCounter is
  port
	(
    CLOCK : in std_logic;
    RESET : in std_logic;

    OVER : out std_logic;
	num : out integer range 0 to 7
  );
end goodCounter;

architecture behav of goodCounter is
signal qq : integer range 0 to 7 := 0;

begin
  process(CLOCK, RESET)
  begin
	if (RESET = '1') then
		OVER <= '0';
		qq <= 0;
    elsif rising_edge(CLOCK) then
		if (qq < 7) then
    		qq <= qq + 1;
		else
			qq <= 0;
		end if;
    end if;
	if (qq < 7) then
		OVER <= '0';
	else
		OVER <= '1';
	end if;
	num <= qq;
  end process;
end behav;
