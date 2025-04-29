library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
package types is
  type integer_array is array (natural range <>) of integer;
end package types;
package body types is
end package body types;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use work.types.all;

entity insertSort is
  generic
  (
    elementsNum : integer := 8
  );
  port
  (
    clk : in std_logic;
    start : in std_logic;
    reset : in std_logic;
    inArr : in integer_array(0 to (elementsNum - 1));

    outArr : out integer_array(0 to (elementsNum - 1));
    done : out std_logic;
    working : out std_logic
  );
end insertSort;

architecture behav of insertSort is
  signal blocking : std_logic;
begin
  process(clk)
    variable localArr : integer_array(0 to (elementsNum - 1)) := (others => 0);
    variable temp : integer := 0;
    variable i : integer range 0 to (elementsNum - 1) := 0;
    variable j : integer range -1 to (elementsNum - 1) := 0;
  begin
    if (rising_edge(clk)) then
      if (reset = '1') then
        outArr <= (others => 0);
        done <= '0';
        blocking <= '1';
      elsif(start = '1') then
        outArr <= (others => 0);
        done <= '0';
        blocking <= '0';
        localArr := inArr;
        i := 0;
        j := 0;
      elsif (blocking = '0') then
        if (i < (elementsNum - 1)) then
          if ((j >= 0) and (localArr(j) > localArr(j + 1))) then
            temp := localArr(j);
            localArr(j) := localArr(j + 1);
            localArr(j + 1) := temp;
            j := j - 1;
          else
            j := i + 1;
            i := i + 1;
          end if;
        else
          done <= '1';
          outArr <= localArr;
          blocking <= '0';
        end if;
      end if;
      working <= blocking;
    end if;
  end process;
end behav;
