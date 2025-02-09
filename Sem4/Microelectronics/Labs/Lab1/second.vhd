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
    Synchro : out std_logic

    --delete
    --seventhDffCopy : out std_logic_vector (7 downto 0);
    --get1Copy : out std_logic;
    --rstrQCopy : out std_logic_vector (1 downto 0);
    --qqCopy : out unsigned(7 downto 0)
  );
end second;

architecture behav of second is
type array_type is array (7 downto 0) of std_logic_vector(7 downto 0);
function rstr (r : std_logic; s : std_logic; q : std_logic) return std_logic is
begin
  if s = '1'  then
    if r = '1' then
        return q;
			else
        return '0';
			end if;
		else
      return '1';
		end if;
    return q;
end function rstr;

function comp (a : std_logic_vector(7 downto 0); b : std_logic_vector(7 downto 0)) return std_logic is
begin
  for i in 7 downto 1 loop
    if (a(i) /= b(i)) then
      return '0';
    end if;
  end loop;
  return '1';
end function comp;

signal seventhDff : std_logic_vector (7 downto 0) := "00000000";
signal dffArr: array_type;
signal get1 : std_logic := '0';
signal rstrQ : std_logic_vector (1 downto 0) := "00";
signal qq : unsigned(7 downto 0) := "00000000";

begin
  process(DATA, KKS, CLK, Query, dffArr, get1, rstrQ, seventhDff, qq)
  begin
    --frame offset
    if rising_edge(CLK) then
      seventhDff <= dffArr(7);
      for i in 7 downto 1 loop
        dffArr(i) <= dffArr(i - 1);
      end loop;
    end if;
    if (get1 = '0') then
      dffArr(0) <= DATA;
    else
      dffArr(0) <= seventhDff;
    end if;
    rstrQ(0) <= rstr(qq(0), Query, rstrQ(0));
    rstrQ(1) <= rstr(qq(0), (rstrQ(0) and comp(seventhDff, KKS)), rstrQ(1));
    get1 <= rstrQ(1);

    --Output
    if rising_edge(CLK) then
      qq <= qq + 1;
    end if;
    if (comp(seventhDff, KKS) = '1') then
      qq <= "00000000";
    end if;
    Get <= get1;
    Synchro <= comp(seventhDff, KKS);
    Q <= dffArr(7);

    --delete
    --seventhDffCopy <= seventhDff;
    --get1Copy <= get1;
    --rstrQCopy <= rstrQ;
    --qqCopy <= qq;
  end process;
end behav;
