library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity second is
  port
  (
    DATA : in std_logic_vector (7 downto 0);
    KKS : in std_logic_vector (7 downto 0);
    CLK : in std_logic;
    Query : in std_logic;

    Q : out std_logic_vector (7 downto 0);
    Get : out std_logic;
    Synchro : out std_logic;
    qq : out integer range 0 to 7
  );
end second;

architecture behav of second is
type array_type is array (7 downto 0) of std_logic_vector(7 downto 0);
signal dffArr : array_type;
--signal num : integer range 0 to 8 := 0;
signal over : std_logic;


begin
  process(CLK)
  variable num: integer;
  begin
    if ((CLK'event) and (CLK = '1')) then
      --num <= num + 1;
      num:= num + 1;
      if (num = 8) then
        over <= '0';
        --num <= 1;
        num := 1;
      end if;

      --compare
      if (KKS = dffArr(7)) then
        --num <= 1;
        num := 1;
        Synchro <= '1';
      else
        Synchro <= '0';
      end if;

      --dff block
      for i in 7 downto 1 loop
        dffArr(i) <= dffArr(i - 1);
      end loop;

      if (((Query = '1') and (dffArr(7) = KKS)) or (over = '1')) then
        if (over = '0') then
          over <= '1';
        end if;
        dffArr(0) <= DATA;
      else
        dffArr(0) <= dffArr(7);
      end if;
    end if;
  end process;
  Get <= over;
  Q <= dffArr(7);
  --qq <= num;
end behav;
