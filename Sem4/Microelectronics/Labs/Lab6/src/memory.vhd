USE work.dp32_types.ALL, work.alu_32_types.ALL;
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
ENTITY memory IS
	GENERIC (Tpd : TIME := unit_delay);
	PORT (
		d_bus : INOUT bus_bit_32 BUS;
		a_bus : IN bit_32;
		read : IN STD_LOGIC;
		write : IN STD_LOGIC;
		ready : OUT STD_LOGIC);
END memory;
ARCHITECTURE behaviour OF memory IS
BEGIN
	PROCESS
		CONSTANT low_address : INTEGER := 0;
		CONSTANT high_address : INTEGER := 65535;
		TYPE memory_array IS
		ARRAY(INTEGER RANGE low_address TO high_address)OF bit_32;
		VARIABLE address : INTEGER := 0;
		VARIABLE mem : memory_array :=
		(X"0700_0000",
		X"1002_0000",
		X"2102_0000",
		X"0000_0008",
		X"1002_0201",
		X"1101_020A",
		X"5009_00FA",
		X"5000_00FA",
		X"0000_0000",

		OTHERS => X"0000_0000");
	BEGIN
		--
		-- put d_bus and reply into initial state
		--
		d_bus <= NULL AFTER Tpd;
		ready <= '0' AFTER Tpd;
		--
		-- wait for a command
		--
		WAIT UNTIL (read = '1') OR (write = '1');
		--
		-- dispatch read or write cycle
		--
		address := bits_to_int(a_bus);
		IF address >= low_address AND address <= high_address THEN
			-- address match for this memory
			IF write = '1' THEN
				ready <= '1' AFTER Tpd;
				WAIT UNTIL write = '0'; -- wait until end of write cycle
				mem(address) := d_bus;--'delayed(Tpd); -- sample data from Tpd ago
			ELSE -- read='1'
				d_bus <= mem(address) AFTER Tpd; -- fetch data
				ready <= '1' AFTER Tpd;
				WAIT UNTIL read = '0'; -- hold for read cycle
			END IF;
		END IF;
	END PROCESS;
END behaviour;
