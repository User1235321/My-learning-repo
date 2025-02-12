LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
ENTITY rstr1 IS
PORT(r:IN std_logic;
	s:IN std_logic;
	q:OUT std_logic);
END rstr1;
ARCHITECTURE behav OF rstr1 IS
	SIGNAL qs:std_logic;
BEGIN
PROCESS(r,s)
	BEGIN
		IF s = '0' THEN
			IF r = '0' THEN
				qs <= qs;
			ELSE
				qs <= '0';
			END IF;
		ELSE
			IF r = '0' THEN
				qs <= '1';
			END IF;
		END IF;
	END PROCESS;
q<=qs;
END behav;
