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
    num : out integer range 0 to 7;
	--
	rstrOUT : out std_logic_vector (1 downto 0);
	getOut : out std_logic
  );
end second;

architecture behav of second is
function rstr (r : std_logic; s : std_logic; q : std_logic) return std_logic is
begin
  if s = '0'  then
    if r = '0' then
      return q;
		else
      return '0';
		end if;
	else
      return '1';
	end if;
end rstr;

function comp (a : std_logic_vector(7 downto 0); b : std_logic_vector(7 downto 0)) return std_logic is
begin
  for i in 7 downto 0 loop
    if (a(i) /= b(i)) then
      return '0';
    end if;
  end loop;
  return '1';
end comp;

type array_type is array (7 downto 0) of std_logic_vector(7 downto 0);
signal seventhDff : std_logic_vector (7 downto 0) := "00000000";
signal dffArr: array_type;
signal get1 : std_logic := '0';
signal rstrQ : std_logic_vector (1 downto 0) := "00";
signal qq : integer range 0 to 7 := 0;
signal aeb : std_logic := '0';

begin
  process(DATA, KKS, CLK, Query, dffArr, get1, rstrQ, seventhDff, qq)
  begin
    --frame offset
    if rising_edge(CLK) then
      seventhDff <= dffArr(7);
      for i in 7 downto 1 loop
        dffArr(i) <= dffArr(i - 1);
      end loop;

      --Mux
      if (get1 = '1') then
        dffArr(0) <= DATA;
      else
        dffArr(0) <= seventhDff;
      end if;
    end if;

    --compare
    aeb <= comp(seventhDff, KKS);
    --Counter
    if (aeb = '1') then
      qq <= 0;
    elsif rising_edge(CLK) then
      if (qq < 8) then
        qq <= qq + 1;
      else
        qq <= 0;
      end if;
    end if;
    if (qq = 7) then
      get1 <= '1';
    else
      get1 <= '0';
    end if;
	getOut <= get1;
	aeb <= comp(seventhDff, KKS);
    --rstr block
    rstrQ(0) <= rstr(get1, Query, rstrQ(0));
    rstrQ(1) <= rstr(get1, (rstrQ(0) and aeb), rstrQ(1));
    get1 <= rstrQ(1);

    --Output
    Get <= get1;
    Synchro <= aeb;
    Q <= dffArr(7);
    num <= qq;
	--
	rstrOUT <= rstrQ;
  end process;
end behav;
