library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lifoCode is
  port
  (
    CLK : in std_logic;
    DI : in std_logic_vector(0 to 7);
    RD : in std_logic;
    WR : in std_logic;
    Reset : in std_logic;

    Full : out std_logic;
    Empty : out std_logic;
    DO : out std_logic_vector(0 to 7)
  );
end lifoCode;

architecture behav of lifoCode is
  type array_type is array (0 to 7) of std_logic_vector(0 to 7);
  signal dffArr : array_type := (others => (others => '0'));
  signal count : integer range 0 to 8 := 0;
begin
  process(CLK, Reset)
  begin
    if (Reset = '1') then
      count <= 0;
      dffArr <= (others => (others => '0'));
      Full <= '0';
      Empty <= '1';
      DO <= (others => '0');
    elsif (rising_edge(CLK)) then
      if ((RD = '1') and (count > 0)) then
        DO <= dffArr(count - 1);
        count <= count - 1;
      elsif ((WR = '1') and (count < 8)) then
        dffArr(count) <= DI;
        count <= count + 1;
      end if;
      if (count = 8) then
        Full <= '1';
        Empty <= '0';
      elsif (count = 0) then
        Full <= '0';
        Empty <= '1';
      else
        Full <= '0';
        Empty <= '0';
      end if;
    end if;
  end process;
end behav;
