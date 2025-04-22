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
		(
        X"0700_0000", -- r0 = 0;

        -- Start array value
        X"1001_0003", -- r1 = 3;
        X"1002_0006", -- r2 = 6;
        X"1003_0007", -- r3 = 7;
        X"1004_0005", -- r4 = 5;
        X"1005_0003", -- r5 = 3;
        X"1006_0005", -- r6 = 5;
        X"1007_0006", -- r7 = 6;
        X"1008_0002", -- r8 = 2;
        --Put start value's to memory
        X"3101_0000", -- M[0] = r1
        X"3102_0001", -- M[1] = r2
        X"3103_0002", -- M[2] = r3
        X"3104_0003", -- M[3] = r4
        X"3105_0004", -- M[4] = r5
        X"3106_0005", -- M[5] = r6
        X"3107_0006", -- M[6] = r7
        X"3108_0007", -- M[7] = r8
        -- i and j
        X"1009_0001", -- r9(i) = 1;
        X"000A_0900", -- r10(j) = r9(i);
        -- other variable
        X"100B_0007", -- r11(number of elements) = 7;
        X"100C_0000", -- r12(valToSwap/j - 1) = 0;
        X"100D_0000", -- r13(M[j - 1]) = 0;
        X"100E_0000", -- r14(M[j]) = 0;

        -- main for: for(r9(i) = 1; r9(i) <= r11(number of elements); ++r9(i));
          X"000A_0900", -- r10(j) = r9(i)
          --  local for: for(r10(j) = r9(i); r10(j) > 0; --r10(j));
            -- if
              X"110C_0A01", -- r12 = r10(j) - 1
              X"300D_0C00", -- r13 = M[r12(j - 1)]
              X"300E_0A00", -- r14 = M[r10(j)]
              X"010C_0D0E", -- r12(valToSwap) = r13(M[j - 1]) - r14(M[j])
              X"500A_0006", -- если r12 < 0, выходим из local for
              -- swap
              X"310D_0A00", -- M[r10(j)] = r13
              X"310E_0C00", -- M[r12(j - 1)] = r14
            X"110A_0A01", -- --r10(j);
            X"5001_00F7", -- если j != 0, возращаемся в начало local for
          X"1009_0901", -- ++r9(i);
          X"010C_0B09", -- r12 = r11(number of elements) - r9(i)
          X"5001_00F3", -- если r12 != 0, возращаемся в начало main for

        --End of programm, save data to registers
        X"3001_0000", -- r1 = M[0]
        X"3002_0001", -- r2 = M[1]
        X"3003_0002", -- r3 = M[2]
        X"3004_0003", -- r4 = M[3]
        X"3005_0004", -- r5 = M[4]
        X"3006_0005", -- r6 = M[5]
        X"3007_0006", -- r7 = M[6]
        X"3008_0007", -- r8 = M[7]

        OTHERS => X"00000000"
        );
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
