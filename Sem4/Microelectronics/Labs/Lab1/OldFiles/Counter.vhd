library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity Counter is
    Port (
        clk   : in  std_logic;      -- Тактовый сигнал
        reset : in  std_logic;      -- Сигнал сброса
        over  : out std_logic       -- Выходной сигнал, равный 1, если прошло 8 временных циклов
    );
end Counter;

architecture Behavioral of Counter is
    signal count : integer range 0 to 7 := 0;  -- Счётчик от 0 до 7
begin

    process(clk, reset)
    begin
        -- Сбрасываем счётчик
        if reset = '1' then
            count <= 0;  -- Устанавливаем счётчик в 0
            over <= '0';  -- Обнуляем сигнал over
        elsif rising_edge(clk) then
            -- Увеличиваем счётчик при каждом тактовом сигнале
            if count < 7 then
                count <= count + 1;  -- Увеличиваем счётчик
                over <= '0';  -- Если счётчик меньше 8, то over = 0
            else
                count <= 0;  -- Обнуляем счётчик, если он достиг 8
                over <= '1';  -- Если счётчик достиг 8, то over = 1
            end if;
        end if;
    end process;

end Behavioral;
